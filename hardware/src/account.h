#include <HTTPClient.h>
#include <ArduinoJson.h>

HTTPClient client;
StaticJsonDocument<512> doc;
String response;
DeserializationError error;

const char apiEndPoint[] = "https://bandit-manchot.cleverapps.io/";

bool getAccount(char* privateKey, char* publicKey, char* address, char* balance, bool* bingo) {
  int calls = 0;
  client.begin(apiEndPoint);
  client.GET();
  response = client.getString();
  client.end();
  error = deserializeJson(doc, response);
  calls++;
  while (error && calls < 5) {
    client.begin(apiEndPoint);
    client.GET();
    response = client.getString();
    client.end();
    error = deserializeJson(doc, response);
    calls++;
  }
  if (calls == 5) return true;
  else {
    strcpy(privateKey, doc["privateKey"].as<const char*>());
    strcpy(publicKey, doc["publicKey"].as<const char*>());
    strcpy(address, doc["address"].as<const char*>());
    strcpy(balance, doc["balance"].as<const char*>());
    strcat(balance, " ETH");
    *bingo = doc["bingo"].as<bool>();
    return false;
  }
}