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
    blink(1, 1000);
    WiFi.begin(ssid, password);
  }
  delay(1000);
}