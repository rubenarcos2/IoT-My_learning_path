Adafruit_PWMServoDriver servoController = Adafruit_PWMServoDriver(0x40);

const uint8_t frequency = 50;    // Frecuencia PWM de 50Hz o T=20ms
const uint16_t ServoMinTicks = 102; // ancho de pulso en ticks para pocicion 0°
const uint16_t ServoMaxTicks = 512; // ancho de pulso en ticks para la pocicion 180°

extern const int laser = 14;

void initApi() {
  servoController.begin();
  servoController.setPWMFreq(frequency);

  pinMode(laser, OUTPUT);
}

void setData(AsyncWebSocketClient * client, String request)
{
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, request);
  if (error) {
    return;
  }

  int idElem = doc["id"];
  int valueElem = doc["value"];

  if (idElem < 16) {
    Serial.print("Mover servo");
    Serial.print(idElem);
    Serial.print(" valor ");
    Serial.println(valueElem);
    servoController.setPWM(idElem, 0, valueElem);
  } else if (idElem = 16) {
    Serial.print("Encender laser " + valueElem);
    digitalWrite(laser, valueElem);
  }

}

void getData(AsyncWebSocketClient * client, String request)
{
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, request);
  if (error) {
    return;
  }

  String response;
  StaticJsonDocument<300> jsonDoc;
  int idElem = doc["id"];
  jsonDoc["id"] = idElem;
  jsonDoc["value"] = servoController.getPWM(idElem);
  serializeJson(jsonDoc, response);

  client->text(response);

}
