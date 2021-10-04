#include <Preferences.h>

Preferences preferences;
String ssid;
String password;

void readNVS() {
  preferences.begin("bandit-manchot", false);
  ssid = preferences.getString("ssid", "");
  password = preferences.getString("password", "");
}

void setSSID(String newSSID) {
  preferences.putString("ssid", newSSID);
}

void setPassword(String newPassword) {
  preferences.putString("password", newPassword);
}

String getSSID() {
  return ssid;
}

String getPassword() {
  return password;
}