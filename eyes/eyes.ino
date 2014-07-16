#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
#include <avr/power.h>
#endif
#define PIN            4
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(28, PIN, NEO_GRB + NEO_KHZ800);
#include "TinyWireS.h"                  // wrapper class for I2C slave routines
#define I2C_SLAVE_ADDR  0x26            // i2c slave address (38)
int rgb[] = {0, 0, 0};

void setup() {
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
  // Seed random number generator from an unused analog input:
  randomSeed(analogRead(2));
#else
  randomSeed(analogRead(A0));
#endif
  pixels.begin();
  pixels.setBrightness(64);
  delay(150);
  //  rainbow(10);
  lookLeft();
  delay(1000);
  clearpixels();
  lookRight();
  delay(1000);
  clearpixels();
  lookUp();
  delay(1000);
  clearpixels();
  lookDown();
  delay(1000);
  clearpixels();
  pulseGreen();
  TinyWireS.begin(I2C_SLAVE_ADDR);      // init I2C Slave mode
}
void loop() {
  byte byteRcvd = 0;
  
  if (TinyWireS.available()){           // got I2C input!
    for(int i=0; i<3; i++) {
      rgb[i] = int(TinyWireS.receive());
    }
    pulseRGB(rgb[0], rgb[1], rgb[2]);
  }
//  pulse(pixels.Color(0, 128, 0));
//  int rgbcolor = pixels.Color(rgb[0], rgb[1], rgb[2]);
//  pulse(rgbcolor);
//    colorWipe(pixels.Color(rgb[0], rgb[1], rgb[2]), 150);
    pixels.Color(rgb[0], rgb[1], rgb[2]);
}

void angry() {
  pixels.setPixelColor(0,185,0,0);
  pixels.setPixelColor(1,185,0,0);
  pixels.setPixelColor(2,185,0,0);
  pixels.setPixelColor(14,185,0,0);
  pixels.setPixelColor(15,185,0,0);

  pixels.setPixelColor(16,185,0,0);
  pixels.setPixelColor(17,185,0,0);
  pixels.setPixelColor(18,185,0,0);
  pixels.setPixelColor(27,185,0,0);
  pixels.show();
  delay(2000);
}
void clearpixels() {
  for (int8_t x=0; x<=27; x++) {
    pixels.setPixelColor(x,0,0,0);
    pixels.show();
  }
}
void lookLeft() {
  clearpixels();
  pixels.setPixelColor(5,45,45,45);
  pixels.setPixelColor(27,45,45,45);
  pixels.show();
}
void lookRight() {
  clearpixels();
  pixels.setPixelColor(14,45,45,45);
  pixels.setPixelColor(21,45,45,45);
  pixels.show();
}
void lookUp() {
  clearpixels();
  pixels.setPixelColor(1,45,45,45);
  pixels.setPixelColor(19,45,45,45);
  pixels.show();
}
void lookDown() {
  clearpixels();
  pixels.setPixelColor(9,45,45,45);
  pixels.setPixelColor(24,45,45,45);
  pixels.show();
}

void pulseGreen() {
  uint16_t i, j;

  for(j=0; j<128; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i,0,j,0);
    }
    pixels.show();
    delay(10);
  }
  for(j=128; j>0; j--) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i,0,j,0);
    }
    pixels.show();
    delay(10);
  }
}
void chatting() {
  uint16_t i, j;
  j = int(20);
  for(i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i,0,j,0);
  }
  pixels.show();
  delay(10);

}

void pulse(uint32_t c) {
  uint16_t i, j;
  
  for(j=0; j<128; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, c);
    }
    pixels.show();
    delay(10);
  }
  for(j=128; j>0; j--) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, c);
    }
    pixels.show();
    delay(10);
  }
}

void pulseRGB(int8_t R, int8_t G, int8_t B) {
  uint16_t i, j;

  for(j=0; j<128; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, (R)+j, (G)+j, (B)+j);
    }  
    pixels.show();
    delay(10);
  }
  for(j=128; j>0; j--) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, (R)+j, (G)+j, (B)+j);    
    }
    pixels.show();
    delay(10);
  }
}


uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void Blink(byte led, byte times){ // poor man's display
  for (byte i=0; i< times; i++){
    digitalWrite(led,HIGH);
    delay (250);
    digitalWrite(led,LOW);
    delay (175);
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, c);
    pixels.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

