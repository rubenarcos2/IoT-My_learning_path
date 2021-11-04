#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Mqtt_utils.h"
#include <JSON_Decoder.h>
#include <OpenWeather.h>
#include "API_Openweather.h"
#include <WiFiUdp.h>
#include <NTPClient.h>               
#include <TimeLib.h>    

#define DHTPIN D4     // Especifica el núnero de pin al que se conecta el sensor DHT
#define DHTTYPE DHT11   // DHT 11

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 7200, 60000);

char Time[ ] = "00:00";
char Date[ ] = "00/00/2000";
byte last_second, second_, minute_, hour_, day_, month_;
int year_;

const int LED_RAIN_1_HOUR = 14;
const int LED_RAIN_2_HOUR = 12;
const int LED_RAIN_1_DAY = 13;

void setup() {
  pinMode(LED_RAIN_1_HOUR, OUTPUT);
  pinMode(LED_RAIN_2_HOUR, OUTPUT);
  pinMode(LED_RAIN_1_DAY, OUTPUT);

  Serial.begin(115200);
  delay(1000);
  Serial.println("[INFO] Iniciando...");

  //Init LCD
  lcd.begin();
  lcd.backlight();
  lcd.print("Iniciando...");
  delay(1000);

  dht.begin();
  Serial.println("[INFO] Actualizando DHT... ");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Obteniendo");
  lcd.setCursor(0, 1);
  lcd.print("la temperatura...");
  delay(1000);

  Serial.println("[INFO] Conectando a internet... ");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conectando");
  lcd.setCursor(0, 1);
  lcd.print("a internet...");
  setup_wifi();

  //Init NTP
  timeClient.begin();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Actualizando");
  lcd.setCursor(0, 1);
  lcd.print("la hora...");
  delay(1000);

  //Openweathermap
  getWeather();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Actualizando");
  lcd.setCursor(0, 1);
  lcd.print("el tiempo...");
  delay(1000);

  Serial.println("[INFO] Conectando a Mqtt... ");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conectando");
  lcd.setCursor(0, 1);
  lcd.print("a centralita...");
  client.setClient(espClient);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

bool checkBound(float newValue, float prevValue, float maxDiff) {
  return newValue < prevValue - maxDiff || newValue > prevValue + maxDiff;
}

long lastMsg = 0;
float temp = 0.0;
float hum = 0.0;
float diff = 1.0;

void loop() {
  long nowRefresh = millis();

  if (!client.connected()) {
    reconnect();
  } else {
    client.loop();

    // Nueva lectura de Temperatura y Humedad cada 10 segundos
    if (nowRefresh - lastMsg > 10000) {
      lastMsg = nowRefresh;

      float newHum = dht.readHumidity();
      float newTemp = dht.readTemperature(false);
      delay(10);

      // Si la nueva lectura de temperatura es diferente de la anterior envía el nuevo valor
      if (checkBound(newTemp, temp, diff)) {
        temp = newTemp;
        Serial.print("New temperature:");
        Serial.println(String(temp).c_str());
        client.publish(temperature_topic, String(temp).c_str(), true);
      }

      // Si la nueva lectura de humedad es diferente de la anterior envía el nuevo valor
      if (checkBound(newHum, hum, diff)) {
        hum = newHum;
        Serial.print("New humidity:");
        Serial.println(String(hum).c_str());
        client.publish(humidity_topic, String(hum).c_str(), true);
      }
    }
  }

  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(" ");
  temp = dht.readTemperature(false);
  delay(10);
  lcd.print(temp);
  lcd.print("C");
  lcd.print("  ");
  hum = dht.readHumidity();
  delay(10);
  lcd.print(hum);
  lcd.print("%  ");

  lcd.setCursor(0, 0);
  timeClient.update();
  unsigned long unix_epoch = timeClient.getEpochTime();    // Get Unix epoch time from the NTP server

  second_ = second(unix_epoch);
  if (last_second != second_) {
 

    minute_ = minute(unix_epoch);
    hour_   = hour(unix_epoch);
    day_    = day(unix_epoch);
    month_  = month(unix_epoch);
    year_   = year(unix_epoch);

    Time[4]  = minute_ % 10 + 48;
    Time[3]  = minute_ / 10 + 48;
    Time[1]  = hour_   % 10 + 48;
    Time[0]  = hour_   / 10 + 48;

    Date[0]  = day_   / 10 + 48;
    Date[1]  = day_   % 10 + 48;
    Date[3]  = month_  / 10 + 48;
    Date[4]  = month_  % 10 + 48;
    Date[8] = (year_   / 10) % 10 + 48;
    Date[9] = year_   % 10 % 10 + 48;

    Serial.println(Time);
    Serial.println(Date);
    lcd.print(Time);
    lcd.print(" ");
    lcd.print(Date);
    last_second = second_;

  }
  delay(30000);

  lcd.clear();
  lcd.print("Prevision +1 hora: ");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(weather_1h_Temp);
  lcd.print("C ");
  lcd.print(" ");
  lcd.print(weather_1h_Hum);
  lcd.print("% ");
  lcd.setCursor(0, 1);
  lcd.print(weather_1h_Desc);
  if (lluvia_1h < 600) {
    digitalWrite(LED_RAIN_1_HOUR, HIGH);
  } else {
    digitalWrite(LED_RAIN_1_HOUR, LOW);
  }
  delay(4000);

  lcd.clear();
  lcd.print("Prevision +2 horas: ");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(weather_2h_Temp);
  lcd.print("C ");
  lcd.print(" ");
  lcd.print(weather_2h_Hum);
  lcd.print("% ");
  lcd.setCursor(0, 1);
  lcd.print(weather_2h_Desc);
  if (lluvia_2h < 600) { //Soleado
    digitalWrite(LED_RAIN_2_HOUR, HIGH);
  } else {
    digitalWrite(LED_RAIN_2_HOUR, LOW);
  }
  delay(4000);

  lcd.clear();
  lcd.print("Prevision +1 dia: ");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Min ");
  lcd.print(weather_1TempMin);
  lcd.print("C ");
  lcd.setCursor(0, 1);
  lcd.print("Max ");
  lcd.print(weather_1TempMax);
  lcd.print("C ");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(weather_1Hum);
  lcd.print("% ");
  lcd.setCursor(0, 1);
  lcd.print(weather_1Desc);
  if (lluvia_1d < 600) {//Soleado
    digitalWrite(LED_RAIN_1_DAY, HIGH);
  } else {
    digitalWrite(LED_RAIN_1_DAY, LOW);
  }
  delay(2000);

  lcd.clear();
  lcd.print("Prevision +2 dia: ");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Min ");
  lcd.print(weather_2TempMin);
  lcd.print("C ");
  lcd.setCursor(0, 1);
  lcd.print("Max ");
  lcd.print(weather_2TempMax);
  lcd.print("C ");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(weather_2Hum);
  lcd.print("% ");
  lcd.setCursor(0, 1);
  lcd.print(weather_2Desc);
  delay(2000);

  if (nowRefresh - lastMsg > 360000) {
    lastMsg = nowRefresh;
    getWeather();
  }
}
