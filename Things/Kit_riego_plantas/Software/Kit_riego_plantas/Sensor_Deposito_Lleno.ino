int deposito = -1; // 0 = Vacío - 1 = Lleno
int pinSensorDeposito = -1;
int pinLedDepositoVacio = -1;

void initSensorDeposito(int pinSensor) {
  pinSensorDeposito = pinSensor;
  if (pinSensorDeposito != -1) {
    pinMode(pinSensorDeposito, INPUT_PULLUP); // Sensor deposito, en modo alto
    Serial.print("[INFO] Inicializado el sensor de depósito. Conectado en Pin: ");
    Serial.println(pinSensorDeposito);
  } else {
    Serial.println("[ERROR] al inicializar el sensor de depósito. Conectado en Pin: ");
    Serial.println(pinSensorDeposito);
  }
}

void initLedDepositoVacio(int pinLed) {
  pinLedDepositoVacio = pinLed;
  if (pinLedDepositoVacio != -1) {
    pinMode(pinLedDepositoVacio, OUTPUT); // LED indicador depósito vacío
    Serial.print("[INFO] Inicializado el led depósito vacío. Conectado en Pin: ");
    Serial.println(pinLedDepositoVacio);
  } else {
    Serial.println("[ERROR] al inicializar el led depósito vacío. Conectado en Pin: ");
    Serial.println(pinLedDepositoVacio);
  }
}

int getEstadoDeposito() {
  return deposito;
}

int checkSensorDeposito() {
  if (pinSensorDeposito != -1) {
    deposito = digitalRead(pinSensorDeposito);
    digitalWrite(pinLedDepositoVacio, !deposito);
    printEstadoDepo();
    return deposito;
  } else {
    return -1;
  }
}

void printEstadoDepo() {
  if (!deposito) {
    Serial.println("[INFO] Nivel de agua del depósito: VACÍO");
  } else {
    Serial.println("[INFO] Nivel de agua del depósito: LLENO");
  }
}
