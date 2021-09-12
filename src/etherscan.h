#include <HTTPClient.h>
#include <ArduinoJson.h>

HTTPClient client;
StaticJsonDocument<300> doc;
String response;

const char apiEndPoint[] = "https://api.etherscan.io/api?module=account&action=balance&tag=latest&apikey=8V6AEKGX62ZCADRARTMW7HY11DGTR7F61Q&address=";

void getBalance(char* address, char* result) {
  char url[sizeof(apiEndPoint) + 42];
  strcpy(url, apiEndPoint);
  strcat(url, address);
  client.begin(url);
  client.GET();
  response = client.getString();
  deserializeJson(doc, response);
  strcpy(result, doc["result"].as<char*>());
  client.end();
}
