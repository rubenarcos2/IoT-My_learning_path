//Totalmente creado por Antonio López Rubia [LPZ] para YouTube.com/antoniolr95, Taringa y http://antoniolr95.blogspot.com
#define LED 16     //El LED esta conectado en el pin 9 
#define LDR 0     //El LDR esta conectador en el pin A0
int luz = 0;       
int valor_sensor = 0;       

void setup() {      //Configuracion de los pin como entrada o salida 
  Serial.begin(115200);    
  pinMode(LED,OUTPUT);
  pinMode(LDR,INPUT);
  digitalWrite(LED, HIGH);
}

void loop ()  //Configuracion de los valores del LDR
{
  valor_sensor = analogRead(LDR); 
  delay(10);
  luz = (5.0 * valor_sensor * 100.0)/1024.0; 
  Serial.print(valor_sensor);             
  delay(300);                       
  
  if (luz <= 290)   //Si la resistencia que produce el ldr es menor o igual que 490
  {
    digitalWrite (LED, LOW);  //El led se apaga
    Serial.println(" LED OFF"); 
  }
  if (luz > 290)   //Si es mayor que 490
  {
    digitalWrite (LED, HIGH);  //El led se eniende
    Serial.println(" LED ON");
  }
}
