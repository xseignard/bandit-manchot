#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#include <SPIFFS.h>

AsyncWebServer server(80);
StaticJsonDocument<256> settings;

void startServer() {
  SPIFFS.begin();
  // TODO: additional headers and 404 handler ?
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods",
                                       "GET, POST, PATCH, PUT, DELETE, OPTIONS");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Origin, Content-Type, X-Auth-Token");
  server.onNotFound([](AsyncWebServerRequest* request) {
    if (request->method() == HTTP_OPTIONS) {
      request->send(200);
    } else {
      request->send(404);
    }
  });
  // end of TODO
  server.on("/", HTTP_GET,
            [](AsyncWebServerRequest* request) { request->send(SPIFFS, "/index.html", "text/html"); });
  server.on("/favicon-16x16.png", HTTP_GET,
            [](AsyncWebServerRequest* request) { request->send(SPIFFS, "/favicon-16x16.png", "image/png"); });
  server.on("/fonts/MaterialIcons.woff2", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(SPIFFS, "/fonts/MaterialIcons.woff2", "font/woff2");
  });
  server.on("/js/app.js", HTTP_GET,
            [](AsyncWebServerRequest* request) { request->send(SPIFFS, "/js/app.js", "text/javascript"); });
  server.on("/css/app.css", HTTP_GET,
            [](AsyncWebServerRequest* request) { request->send(SPIFFS, "/css/app.css", "text/css"); });
  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest* request) {
    settings.clear();
    settings["ssid"] = getSSID();
    settings["password"] = getPassword();
    String res;
    serializeJson(settings, res);
    request->send(200, "application/json", res);
  });
  AsyncCallbackJsonWebHandler* handler =
      new AsyncCallbackJsonWebHandler("/update", [](AsyncWebServerRequest* request, JsonVariant& json) {
        JsonObject jsonObj = json.as<JsonObject>();
        setSSID(jsonObj["ssid"].as<String>());
        setPassword(jsonObj["password"].as<String>());
        request->send(201, "application/json", "{\"status\":\"ok\"}");
        delay(1000);
        ESP.restart();
      });
  server.addHandler(handler);
  server.begin();
}