#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_NeoPixel.h>

#define NUM_SEG 4
#define NUM_DOT 1
#define DATA_PIN 4
int ColorPotPin = A0;
int BrightnessPin = D5;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_SEG * 7 + NUM_DOT * 2, DATA_PIN, NEO_GRB + NEO_KHZ800);

byte digits[12][7] = { {1, 1, 0, 1, 1, 1, 1}, // Digit 0
  {0, 0, 0, 1, 0, 0, 1}, // Digit 1
  {1, 1, 1, 1, 1, 0, 0}, // Digit. 2
  {1, 0, 1, 1, 1, 0, 1}, // Digit 3
  {0, 0, 1, 1, 0, 1, 1}, // Digit 4
  {1, 0, 1, 0, 1, 1, 1}, // Digit 5
  {1, 1, 1, 0, 1, 1, 1}, // Digit 6
  {0, 0, 0, 1, 1, 0, 1}, // Digit 7
  {1, 1, 1, 1, 1, 1, 1}, // Digit 8
  {1, 0, 1, 1, 1, 1, 1}, // Digit 9
  {0, 0, 1, 1, 1, 1, 0}, // Digit o
  {1, 1, 0, 0, 1, 1, 0}
};  // Digit C

byte leds[NUM_SEG * 7];

byte gr = 0;
bool grb = false;

#include "color_ctl.h"
#include "NTP_client.h"

int countBrightness = 0;

ICACHE_RAM_ATTR void changeBrightness() {
  if (countBrightness < 255) {
    countBrightness++;
    Serial.println(countBrightness);
    strip.setBrightness(countBrightness);
  } else {
    countBrightness = 0;
  }
}

void setup() {
  Serial.begin(115200);

  initColorCtl();
  strip.begin();
  strip.setBrightness(150);

  pinMode(BrightnessPin, INPUT_PULLUP);
  attachInterrupt(BrightnessPin, changeBrightness, CHANGE);

  Serial.println("NTP Client get hour");
  initNTPClient();
}

void setDigit (byte numder, byte index)
{
  for (byte i = 1; i <= 2; i++)
  {
    byte digit = numder % 10;
    numder /= 10;
    for (byte j = 0; j < 7; j++)
    {
      if ((digits[digit][j] == 0) && (leds[(index * 2 - i) * 7 + j] == 1))
      {
        leds[(index * 2 - i) * 7 + j] = 2; //MARKER TO DEL SEGMENT
        gr = 0;
        // grb=true;
      }
      if ((digits[digit][j] == 1) && (leds[(index * 2 - i) * 7 + j] == 0))
      {
        leds[(index * 2 - i) * 7 + j] = 3; //MARKER TO ADD SEGMENT
        gr = 0;
        // grb=true;
      }
    }
  }
}

void loop() {
  DateTime dt = getNTPHour();

  // For leading zero look to DS3231_dateformat example
  //  Serial.print("Raw data: ");
  //  Serial.print(dt.year());   Serial.print("-");
  //  Serial.print(dt.month());  Serial.print("-");
  //  Serial.print(dt.day());    Serial.print(" ");
  //  Serial.print(dt.hour());   Serial.print(":");
  //  Serial.print(dt.minute()); Serial.print(":");
  //  Serial.print(dt.second()); Serial.println("");

  uint32_t color = refreshColorCtl();
  setDigit(dt.hour() , 1);
  setDigit(dt.minute() , 2);

  //if (grb)  {
  gr++;
  //}

  if (gr > 254)
  {
    for (byte i = 0; i < NUM_SEG * 7; i++)
    {
      if (leds[i] == 2)   {
        leds[i] = 0;
      }
      if (leds[i] == 3)   {
        leds[i] = 1;
      }
    }
    gr = 0;
    //grb=false;
  }

  for (byte i = 0; i < NUM_SEG; i++)
  {
    for (byte j = 0; j < 7; j++)
    {

      if (leds[i * 7 + j] == 0)
      {
        strip.setPixelColor(i * 7 + j, 0x000000);
      }
      if (leds[i * 7 + j] == 1)
      {
        //strip.setPixelColor(i * 7 + j, strip.Color(254, 254, 254));
        strip.setPixelColor(i * 7 + j, color);
      }
      if (leds[i * 7 + j] == 2)
      {
        strip.setPixelColor(i * 7 + j, strip.Color(254 - gr, 254 - gr, 254 - gr));
      }
      if (leds[i * 7 + j] == 3)
      {
        strip.setPixelColor(i * 7 + j, strip.Color(gr, gr, gr));
      }
    }
  }
  if (dt.second() % 2 == 0)
  {
    strip.setPixelColor(NUM_SEG * 7, 0x000000);
    strip.setPixelColor(NUM_SEG * 7 + 1, 0x000000);
  }
  else
  {
    strip.setPixelColor(NUM_SEG * 7, color);
    strip.setPixelColor(NUM_SEG * 7 + 1, color);
  }
  strip.show();
  //delay(10);
}
