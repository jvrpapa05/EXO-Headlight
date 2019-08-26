// Both NL&CR @ Serial Monitor
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX
String message;
String data;

#include <Adafruit_NeoPixel.h>
#define PIXEL_PIN    4
#define PIXEL_COUNT 24
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
uint32_t b;
int count;
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  Serial.println("BEGIN");

  mySerial.begin(38400);
  mySerial.setTimeout(10);

  strip.begin();
  strip.show();
}

void loop() {

  if (Serial.available() > 0) {
    message = Serial.readString();
    Serial.println(message);
    mySerial.print(message);
  }

  if (mySerial.available() > 0) {
    data = mySerial.readStringUntil('\n');
    Serial.println(String("> ") + data);


  }
  startShow(data.toInt());
}

void startShow(int i) {
  switch (i) {

    case 0:
      mySerial.println("Connected --> OFF");
      colorWipe(strip.Color(0, 0, 0), 50);    // Black/off
      break;

    case 1:
      colorWipe(strip.Color(255, 0, 0), 50);  // Red
      mySerial.println("RED");
      break;

    case 2:
      mySerial.println("GREEN");
      colorWipe(strip.Color(0, 255, 0), 50);  // Green
      break;

    case 3:
      mySerial.println("BLUE");
      colorWipe(strip.Color(0, 0, 255), 50);  // Blue
      break;

    case 4:
      mySerial.println("THEATER CHASE - WHITE");
      theaterChase(strip.Color(127, 127, 127), 50); // White
      break;

    case 5:
      mySerial.println("THEATER CHASE - RED");
      theaterChase(strip.Color(127,   0,   0), 50); // Red
      break;

    case 6:
      mySerial.println("THEATER CHASE - BLUE");
      theaterChase(strip.Color(  0,   0, 127), 50); // Blue
      break;

    case 7:
      mySerial.println("RAINBOW");
      rainbow(20);
      break;

    case 8:
      mySerial.println("RAINBOW CYCLE");
      rainbowCycle(20);
      break;

    case 9:
      mySerial.println("THEATER CHASE - RAINBOW");
      theaterChaseRainbow(50);
      break;

    case 10:
      mySerial.println("VIOLET");
      colorWipe(strip.Color(255, 0, 255), 50);  // Violet
      break;

    case 11:
      mySerial.println("VIOLET - BLINK FAST");
      colorWipe(strip.Color(255, 0, 255), 0);  // Violet Blink
      delay(100);
      colorWipe(strip.Color(0, 0, 0), 0);
      delay(100);
      break;

    case 12:
      mySerial.println("VIOLET - BLINK SLOW");
      colorWipe(strip.Color(255, 0, 255), 0);  // Violet Blink
      delay(1000);
      colorWipe(strip.Color(0, 0, 0), 0);
      delay(1000);
      break;

    case 13:
      mySerial.println("VIOLET - FADE");

      do {
        colorWipe(strip.Color(count, 0, count), 0);
        count++;
        Serial.println(count);
      } while (count <= 254);

      do {
        colorWipe(strip.Color(count, 0, count), 0);
        count--;
        Serial.println(count);
      } while (count >= 1);

      break;

    case 14:
      mySerial.println("VIOLET WHITE - FADE");

      count = 0;

      do {
        count++;
        colorWipe(strip.Color(count, 0, count), 0);
        Serial.println(count);
      } while (count <= 254);

      count = 0;

      do {
        count++;
        colorWipe(strip.Color(255, count, 255), 0);
        Serial.println(count);
      } while (count <= 254);

      do {
        colorWipe(strip.Color(count, count, count), 0);
        count--;
        Serial.println(count);
      } while (count >= 1);
      delay(10);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
