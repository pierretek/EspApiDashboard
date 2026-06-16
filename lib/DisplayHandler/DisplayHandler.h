#pragma once
//This libary handles printing information from the API endpoints to the TFT display

#include <TFT_eSPI.h>
#include "ApiState.h"

//Status code enum used in printToDisplay()
enum {
	WIFI_FAILED, //wifi failed to connect
	CONNECTION_FAILED, //server failed to connect
	KEY_NOT_FOUND, //json key not found
	STARTUP, //used for display ip on startup
	PARSE_SUCCESS, //perfect

};

extern SemaphoreHandle_t displaySemaphore;

void setupDisplay();
void printToDisplay(int id, const char* value, int opCode);
void clearDisplay();
