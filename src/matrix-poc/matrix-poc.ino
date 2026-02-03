#include <Adafruit_NeoPixel.h>

#define PIN        1
#define NUMPIXELS 64

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 1500

const int L0 = 8;
const int L1 = 14;
const int L2 = 56;
const int L3 = 62;
int led_idxs[4] = {L0,L1,L2,L3};

int matrix[64] = {
  0,0,0,0,0,0,0,0,
  1,0,0,0,0,0,1,0,
  0,0,0,0,0,0,0,0,
  0,0,1,1,1,0,0,0,
  0,0,1,1,1,0,0,0,
  0,0,1,1,1,0,0,0,
  0,0,0,0,0,0,0,0,
  1,0,0,0,0,0,1,0
};

int readMux(int channel){
  int controlPin[] = {D1, D2, D4, D5};

  int muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  };

  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }

  //read the value at the SIG pin
  int val = digitalRead(D3);

  //return the value
  return val;
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D4, LOW);
  digitalWrite(D5, LOW);

  pinMode(D3, INPUT_PULLUP);

  Serial.begin(115200);
  pixels.begin();

  pixels.clear();
  pixels.show();

  Serial.println("setting pixels now");
  for(int i=0; i<NUMPIXELS; i++) {
    uint32_t color = 0;
    if (matrix[i] == 1) {
      color = pixels.Color(0, 100, 100);
    } else if (matrix[i] == 2) {
      color = pixels.Color(0, 100, 0);
    }
    pixels.setPixelColor(i, color);
    pixels.setBrightness(10);
  }
  pixels.show();
}

// the loop function runs over and over again forever
void loop() {
  
  for(int i = 0; i < 4; i ++){
    matrix[led_idxs[i]] = !!readMux(i) + 1;
  }

  for(int i=0; i<NUMPIXELS; i++) {
    uint32_t color = 0;
    if (matrix[i] == 1) {
      color = pixels.Color(0, 100, 100);
    } else if (matrix[i] == 2) {
      color = pixels.Color(0, 100, 0);
    }
    pixels.setPixelColor(i, color);
    pixels.setBrightness(10);
  }
  pixels.show();

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
