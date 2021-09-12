#include <WiFi.h>

const char* ssid = "SFR_59E8";
const char* password = "coucouplouplou";

void wifiReset(wifi_mode_t mode) {
  WiFi.mode(mode);
  delay(2000);
}

void startWifi() {
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  wifiReset(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED) {
    wifiReset(WIFI_STA);
    Serial.print(".");
    WiFi.begin(ssid, password);
  }
  Serial.println();
  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());
  delay(1000);
}