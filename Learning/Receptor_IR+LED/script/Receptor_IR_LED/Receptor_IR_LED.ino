//incluimos la librería de IR
#include <IRremote.h>
#include <IRremoteInt.h>

int RECV_PIN = 2; //pin receptor IR
int pinLed=12; //pin LED

//Activa el receptor y decodifica los resultados
IRrecv irrecv(RECV_PIN);
decode_results results;

int ValorIR=0;
boolean estadoLed=0;


void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); //Activa el receptor IR
  pinMode(pinLed, OUTPUT); //pin del led como de salida
}


//Devuelve el valor del boton pulsado
int leerIR(){
  if (irrecv.decode(&results)) { // si recibe señal IR
    int lectura = results.value; //Guarda el valor de la señal en la variable lectura
    irrecv.resume(); // Recibe siguiente valor IR
    //Serial.println(lectura); //Imprime el codigo del boton por consola
    return lectura;
  }
}


void encenderLED(int num){
  Serial.print("BOTON ");
  
  if (num==0){
    Serial.println("ON/OFF");
    if (estadoLed==0){ //comprobamos si el led está encendido
      digitalWrite(pinLed,HIGH); //lo encendemos
      Serial.println("LED ON");
      estadoLed=1; //marcamos como que está encendido
    }
    else{ //si no está apgado es que esta endendico
      digitalWrite(pinLed,LOW); //lo apagamos
      Serial.println("LED OFF");
      estadoLed=0; //marcamos como apagado
    }
  }else{
    Serial.println(num);
    for (int i=0; i<num; i++) {
      digitalWrite(pinLed,HIGH); //lo encendemos
      Serial.println("LED ON");
      delay(200);
      digitalWrite(pinLed,LOW); //lo apagamos
      Serial.println("LED OFF");
      delay(200); 
    }
  }
  Serial.println("");
}


void loop() {
  ValorIR=leerIR();
  
  switch(ValorIR){
      //BONTON ON/OFF
    case -23971:
      encenderLED(0);
      break;
      //BOTON 1
    case 12495:
      encenderLED(1);
      break;
      //BOTON 2
    case 6375:
      encenderLED(2);
      break;
      //BOTON 3
    case 31365:
      encenderLED(3);;
      break;
      //BOTON 4
    case 4335:
      encenderLED(4);
      break;
      //BOTON 5
    case 14535:
      encenderLED(5);
      break;
      //BOTON 6
    case 23205:
      encenderLED(6);
      break;
      //BOTON 7
    case 17085:
      encenderLED(7);
      break;
      //BOTON 8
    case 19125:
      encenderLED(8);
      break;
      //BOTON 9
    case 21165:
      encenderLED(9);
      break;
  }
}





