void setup_wifi() {
    //WiFi.softAPdisconnect();
    //WiFi.disconnect();
    //WiFi.mode(WIFI_STA);
    //delay(100);
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(wifi_ssid);

    WiFi.begin(wifi_ssid, wifi_password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void reconnect() {
  // Bucle hasta conseguir la reconexión
  while (!client.connected()) {
    // Intento de conexión
    Serial.print("Attempting MQTT connection...");

    // Si utilizas usuario y contraseña, cambia la siguiente línea por esta
    // if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      client.subscribe(pump_topic);
      client.subscribe(humidity_topic);
      client.subscribe(tank_topic);
      client.subscribe(led_empty_topic);
      client.subscribe(led_watering_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      // Espera 5 segundos para un nuevo intento
      delay(5000);
    }
  }
}
