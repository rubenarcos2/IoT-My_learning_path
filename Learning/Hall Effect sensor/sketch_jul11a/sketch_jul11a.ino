#define SENSOR 16 // define the Hall magnetic sensor

int val = 0;

void setup() {
  Serial.begin(9600);
  pinMode(SENSOR, INPUT) ;  // define the Hall magnetic sensor line as input
}

void loop() {
  val = 0;
  val = analogRead(SENSOR) ; // read sensor line
  Serial.print(val);
  
  if (val == LOW) // when the Hall sensor detects a magnetic field, Arduino LED lights up
  {
    Serial.println(" - Magnetic field detected ");  
  }else{
    Serial.println(" - No magnetic field detected ");
  }

  delay(100);
}
