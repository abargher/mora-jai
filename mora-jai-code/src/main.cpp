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

TaskHandle_t BlinkTaskHandle = NULL;
TaskHandle_t ButtonPollTaskHandle = NULL;

int readMux(int channel)
{
    int controlPin[] = {MUX_CH0, MUX_CH1, MUX_CH2, MUX_CH3};

    // TODO: replace with bit shifting
    int muxChannel[16][4] = {
        {0, 0, 0, 0}, // channel 0
        {1, 0, 0, 0}, // channel 1
        {0, 1, 0, 0}, // channel 2
        {1, 1, 0, 0}, // channel 3
        {0, 0, 1, 0}, // channel 4
        {1, 0, 1, 0}, // channel 5
        {0, 1, 1, 0}, // channel 6
        {1, 1, 1, 0}, // channel 7
        {0, 0, 0, 1}, // channel 8
        {1, 0, 0, 1}, // channel 9
        {0, 1, 0, 1}, // channel 10
        {1, 1, 0, 1}, // channel 11
        {0, 0, 1, 1}, // channel 12
        {1, 0, 1, 1}, // channel 13
        {0, 1, 1, 1}, // channel 14
        {1, 1, 1, 1}  // channel 15
    };

    // Tell the mux which channel's value to show on MUX_SIGNAL
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(controlPin[i], muxChannel[channel][i]);
    }

    int val = digitalRead(MUX_SIGNAL);
    // Serial.printf("reading %d value on mux out\n", val);

    return val;
}

volatile uint32_t buttonState = 0;
volatile uint32_t buttonsState[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// volatile uint32_t lastButtonState = LOW;
volatile uint32_t lastDebounceTime = 0;
const uint32_t debounceDelay = 50; // milliseconds

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
                Serial.printf("button %d changed to %d\n", i, readings[i]);
            }
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);

        // .check value of button
        // .write it to newState
        // .compare to lastState
        // .if different, send event with new state
        // .make lastState = newState, wipe newState clean
    }
}

void BlinkTask(void *parameter)
{
    while (true)
    {
        digitalWrite(LED_PIN, LOW);
        // Serial.println("BlinkTask: LED ON");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        digitalWrite(LED_PIN, HIGH);
        // Serial.println("BlinkTask: LED OFF");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // Serial.print("BlinkTask running on core ");
        // Serial.println(xPortGetCoreID());
    }
}

void setup()
{
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);
    pinMode(MUX_CH0, OUTPUT);
    pinMode(MUX_CH1, OUTPUT);
    pinMode(MUX_CH2, OUTPUT);
    pinMode(MUX_CH3, OUTPUT);
    pinMode(MUX_SIGNAL, INPUT);

    xTaskCreatePinnedToCore(
        BlinkTask,        // Task function
        "BlinkTask",      // Task name
        10000,            // Stack size (bytes)
        NULL,             // Parameters
        1,                // Priority
        &BlinkTaskHandle, // Task handle
        1                 // Core 1
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
