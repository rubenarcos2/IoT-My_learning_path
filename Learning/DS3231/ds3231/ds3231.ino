#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

uint8_t alarma_hora = 11;
uint8_t alarma_min = 15;
String hora_actual = "00:00:00";
void setup () {
    Serial.begin(115200);
    Wire.begin();
    rtc.begin();

    if (! rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }
    
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    if (rtc.lostPower()) {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        //rtc.adjust(DateTime(2015, 1, 1, 3, 0, 0));
    }

}

void loop () {
  
    DateTime now = rtc.now();
    
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.println(now.second(), DEC);
    Serial.println(hora_actual);
    delay(1000);

    if (now.hour() == alarma_hora && now.minute() == alarma_min){
      Serial.println("Es la hora");
    }
}
