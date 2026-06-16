#pragma once
//This header handles the api tasks, and the overall task manager

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <ArduinoJson.h>

#include "ApiState.h"
#include "DisplayHandler.h"

#define MAX_APIS 5

extern TaskHandle_t taskManagerHandle;

void setupApiTasks();
int parseJson(const char* link, const char* key, char* val);

//The actual RTOS tasks
void taskManager(void* parameter);
void api1(void* parameter);
void api2(void* parameter);
void api3(void* parameter);
void api4(void* parameter);
void api5(void* parameter);


