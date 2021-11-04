#include <SoftwareSerial.h>
#include "ESP8266.h"

const char* SSID = "";
const char* PASSWORD = "";
const char* HOST_NAME = "www.pasted.co";
const int HOST_PORT = 80;
 
SoftwareSerial softSerial(2, 3); // RX, TX
ESP8266 wifi(softSerial);
 
void setup(void)
{
   Serial.begin(9600);
 
   if (wifi.setOprToStationSoftAP()) {
      Serial.print("to station + softap ok\r\n");
   }
   else {
      Serial.print("to station + softap err\r\n");
   }
 
   if (wifi.joinAP(SSID, PASSWORD)) {
      Serial.print("Join AP success\r\n");
      Serial.print("IP:");
      Serial.println(wifi.getLocalIP().c_str());
   }
   else {
      Serial.print("Join AP failure\r\n");
   }
 
   if (wifi.disableMUX()) {
      Serial.print("single ok\r\n");
   }
   else {
      Serial.print("single err\r\n");
   }
 
   Serial.print("setup end\r\n");
}
 
void loop(void)
{
   uint8_t buffer[800] = { 0 };
 
   if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
      Serial.print("create tcp ok\r\n");
   }
   else {
      Serial.print("create tcp err\r\n");
   }
 
   char *request = "GET /2434bc64 HTTP/1.1\r\nHost: www.pasted.co\r\nConnection: close\r\n\r\n";
   wifi.send((const uint8_t*)request, strlen(request));
 
   uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);
   if (len > 0) 
   {
      Serial.print("Received:\r\n");
      for (uint32_t i = 0; i < len; i++) 
      {
         char c = (char)buffer[i];
         if (c == '~')
         {
            for (uint32_t j = i + 1; j < len; j++)
            {
               c = (char)buffer[j];
               if (c == '~') break;
               Serial.print(c);
            }
            break;
         }
      }
      Serial.print("\r\n");
   }
 
   while (1) delay(1000);
}
