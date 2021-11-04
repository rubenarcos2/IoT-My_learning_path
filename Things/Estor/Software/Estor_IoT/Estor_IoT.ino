//definicion de pins
const int btnUpPin = 14;
const int btnDownPin = 12;
const int ledUpPin = 13;
const int ledDownPin = 15;
const int motorPinIN1 = 5;
const int motorPinIN2 = 4;
const int motorPinIN3 = 0;
const int motorPinIN4 = 2;

//definicion variables
int motorSpeed = 120;   //variable para fijar la velocidad
int stepCounter = 0;     // contador para los pasos
int stepsPerPulse = 1;  // pasos para una vuelta completa

//tablas con la secuencia de encendido (descomentar la que necesiteis)
//secuencia 1-fase
const int numSteps = 4;
const int stepsLookup[4] = { B1000, B0100, B0010, B0001 };

//secuencia 2-fases
//const int numSteps = 4;
//const int stepsLookup[4] = { B1100, B0110, B0011, B1001 };

//secuencia media fase
//const int numSteps = 8;
//const int stepsLookup[8] = { B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001 };


void setup()
{
  Serial.begin(115200);

  pinMode(btnUpPin, INPUT_PULLUP);
  pinMode(btnDownPin, INPUT_PULLUP);
  pinMode(ledUpPin, OUTPUT);
  pinMode(ledDownPin, OUTPUT);
  pinMode(motorPinIN1, OUTPUT);
  pinMode(motorPinIN2, OUTPUT);
  pinMode(motorPinIN3, OUTPUT);
  pinMode(motorPinIN4, OUTPUT);

  digitalWrite(ledUpPin,    LOW);
  digitalWrite(ledDownPin,  LOW);
  Serial.println("Iniciado...");
}

void loop()
{
  int btnUpState = digitalRead(btnUpPin);
  int btnDownState = digitalRead(btnDownPin);

  if (btnUpState == LOW) {
    Serial.println("Botón Arriba pulsado");
    digitalWrite(ledUpPin, HIGH);
    for (int i = 0; i < stepsPerPulse; i++)
    {
      clockwise();
      delayMicroseconds(motorSpeed);
    }
  } else if (btnDownState == LOW) {
    Serial.println("Botón Abajo pulsado");
    digitalWrite(ledDownPin, HIGH);
    for (int i = 0; i < stepsPerPulse; i++)
    {
      anticlockwise();
      delayMicroseconds(motorSpeed);
    }
  } else {
    stopMotor();
  }

}

void clockwise()
{
  stepCounter++;
  if (stepCounter >= numSteps) stepCounter = 0;
  moveMotor(stepCounter);
}

void anticlockwise()
{
  stepCounter--;
  if (stepCounter < 0) stepCounter = numSteps - 1;
  moveMotor(stepCounter);
}

void moveMotor(int step)
{
  digitalWrite(motorPinIN1, bitRead(stepsLookup[step], 0));
  digitalWrite(motorPinIN2, bitRead(stepsLookup[step], 1));
  digitalWrite(motorPinIN3, bitRead(stepsLookup[step], 2));
  digitalWrite(motorPinIN4, bitRead(stepsLookup[step], 3));
}

void stopMotor() {
  digitalWrite(ledUpPin,    LOW);
  digitalWrite(ledDownPin,  LOW);
  digitalWrite(motorPinIN1, LOW);
  digitalWrite(motorPinIN2, LOW);
  digitalWrite(motorPinIN3, LOW);
  digitalWrite(motorPinIN4, LOW);
}
