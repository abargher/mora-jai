#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <ESP32Servo.h>
#include "LittleFS.h"

#define LED_PIN LED_BUILTIN

#define MUX_SIGNAL D3
#define MUX_CH0 D1
#define MUX_CH1 D2
#define MUX_CH2 D0
#define MUX_CH3 D4

#define SERVO_CTRL_PIN D8

#define INPUT_POLL_WAIT_MS 50

#define LED_DATA_PIN D5
#define NUM_PIXELS 22

#define QUEUE_SIZE 5

typedef struct
{
    uint32_t buttonNum;
    bool isPressed; // pressed = true, released = false;
} buttonUpdate_t;

typedef struct
{
    uint32_t index;
    uint32_t color;
} ledUpdate_t;

QueueHandle_t buttonUpdateQueue = NULL;
QueueHandle_t ledUpdateQueue = NULL;

TaskHandle_t ServoSweepTaskHandle = NULL;
TaskHandle_t ButtonPollTaskHandle = NULL;
TaskHandle_t LEDUpdateTaskHandle = NULL;
TaskHandle_t GameLogicMockTaskHandle = NULL;

Adafruit_NeoPixel pixels(NUM_PIXELS, LED_DATA_PIN, NEO_GRB + NEO_KHZ800);
Servo servo;
int servoPos = 0;

int readMux(int channel)
{
    // Tell the mux which channel's value to show on MUX_SIGNAL
    digitalWrite(MUX_CH0, (channel >> 0) & 0x1);
    digitalWrite(MUX_CH1, (channel >> 1) & 0x1);
    digitalWrite(MUX_CH2, (channel >> 2) & 0x1);
    digitalWrite(MUX_CH3, (channel >> 3) & 0x1);

    int val = digitalRead(MUX_SIGNAL);
    // Serial.printf("reading %d value on mux out\n", val);

    return val;
}

volatile uint32_t buttonsState[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void ButtonPollTask(void *parameter)
{
    while (true)
    {
        uint32_t readings[16];
        for (int i = 0; i < 16; i++)
        {
            readings[i] = (uint32_t)(!readMux(i));
        }

        for (int i = 0; i < 16; i++)
        {
            if (readings[i] != buttonsState[i])
            {
                buttonsState[i] = readings[i];
                buttonUpdate_t updateEvent = {i, (bool)readings[i]};
                xQueueSend(buttonUpdateQueue, &updateEvent, 0); // 0 returns immediate if queue is full
                Serial.printf("sent event: button %d changed to %d\n", i, readings[i]);
            }
        }
        vTaskDelay(INPUT_POLL_WAIT_MS / portTICK_PERIOD_MS);
    }
}

uint32_t colors[NUM_PIXELS] = {0};
void LEDUpdateTask(void *parameter)
{
    // consume LED Update events
    // set LED colors accordingly
    while (true)
    {
        ledUpdate_t lightUpdate;
        if (xQueueReceive(ledUpdateQueue, &lightUpdate, portMAX_DELAY))
        {
            uint32_t index = lightUpdate.index;
            uint32_t color = lightUpdate.color;
            colors[index] = color;
            pixels.setPixelColor(index, color);
            pixels.show();
        }
    }
}

bool ledToggleStates[7] = {false, false, false, false, false, false, false};
void GameLogicMockTask(void *parameter)
{
    // consume button state events
    // dispatch corresponding LED Update events
    while (true)
    {
        buttonUpdate_t updateEvent;
        if (xQueueReceive(buttonUpdateQueue, &updateEvent, portMAX_DELAY))
        {
            uint32_t buttonIndex = updateEvent.buttonNum;
            bool isButtonPressed = updateEvent.isPressed;
            if (buttonIndex < 4 && isButtonPressed)
            {
                ledToggleStates[buttonIndex] = !ledToggleStates[buttonIndex];
                Serial.printf("sent event: toggled light %d\n", buttonIndex);

                uint32_t color = ledToggleStates[buttonIndex] ? pixels.Color(0, 100, 100) : pixels.Color(0, 100, 0);
                ledUpdate_t lightUpdate = {buttonIndex, color};
                xQueueSend(ledUpdateQueue, &lightUpdate, 0);
            }
            if (buttonIndex == 4 && isButtonPressed)
            {
                servo.write(0);
            }
            if (buttonIndex == 5 && isButtonPressed)
            {
                servo.write(90);
            }
            if (buttonIndex == 6 && isButtonPressed)
            {
                servo.write(180);
            }
        }
    }
}

#define SERVO_WAIT_MS 15
void ServoSweepTask(void *parameter)
{
    for (servoPos = 0; servoPos <= 180; servoPos += 1)
    {
        servo.write(servoPos);
        vTaskDelay(SERVO_WAIT_MS / portTICK_PERIOD_MS);
    }
    for (servoPos = 180; servoPos >= 0; servoPos -= 1)
    {
        servo.write(servoPos);
        vTaskDelay(SERVO_WAIT_MS / portTICK_PERIOD_MS);
    }
}

#define SERVO_MIN 500
#define SERVO_MAX 2500
void SetupServo()
{
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    servo.setPeriodHertz(50);
    servo.attach(SERVO_CTRL_PIN, SERVO_MIN, SERVO_MAX);
}

void SetupPixels()
{
    pixels.begin();
    pixels.clear();
    pixels.show();

    pixels.setBrightness(10);
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        uint32_t color = pixels.Color(0, 100, 0);
        pixels.setPixelColor(i, color);
    }
    pixels.show();
}

