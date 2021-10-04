#include <Wifi.h>
#include <DNSServer.h>

#define MAX_ATTEMPTS 30

IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;

void wifiReset(wifi_mode_t mode) {
  WiFi.mode(mode);
  delay(2000);
}

void startHotspot() {
  wifiReset(WIFI_AP);
  WiFi.softAP("bandit-manchot");
  delay(2000); // see: https://github.com/espressif/arduino-esp32/issues/2025
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  dnsServer.start(53, "*", apIP);
}

void startWifi() {
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  if (getSSID() != "") {
    uint8_t attempts = 0;
    wifiReset(WIFI_STA);
    while (WiFi.status() != WL_CONNECTED && attempts < MAX_ATTEMPTS) {
      wifiReset(WIFI_STA);
      blink(1, 1000);
      if (getPassword() != "") {
        WiFi.begin(getSSID().c_str(), getPassword().c_str());
      } else {
        WiFi.begin(getSSID().c_str());
      }
      attempts++;
    }
    if (attempts >= MAX_ATTEMPTS) startHotspot();
  } else {
    startHotspot();
  }
}