void initOTA() {
  if (WiFi.status() == WL_CONNECTED) //comprobar estado de conexion del WIFI
  {
    // Port defaults to 8266
    //ArduinoOTA.setPort(8266);

    // Hostname defaults to esp8266-[ChipID]
    ArduinoOTA.setHostname("IOT_BRAZOROBOTICOMK2");

    // No authentication by default
    //ArduinoOTA.setPassword("123456");

    // Password can be set with it's md5 value as well
    //MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    //ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    ArduinoOTA.onStart([]() {
      String type;

      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else { // U_FS
        type = "filesystem";
      }

      // NOTE: if updating FS this would be the place to unmount FS using FS.end()
      Serial.println("[INFO] OTA Start updating " + type);
      digitalWrite(LED_BUILTIN, HIGH);
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\n[INFO] OTA update end");
      digitalWrite(LED_BUILTIN, LOW);
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      digitalWrite(LED_BUILTIN, LOW);
      Serial.printf("[INFO] OTA Progress: %u%%\r", (progress / (total / 100)));
      digitalWrite(LED_BUILTIN, HIGH);
    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("[ERROR] OTA Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("[ERROR] OTA Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("[ERROR] OTA Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("[ERROR] OTA Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("[ERROR] OTA Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("[ERROR] OTA End Failed");
      }
    });
    ArduinoOTA.begin();
    Serial.println("[INFO] Servidor de actualizaciones OTA iniciado");
  }

}