void setup()
{
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);
    pinMode(LED_DATA_PIN, OUTPUT);

    pinMode(MUX_CH0, OUTPUT);
    pinMode(MUX_CH1, OUTPUT);
    pinMode(MUX_CH2, OUTPUT);
    pinMode(MUX_CH3, OUTPUT);
    pinMode(MUX_SIGNAL, INPUT);

    SetupServo();
    SetupPixels();

    // setup queues
    buttonUpdateQueue = xQueueCreate(QUEUE_SIZE, sizeof(buttonUpdate_t));
    if (buttonUpdateQueue == NULL)
    {
        Serial.println("failed to create button queue!");
        while (1)
            ;
    }
    ledUpdateQueue = xQueueCreate(QUEUE_SIZE, sizeof(ledUpdate_t));
    if (ledUpdateQueue == NULL)
    {
        Serial.println("failed to create LED queue!");
        while (1)
            ;
    }

    // setup tasks
    xTaskCreatePinnedToCore(
        GameLogicMockTask,        // Task function
        "GameLogicMockTask",      // Task name
        3000,                     // Stack size (bytes)
        NULL,                     // Parameters
        1,                        // Priority
        &GameLogicMockTaskHandle, // Task handle
        1                         // Core 1
    );

    xTaskCreatePinnedToCore(
        LEDUpdateTask,        // Task function
        "LEDUpdateTask",      // Task name
        3000,                 // Stack size (bytes)
        NULL,                 // Parameters
        1,                    // Priority
        &LEDUpdateTaskHandle, // Task handle
        1                     // Core 1
    );

    // xTaskCreatePinnedToCore(
    //     ServoSweepTask,        // Task function
    //     "ServoSweepTask",      // Task name
    //     3000,                  // Stack size (bytes)
    //     NULL,                  // Parameters
    //     1,                     // Priority
    //     &ServoSweepTaskHandle, // Task handle
    //     1                      // Core 1
    // );

    xTaskCreatePinnedToCore(
        ButtonPollTask,        // Task function
        "ButtonPollTask",      // Task name
        6000,                  // Stack size (bytes)
        NULL,                  // Parameters
        1,                     // Priority
        &ButtonPollTaskHandle, // Task handle
        0                      // Core 0
    );
}

void loop()
{
    // Empty because FreeRTOS scheduler runs the task
}
