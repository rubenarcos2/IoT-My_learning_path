int pinReleBombaAgua = -1;
bool estadoBombaAgua;
int pinLedRegando = -1;

void initReleBombaAgua(int pinRele) {
  pinReleBombaAgua = pinRele;
  if (pinReleBombaAgua != -1) {
    pinMode(pinReleBombaAgua, OUTPUT);     // Relé: 0 = Funcionando. 1 = Parado
    digitalWrite(pinReleBombaAgua, HIGH); //Bomba de agua parada
    digitalWrite(pinLedRegando, LOW);
    estadoBombaAgua = false;
    Serial.print("[INFO] Inicializado el relé de la bomba de agua. Conectado en Pin: ");
    Serial.println(pinReleBombaAgua);
  } else {
    Serial.println("[ERROR] al inicializar el relé de la bomba de agua. Conectado en Pin: ");
    Serial.println(pinReleBombaAgua);
  }
}

void initLedRegando(int pinLed) {
  pinLedRegando = pinLed;
  if (pinLedRegando != -1) {
    pinMode(pinLedRegando, OUTPUT); // LED indicador depósito vacío
    Serial.print("[INFO] Inicializado el led de regado. Conectado en Pin: ");
    Serial.println(pinLedRegando);
  } else {
    Serial.println("[ERROR] al inicializar el led de regado. Conectado en Pin: ");
    Serial.println(pinLedRegando);
  }
}

int getEstadoBombaAgua() {
  return estadoBombaAgua;
}

int activarBombaAgua() {
  if (pinReleBombaAgua != -1) {
    digitalWrite(pinReleBombaAgua, LOW); //Bomba de agua funcionando
    digitalWrite(pinLedRegando, HIGH);
    estadoBombaAgua = true;
    Serial.println("[OK]  Bomba de agua activada.");
    return true;
  } else {
    Serial.println("[ERROR] al activar la bomba de agua.");
    return -1;
  }
}

int pararBombaAgua() {
  if (pinReleBombaAgua != -1) {
    digitalWrite(pinReleBombaAgua, HIGH); //Bomba de agua parada, porque no hay agua en el deposito
    digitalWrite(pinLedRegando, LOW);
    estadoBombaAgua = false;
    Serial.println("[OK]  Bomba de agua parada.");
    return true;
  } else {
    Serial.println("[ERROR] al parar la bomba de agua.");
    return -1;
  }
}
