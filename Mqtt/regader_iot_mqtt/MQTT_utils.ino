String selector;

// Rutina para la recepción de mensajes MQTT. No usada para el sensor de temperatura y humedad
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("] ");
  String(receivedChar) = "";
  for (int i = 0; i < length; i++) {
    receivedChar += (char)payload[i];
  }
  Serial.println(receivedChar);
  String selector = String (topic);
  

  if (selector == "topicName/pump") {
    if (receivedChar == "OFF") {
      digitalWrite(PIN_RELE_BOMBA_AGUA, HIGH);//Lógica inversa
    }
    if (receivedChar == "ON") {
      digitalWrite(PIN_RELE_BOMBA_AGUA, LOW);
    }
  }

  if (selector == "topicName/empty") {
    if (receivedChar == "OFF") {
      digitalWrite(PIN_LED_DEPOSITO_VACIO, LOW);
    }
    if (receivedChar == "ON") {
      digitalWrite(PIN_LED_DEPOSITO_VACIO, HIGH);
    }
  }

  if (selector == "topicName/watering") {
    if (receivedChar == "OFF") {
      digitalWrite(PIN_LED_REGANDO, LOW);
    }
    if (receivedChar == "ON") {
      digitalWrite(PIN_LED_REGANDO, HIGH);
    }
  }
}
