#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
//#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>

const char *ssid     = "";
const char *password = "";

WiFiUDP ntpUDP;

// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

void initNTPClient() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
}

DateTime getNTPHour() {

  timeClient.update();

  String FechaNTP; 
  FechaNTP = timeClient.getFormattedTime();
  
  int a, m, d, h, n, s;

  h = FechaNTP.substring(0, 2).toInt();

  n = FechaNTP.substring(3, 5).toInt();

  s = FechaNTP.substring(6, 8).toInt();

  DateTime dt = DateTime(a, m, d, h, n, s);
  
  return dt;
}
