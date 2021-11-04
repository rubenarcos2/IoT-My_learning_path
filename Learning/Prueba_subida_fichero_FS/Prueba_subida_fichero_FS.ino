#include "FS.h"
 
void setup() {
  Serial.begin(115200);
  // Ejecutamos el inicio de SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("Ha ocurrido un error al ejecutar SPIFFS");
    return;
  }
  // abrimos el archivo prueba.txt en modo lectura
  File file = SPIFFS.open("/prueba.txt", "r");
  if(!file){
    Serial.println("Error al abrir el archivo en modo lectura");
    return;
  }
  
  Serial.println();
  Serial.println("Contenido del archivo:");

// Mostramos el contenido del archivo
  while(file.available()){
    Serial.write(file.read());
  }

// cerramos el archivo
  file.close();
}
 void loop() {
}
