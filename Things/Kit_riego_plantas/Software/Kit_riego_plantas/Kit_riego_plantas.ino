#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include "Wifi_Utils.hpp"
#include "Server.hpp"
#include "WebSockets_Utils.hpp"
#include "NTP_Time_Utils.hpp"
//#include "RTC_Clock_Utils.hpp"
//#include "RTC_DS3231_Utils.hpp"

const String HORARIO_RIEGO = "19:10"; //Formato: HH:mm
const int NUM_RIEGOS = 3; //Cantidad de veces que riega hasta el % de humedad en suelo
const int PORC_HUMEDAD_SUELO_RIEGO = 30; //Riega hasta alcanzar el % de humedad en suelo

// PINES GPIO
const int PIN_RELE_BOMBA_AGUA =      12;
const int PIN_SENSOR_BOYA_DEPOSITO = 0;
const int PIN_LED_DEPOSITO_VACIO =   14;
const int PIN_LED_REGANDO =          15;
const int PIN_SENSOR_HUMEDAD =       A0;

bool hibernado = false;
int num_riegos_realizados = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);

  initSensorDeposito(PIN_SENSOR_BOYA_DEPOSITO);
  initReleBombaAgua(PIN_RELE_BOMBA_AGUA);
  initSensorHumedadSuelo(PIN_SENSOR_HUMEDAD);
  initLedDepositoVacio(PIN_LED_DEPOSITO_VACIO);
  initLedRegando(PIN_LED_REGANDO);

  ConnectWiFi_STA(true);
  initOTA();
  InitWebSockets();
  InitServer();
  initNTP_Time();
  //initRTC();
  //Serial.println(getRTCTime() + " Temp: " + getRTCTemp());
  //setRTCAlarm1(HORARIO_RIEGO);

  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  ArduinoOTA.handle();

  //if (getNTP_Local_Time_Hour() == HORARIO_RIEGO) {
    if (!checkSensorDeposito()) {
      if (getEstadoBombaAgua())
        pararBombaAgua();
    } else {
      int porcHumedad = checkSensorHumedadSuelo();
      if (porcHumedad < PORC_HUMEDAD_SUELO_RIEGO ) {
        if (!getEstadoBombaAgua() && num_riegos_realizados < NUM_RIEGOS) {
          activarBombaAgua();
          num_riegos_realizados++;
        } else
          pararBombaAgua();
      }
    }
  //}
  //num_riegos_realizados = 0;
}
