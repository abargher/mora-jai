#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LittleFS.h>

#include <pins.h>
#include <hardware.hpp>

#define INPUT_POLL_WAIT_MS 100
#define QUEUE_SIZE 5

// OLED Example setup
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16
static const unsigned char PROGMEM logo_bmp[] =
    {0b00000000, 0b11000000,
     0b00000001, 0b11000000,
     0b00000001, 0b11000000,
     0b00000011, 0b11100000,
     0b11110011, 0b11100000,
     0b11111110, 0b11111000,
     0b01111110, 0b11111111,
     0b00110011, 0b10011111,
     0b00011111, 0b11111100,
     0b00001101, 0b01110000,
     0b00011011, 0b10100000,
     0b00111111, 0b11100000,
     0b00111111, 0b11110000,
     0b01111100, 0b11110000,
     0b01110000, 0b01110000,
     0b00000000, 0b00110000};
// end OLED Example

QueueHandle_t buttonUpdateQueue = NULL;
QueueHandle_t ledUpdateQueue = NULL;

TaskHandle_t ButtonPollTaskHandle = NULL;
TaskHandle_t LEDUpdateTaskHandle = NULL;
TaskHandle_t ButtonEventConsumerTaskHandle = NULL;

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
    vTaskDelay(0.1 / portTICK_PERIOD_MS);

    int val = digitalRead(MUX_SIGNAL);

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

        for (uint32_t i = 0; i < 16; i++)
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
void ButtonEventConsumerTask(void *parameter)
{
    // consume button state events
    // dispatch corresponding LED Update events
    while (true)
    {
        buttonUpdate_t updateEvent;
        if (xQueueReceive(buttonUpdateQueue, &updateEvent, portMAX_DELAY))
        {
            ExecuteCallbacks(updateEvent);
        }
    }
}

void ToggleLights(buttonUpdate_t updateEvent)
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

// #define SERVO_WAIT_MS 15
// void ServoSweepTask(void *parameter)
// {
//     for (servoPos = 0; servoPos <= 180; servoPos += 1)
//     {
//         servo.write(servoPos);
//         vTaskDelay(SERVO_WAIT_MS / portTICK_PERIOD_MS);
//     }
//     for (servoPos = 180; servoPos >= 0; servoPos -= 1)
//     {
//         servo.write(servoPos);
//         vTaskDelay(SERVO_WAIT_MS / portTICK_PERIOD_MS);
//     }
// }

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

void testdrawstyles(void)
{
    display.clearDisplay();

    display.setTextSize(1);              // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 0);             // Start at top-left corner
    display.println(F("Hello, world!"));

    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
    display.println(3.141592, 6);                       // print a double with 6 decimal places

    display.setTextSize(2); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.print(F("0x"));
    display.println(0xDEADBEEF, HEX);

    display.display();
    delay(2000);
}

void SetupDisplay()
{
    vTaskDelay(500 / portTICK_PERIOD_MS); // wait for display
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }
    display.display();
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    testdrawstyles(); // Draw 'stylized' characters
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

    digitalWrite(LED_PIN, LOW); // built-in LED is active-low

    SetupServo();
    SetupPixels();
    SetupDisplay();

    SetupEventLocks();

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

    RegisterButtonDownCallback(ToggleLights);
    RegisterButtonUpCallback(ToggleLights);

    // setup tasks
    xTaskCreatePinnedToCore(
        ButtonEventConsumerTask,        // Task function
        "ButtonEventConsumerTask",      // Task name
        3000,                           // Stack size (bytes)
        NULL,                           // Parameters
        1,                              // Priority
        &ButtonEventConsumerTaskHandle, // Task handle
        1                               // Core 1
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
