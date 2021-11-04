const int NUM_MEDICIONES = 250;
const int SENSOR_HUMEDAD_SECO = 800;   //Valor del sensor capacitativo totalmente seco
const int SENSOR_HUMEDAD_MOJADO = 500;  //Valor del sensor capacitativo sumergido en agua
const int MARGEN_TOLERANCIA = 120;
int pinSensorHumedadSuelo = -1;
int humedadSueloValor = 0;
int humedadSueloPorc = 0;
unsigned int medicHumSuel[NUM_MEDICIONES];

void initSensorHumedadSuelo(int pinSensor) {
  if (pinReleBombaAgua != -1) {
    pinSensorHumedadSuelo = pinSensor;
    pinMode(pinSensorHumedadSuelo, INPUT); // Sensor capacitativo de humedad en tierra
    Serial.print("[INFO] Inicializado el sensor de humedad suelo. Conectado en Pin: ");
    Serial.println(pinSensorHumedadSuelo);
  } else {
    Serial.println("[ERROR] al inicializar el sensor de humedad suelo. Conectado en Pin: ");
    Serial.println(pinSensorHumedadSuelo);
  }

  for (int i = 0; i < NUM_MEDICIONES; i++) {
    medicHumSuel[NUM_MEDICIONES] = 0;
  }
}

int checkSensorHumedadSuelo() {
  if (pinSensorHumedadSuelo != -1) {
    Serial.print("[INFO] Midiendo humedad suelo...");
    for (int i = 0; i < NUM_MEDICIONES; i++) {
      int medic = -1;
      medic = analogRead(pinSensorHumedadSuelo);
/*    Error en medicion, fuera de rango
      while (!(medic <= SENSOR_HUMEDAD_SECO + MARGEN_TOLERANCIA && medic >= SENSOR_HUMEDAD_MOJADO - MARGEN_TOLERANCIA)) {
        delay(10);
        medic = analogRead(pinSensorHumedadSuelo);        
          Serial.print("[ERROR] Medición ");
          Serial.print(i);
          Serial.print(" sensor humedad suelo. ");
          Serial.print(medic);
          printSensorHumedadSueloPorc(medic);
          Serial.println();
        
      }
*/
      medicHumSuel[i] = medic;
      delay(10);
    }

    //printSenHumSueloUltMediciones();
    printSensorHumedadSueloPorc(getSenHumSueloValMediaMedicns());
    Serial.println();
    return getSenHumSueloPorc();

  } else {
    Serial.println("[ERROR] Midiendo humedad suelo.");
    return -1;
  }
}

int getSenHumSueloValMediaMedicns() {
  int sumMedicHumSuel = 0;

  for (int i = 0; i < NUM_MEDICIONES; i++) {
    sumMedicHumSuel += medicHumSuel[i];
  }
  return sumMedicHumSuel / NUM_MEDICIONES;
}

int getSenHumSueloPorc() {
  return map(getSenHumSueloValMediaMedicns(), SENSOR_HUMEDAD_SECO, SENSOR_HUMEDAD_MOJADO, 0, 100);
}

int getSenHumSueloPorc(int valMedicion) {
  return map(valMedicion, SENSOR_HUMEDAD_SECO, SENSOR_HUMEDAD_MOJADO, 0, 100);
}

void printSenHumSueloUltMedicion(int numMedic, int medic) {
  Serial.print("[INFO] Medición ");
  Serial.print(numMedic);
  Serial.print(" humedad suelo: ");
  Serial.print(medic);
  printSensorHumedadSueloPorc(medic);
  Serial.println();
}

void printSenHumSueloUltMediciones() {
  for (int i = 0; i < NUM_MEDICIONES; i++) {
    printSenHumSueloUltMedicion(i, medicHumSuel[i]);
  }
  Serial.print("[INFO] Medición media humedad suelo: ");
  Serial.print(getSenHumSueloValMediaMedicns());
  printSensorHumedadSueloPorc(getSenHumSueloValMediaMedicns());
  Serial.println();
}

void printSensorHumedadSueloPorc(int medic) {
  if (getSenHumSueloPorc(medic) > 100)
  {
    Serial.print(" (100%)");
  }
  else if (getSenHumSueloPorc(medic) < 0)
  {
    Serial.print(" (0 %)");
  }
  else if (getSenHumSueloPorc(medic) > 0 && getSenHumSueloPorc(medic) < 100)
  {
    Serial.print(" (");
    Serial.print(getSenHumSueloPorc(medic));
    Serial.print("%)");
  }
}
