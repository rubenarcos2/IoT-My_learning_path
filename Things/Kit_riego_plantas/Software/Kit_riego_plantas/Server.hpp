AsyncWebServer server(80);

void InitServer()
{
  if (WiFi.status() == WL_CONNECTED) //comprobar estado de conexion del WIFI
  {
    SPIFFS.begin();

    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

    server.onNotFound([](AsyncWebServerRequest * request) {
      request->send(400, "text/plain", "Not found");
    });

    server.begin();
    Serial.println("[INFO] HTTP server started");
  }

}
