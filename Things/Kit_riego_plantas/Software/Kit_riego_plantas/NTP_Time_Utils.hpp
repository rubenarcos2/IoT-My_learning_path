#include <ezTime.h>

Timezone Madrid;

void initNTP_Time() {
  Madrid.setLocation("Europe/Madrid");

  if (WiFi.status() == WL_CONNECTED) //comprobar estado de conexion del WIFI
  {
    waitForSync();
    Serial.println("[INFO] Sincronización NTP activa. Hora actual: " + UTC.dateTime("d/m/Y H:i:s.v T"));
  }
}

String getNTP_UTC_Time() {
  if (WiFi.status() == WL_CONNECTED) { //comprobar estado de conexion del WIFI
    waitForSync();
    return UTC.dateTime();
  } else
    return "null";
}


String getNTP_Local_Time() {
  if (WiFi.status() == WL_CONNECTED) { //comprobar estado de conexion del WIFI
    waitForSync();
    return Madrid.dateTime("d/m/Y H:i:s");
  } else
    return "";
}

String getNTP_Local_Time_Hour() {
  if (WiFi.status() == WL_CONNECTED) { //comprobar estado de conexion del WIFI
    waitForSync();
    return Madrid.dateTime("H:i");
  } else
    return "";
}

String getNTP_Local_Time_To_RTC() {
  String fechaHoraNTP;
  if (WiFi.status() == WL_CONNECTED) { //comprobar estado de conexion del WIFI
    waitForSync();
    fechaHoraNTP = getNTP_Local_Time();
    int anio = fechaHoraNTP.substring(7, 10).toInt();
    int mes = fechaHoraNTP.substring(4, 5).toInt();
    int dia = fechaHoraNTP.substring(1, 2).toInt();
    int hora = fechaHoraNTP.substring(13, 14).toInt();
    int minutos = fechaHoraNTP.substring(16, 17).toInt();
    int seg = fechaHoraNTP.substring(19, 20).toInt();
  }
  return fechaHoraNTP;
}

String getNTP_LocalFormatedHour(){
  String fechaHoraNTP = getNTP_Local_Time();
  int anio = fechaHoraNTP.substring(7, 10).toInt();
  int mes = fechaHoraNTP.substring(4, 5).toInt();
  int dia = fechaHoraNTP.substring(1, 2).toInt();
  int hora = fechaHoraNTP.substring(13, 14).toInt();
  int minutos = fechaHoraNTP.substring(16, 17).toInt();
  int seg = fechaHoraNTP.substring(19, 20).toInt();
  return fechaHoraNTP;
}

void printNTP_Clock() {
  Serial.print("[INFO] NTP hora actual: ");
  Serial.println(getNTP_Local_Time());
}
