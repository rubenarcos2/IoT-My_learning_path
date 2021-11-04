#include "Wifi_Config.h"  // Configuración Wifi

void ConnectWiFi_STA(bool useStaticIP = false)
{
  Serial.println("");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (useStaticIP) WiFi.config(ip, dns, gateway, subnet);
  WiFi.hostname(hostname);
  Serial.print("[INFO] Conectando a la red Wifi: ");
  while (WiFi.status() != WL_CONNECTED)
  {    
    delay(100);
    Serial.print('.');
  }

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("[ERROR] No se ha podido realizar la conexión a la red Wifi. Reiniciando...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("");
  Serial.print("[INFO] Iniciado en modo STA:\t");
  Serial.println(ssid);
  Serial.print("[INFO] Dirección IP:\t");
  Serial.println(WiFi.localIP());
}

void ConnectWiFi_AP(bool useStaticIP = false)
{
  Serial.println("");
  WiFi.mode(WIFI_AP);
  WiFi.begin(ssid_ap, password_ap);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("[INFO] Conectando a la red Wifi: ");
    delay(100);
    Serial.print('.');
  }

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("[ERROR] No se ha podido realizar la conexión a la red Wifi. Reiniciando...");
    delay(5000);
    ESP.restart();
  }
  if (useStaticIP) WiFi.softAPConfig(ip_ap, gateway_ap, subnet);
  WiFi.hostname(hostname_ap);
  Serial.println("");
  Serial.print("[INFO] Iniciado en modo AP:\t");
  Serial.println(ssid_ap);
  Serial.print("[INFO] Dirección IP:\t");
  Serial.println(WiFi.softAPIP());
}
