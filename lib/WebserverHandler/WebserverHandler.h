#pragma once
//This libary handles hosting the webserver, and recieving http requests from it

#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

#include "ApiState.h"
#include "WebsiteFiles.h"
#include "DisplayHandler.h"
#include "TaskManager.h"

//Wifi details
#define SSID "YOUR_WIFI_NAME_HERE"
#define PASS "YOUR_WIFI_PASSWORD_HERE"

void setupWebserver();
void preloadTestApis();
void updateApiState(AsyncWebServerRequest* req, uint8_t* data, size_t len);
void uploadApiState();
void printDataToSerial();
