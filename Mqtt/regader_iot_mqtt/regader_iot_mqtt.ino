#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <Wire.h>
#include <ArduinoOTA.h>

#define wifi_ssid ""
#define wifi_password ""

#define mqtt_server "192.168.1.195"
#define mqtt_port 1883
//#define mqtt_user ""
//#define mqtt_password ""


#define pump_topic "topicName/pump" // Bomba de agua
#define humidity_topic "topicName/humidity" // Sensor analógico de humedad
#define tank_topic "topicName/tank" // Boya tanque
#define led_empty_topic "topicName/empty" // LED de depósito vacío
#define led_watering_topic "topicName/watering" // LED de regando

// PINOUTS GPIO
const int PIN_RELE_BOMBA_AGUA =      12;
const int PIN_SENSOR_BOYA_DEPOSITO = 0;
const int PIN_LED_DEPOSITO_VACIO =   14;
const int PIN_LED_REGANDO =          15;
const int PIN_SENSOR_HUMEDAD =       A0;

WiFiClient espClient;
PubSubClient client;

void setup() {
  Serial.begin(115200);
  delay(1000);
  setup_wifi();
  client.setClient(espClient);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  pinMode(PIN_RELE_BOMBA_AGUA, OUTPUT);
  digitalWrite(PIN_RELE_BOMBA_AGUA, HIGH);
  pinMode(PIN_SENSOR_BOYA_DEPOSITO, INPUT_PULLUP);
  pinMode(PIN_SENSOR_HUMEDAD, INPUT);
  pinMode(PIN_LED_REGANDO, OUTPUT);
  digitalWrite(PIN_LED_REGANDO, LOW);
  pinMode(PIN_LED_DEPOSITO_VACIO, OUTPUT);

  initOTA();
}

bool checkBound(float newValue, float prevValue, float maxDiff) {
  return newValue < prevValue - maxDiff || newValue > prevValue + maxDiff;
}

long lastMsg = 0;

int hum = 0;
int tank = 0;
float diff = 1.0;

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();

  // Nueva lectura de Temperatura y Humedad cada 10 segundos
  if (now - lastMsg > 10000) {
    lastMsg = now;
    int newHum = analogRead(PIN_SENSOR_HUMEDAD);
    int newTank = digitalRead(PIN_SENSOR_BOYA_DEPOSITO);

    // Si la nueva lectura de humedad es diferente de la anterior envía el nuevo valor
    if (checkBound(newHum, hum, diff)) {
      hum = newHum;
      Serial.print("New humidity:");
      Serial.println(String(hum).c_str());
      client.publish(humidity_topic, String(hum).c_str(), true);
    }

    if (checkBound(newTank, tank, diff)) {
      tank = newTank;
      Serial.print("New tank:");
      Serial.println(String(tank).c_str());
      client.publish(tank_topic, String(tank).c_str(), true);
    }
  }
}
