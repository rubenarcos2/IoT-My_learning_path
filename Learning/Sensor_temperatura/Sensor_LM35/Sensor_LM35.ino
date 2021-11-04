/* Sketch de ejemplo para testear el sensor de temperatura analógico LM35
 Escrito por Regata para www.tallerarduino.wordpress.com
 
 MODO DE CONEXIÓN DEL SENSOR
 
 Conectamos el pin 1 que corresponde a la alimentación del sensor con los 5V del Arduino
 Conectamos el pin 2 que corresponde al pin de datos del sensor con cualquier pin analógico del Arduino A0
 Conectamos el pin 3 que corresponde al pin de masa (GND) del sensor con el pin GND del Arduino
 
 */
// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int temp = 0;  //Pin analógico A0 del Arduino donde conectaremos el pin de datos del sensor LM35
float maxC = 0, minC = 100;  //Variables para ir comprobando maximos y minimos

void setup()
{
  Serial.begin(9600);  //Iniciamos comunicación serie con el Arduino para ver los resultados del sensor
  //a través de la consola serie del IDE de Arduino
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop()
{
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  
  
  float gradosC;  //Declaramos estas variables tipo float para guardar los valores de lectura

  gradosC = (5.0 * analogRead(temp) * 100.0) / 1024;  //Esta es la funcion con la que obtenemos la medida del sensor
  // en ºC

  //Mostramos mensaje con valores actuales de temperatura, asi como maximos y minimos de cada uno de ellos
  Serial.print("####################\n#   Temp. actual   #\n");
  Serial.print("#     ");
  Serial.print(gradosC);
  lcd.print(gradosC);
  lcd.print(millis()/1000);s
  Serial.print(" C. "); 
  Serial.print("    #\n####################");
  //Comprobacion de maximos y minimos de temperatura
  if (maxC < gradosC)
    maxC = gradosC;
  if (gradosC < minC)
    minC = gradosC;
  Serial.print("\nTemp. max.: ");
  Serial.print(maxC);
  Serial.print(" C."); 

  Serial.print("\nTemp. min.: ");
  Serial.print(minC);
  Serial.print(" C. "); 
  Serial.print("\n\n");
  delay(2000);  //Usamos un retardo de 2 segundos entre lectura y lectura 
}

