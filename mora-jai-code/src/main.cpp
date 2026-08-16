#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <ESP32Servo.h>

#define PIN D10
#define NUMPIXELS 22

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 1500

const int L0 = 8;
const int L1 = 14;
const int L2 = 56;
const int L3 = 62;
int led_idxs[4] = {L0, L1, L2, L3};

int matrix[64] = {
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1};
// int matrix[64] = {
//     0, 0, 0, 0, 0, 0, 0, 0,
//     1, 0, 0, 0, 0, 0, 1, 0,
//     0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 1, 1, 1, 0, 0, 0,
//     0, 0, 1, 1, 1, 0, 0, 0,
//     0, 0, 1, 1, 1, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0,
//     1, 0, 0, 0, 0, 0, 1, 0};

int readMux(int channel)
{
    int controlPin[] = {D0, D1, D2, D3};

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

    // loop through the 4 sig
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(controlPin[i], muxChannel[channel][i]);
    }

    // read the value at the SIG pin
    int val = digitalRead(D6);
    Serial.printf("reading %d value on mux out\n", val);

    // return the value
    return val;
}

Servo servo;
int servoPos = 0;

// the setup function runs once when you press reset or power the board
void setup()
{
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    servo.setPeriodHertz(50);
    servo.attach(D8, 1000, 2000);

    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    digitalWrite(D0, LOW);
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);

    // pull mux enable pin low (chip enables active low)
    pinMode(D7, OUTPUT);
    digitalWrite(D7, LOW);

    // motor control pin
    // pinMode(D9, OUTPUT);

    pinMode(D6, INPUT);

    Serial.begin(115200);
    pixels.begin();

    pixels.clear();
    pixels.show();

    Serial.println("setting pixels now");
    for (int i = 0; i < NUMPIXELS; i++)
    {
        uint32_t color = 0;
        if (matrix[i] == 1)
        {
            color = pixels.Color(0, 100, 100);
        }
        else if (matrix[i] == 2)
        {
            color = pixels.Color(0, 100, 0);
        }
        pixels.setPixelColor(i, color);
        pixels.setBrightness(10);
    }
    pixels.show();
}

typedef enum
{
    GRAY,
    BLACK,
    RED,
    YELLOW,
    VIOLET,
    ORANGE,
    WHITE,
    PINK,
    GREEN,
    BLUE,
    COLOR_COUNT // number of distinct button colors
} ButtonColor;

typedef struct
{

} PuzzleState;

// the loop function runs over and over again forever
void loop()
{
    for (int i = 0; i < 4; i++)
    {
        matrix[led_idxs[i]] = !readMux(i) + 1;
    }

    for (int i = 0; i < NUMPIXELS; i++)
    {
        uint32_t color = 0;
        if (matrix[i] == 1)
        {
            color = pixels.Color(0, 100, 100);
        }
        else if (matrix[i] == 2)
        {
            color = pixels.Color(0, 100, 0);
        }
        pixels.setPixelColor(i, color);
        pixels.setBrightness(10);
    }

    pixels.show();

    bool motorOn = false;
    if (motorOn)
    {
        for (servoPos = 0; servoPos <= 180; servoPos += 1)
        { // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            servo.write(servoPos); // tell servo to go to position in variable 'pos'
            delay(5);              // waits 5ms for the servo to reach the position
        }
        for (servoPos = 180; servoPos >= 0; servoPos -= 1)
        {                          // goes from 180 degrees to 0 degrees
            servo.write(servoPos); // tell servo to go to position in variable 'pos'
            delay(5);              // waits 5ms for the servo to reach the position
        }
    }
    /*
        - Set initial state of puzzle

        interaction loop:
        - write LED colors from puzzle state
        - Poll state of all buttons
        - if button pressed, update state of puzzle


    */

    // Serial.printf("b1 = %d, b2 = %d\n", b1, b2);
    // pixels.clear();
    // pixels.show();
    // delay(DELAYVAL);
    // Serial.println("setting pixels now");
    // for(int i=0; i<NUMPIXELS; i++) {
    //   uint32_t color = 0;
    //   if (matrix[i] == 1) {
    //     color = pixels.Color(0, 100, 100);
    //   }
    //   pixels.setPixelColor(i, color);
    //   pixels.setBrightness(10);
    // }

    // pixels.show();
    // delay(DELAYVAL);
}
