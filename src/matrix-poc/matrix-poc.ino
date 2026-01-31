#include <Adafruit_NeoPixel.h>

#define PIN        1
#define NUMPIXELS 64

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 1500

const int matrix[64] = {
  0,0,0,0,0,0,0,0,
  1,0,0,0,0,0,1,0,
  0,0,0,0,0,0,0,0,
  0,0,1,1,1,0,0,0,
  0,0,1,1,1,0,0,0,
  0,0,1,1,1,0,0,0,
  0,0,0,0,0,0,0,0,
  1,0,0,0,0,0,1,0
};


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  pixels.begin();

  pixels.clear();
  pixels.show();

  Serial.println("setting pixels now");
  for(int i=0; i<NUMPIXELS; i++) {
    uint32_t color = 0;
    if (matrix[i] == 1) {
      color = pixels.Color(0, 100, 100);
    }
    pixels.setPixelColor(i, color);
    pixels.setBrightness(10);
  }
  pixels.show();
}

// the loop function runs over and over again forever
void loop() {
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
