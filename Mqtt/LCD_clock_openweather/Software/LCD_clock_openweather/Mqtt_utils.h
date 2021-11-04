#define wifi_ssid ""
#define wifi_password ""

#define mqtt_server "192.168.1.195"
#define mqtt_port 1883
//#define mqtt_user "light"
//#define mqtt_password "12345"     

#define humidity_topic "topicName/humidity"         //Cambia el nombre del topic a tu gusto
#define temperature_topic "topicName/temperature"
#define light_topic "topicName/light"

int numReconnectWifi = 50;
int numReconnectMqtt = 5;

WiFiClient espClient;
PubSubClient client;

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

    while (WiFi.status() != WL_CONNECTED && numReconnectWifi >0) {
        delay(500);
        Serial.print(".");
        numReconnectWifi--;
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void reconnect() {
    // Bucle hasta conseguir la reconexión
    while (!client.connected() && numReconnectMqtt > 0) {
        // Intento de conexión
        Serial.print("Attempting MQTT connection...");
        
        // Si utilizas usuario y contraseña, cambia la siguiente línea por esta
        // if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
        if (client.connect("ESP8266Client")) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            
            // Espera 5 segundos para un nuevo intento
            delay(5000);
            numReconnectMqtt--;
        }
    }
}

// Rutina para la recepción de mensajes MQTT. No usada para el sensor de temperatura y humedad
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
        char receivedChar = (char)payload[i];
        Serial.print(receivedChar);
        //if (receivedChar == '0')
        // digitalWrite(in_led, LOW);
        //if (receivedChar == '1')
        // digitalWrite(in_led, HIGH);
    }
    Serial.println();
}
