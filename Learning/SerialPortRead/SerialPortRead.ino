#include <SoftwareSerial.h>
SoftwareSerial ArduinoUno(3,2);

void setup(){
  
  Serial.begin(9600);
  ArduinoUno.begin(4800);
  Serial.println("Init");
  ArduinoUno.print("Init");
}

void loop(){
  int i = 333;
  while(ArduinoUno.available()>0){    
    String val = ArduinoUno.readString();
    Serial.println(val);
    
    delay(1000);    
    ArduinoUno.print(i);
  }
}
