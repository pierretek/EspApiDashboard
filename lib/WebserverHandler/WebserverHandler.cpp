#include "WebserverHandler.h"

AsyncWebServer server(80);

IPAddress gateway(192, 168, 68, 1); //Change this
IPAddress subnet(255, 255, 255, 0); //Change this
IPAddress dns(192, 168, 68, 1); //Change this, usually same as gateway

IPAddress localIP(192, 168, 68, 20);

void preloadTestApis() {

	//Github status
	strcpy(state[0].apiLink, "https://isitdownstatus.com/api/v1/status/github");
	strcpy(state[0].title, "Github Status");
	strcpy(state[0].jsonKey, "data.status");
	state[0].refreshCooldown = 2000;
	state[0].displayColor = 0x27EEF5;

	//Temperature (not my actual coords lol)
	strcpy(state[1].apiLink, "http://api.open-meteo.com/v1/forecast?latitude=52.52&longitude=13.41&current=temperature_2m,precipitation");
	strcpy(state[1].title, "Temperature");
	strcpy(state[1].jsonKey, "current.temperature_2m");
	strcpy(state[1].suffix, "C");
	state[1].refreshCooldown = 60000;
	state[1].displayColor = 0x7DF527;

	//Precipitation 
	strcpy(state[2].apiLink, "http://api.open-meteo.com/v1/forecast?latitude=52.52&longitude=13.41&current=temperature_2m,precipitation");
	strcpy(state[2].title, "Precipitation");
	strcpy(state[2].jsonKey, "current.precipitation");
	strcpy(state[2].suffix, "%");
	state[2].refreshCooldown = 60000;
	state[2].displayColor = 0x27ADF5;

	// //Currency
	strcpy(state[3].apiLink, "http://api.exchangerate.fun/latest?base=CAD");
	strcpy(state[3].title, "CAD to EGP");
	strcpy(state[3].jsonKey, "rates.EGP");
	// strcpy(state[3].suffix, "%");
	state[3].refreshCooldown = 360000;
	state[3].displayColor = 0xEB27F5;


	//number of people in space
	strcpy(state[4].apiLink, "http://api.open-notify.org/astros");
	strcpy(state[4].title, "People in space");
	strcpy(state[4].jsonKey, "number");
	strcpy(state[4].suffix, "people");
	state[4].refreshCooldown = 360000;
	state[4].displayColor = 0xF5EE27;

	xTaskNotifyGive(taskManagerHandle);
}


//Helper function to setup the wifi, and create the webserver
void setupWebserver() {

	//initializing the wifi
	WiFi.config(localIP, gateway, subnet, dns);

	WiFi.begin(SSID, PASS);
	Serial.printf("Connecting to %s", SSID);

	while (WiFi.status() != WL_CONNECTED) {
		Serial.print('.');

		vTaskDelay(500 / portTICK_PERIOD_MS);
	}

	Serial.print("\nWIFI CONNECTED! IP: ");
	Serial.println(WiFi.localIP());

	//printing ip to display
	if (xSemaphoreTake(displaySemaphore, 500 / portTICK_PERIOD_MS) == pdTRUE) {
		printToDisplay(0, WiFi.localIP().toString().c_str(), STARTUP);
		xSemaphoreGive(displaySemaphore);
	}

	//preparing the web server
	server.on("/", HTTP_GET, [](AsyncWebServerRequest* req) {
		req->send(200, "text/html", INDEX_HTML);
		});

	server.on("/update", HTTP_PUT,
		[](AsyncWebServerRequest* req) {}, nullptr,
		[](AsyncWebServerRequest* req, uint8_t* data, size_t len, size_t index, size_t total) {
			updateApiState(req, data, len);
		}
	);

	server.begin();

}

//parses request from web server into Api State variable in "ApiState.h"
void updateApiState(AsyncWebServerRequest* req, uint8_t* data, size_t len) {

	//clearing the display on the first use
	// static bool firstTime{true};
	// if (firstTime) {
	// 	clearDisplay();
	// 	firstTime = !firstTime;
	// }

	JsonDocument doc;
	deserializeJson(doc, (const char*)data, len); // cast uint8_t* to const char*
	JsonArray apis = doc.as<JsonArray>();

	for (uint8_t i = 0; i < apis.size() && i < MAX_APIS; i++) {
		state[i].refreshCooldown = apis[i]["cooldown"] | 0;
		strlcpy(state[i].title, apis[i]["title"] | "", sizeof(state[i].title));
		strlcpy(state[i].apiLink, apis[i]["link"] | "", sizeof(state[i].apiLink));
		strlcpy(state[i].jsonKey, apis[i]["jsonKey"] | "", sizeof(state[i].jsonKey));
		strlcpy(state[i].suffix, apis[i]["suffix"] | "", sizeof(state[i].suffix));
		const char* color = apis[i]["color"] | "#ffffff";
		state[i].displayColor = strtoul(color + 1, NULL, 16);
	}

	req->send(200, "application/json", "{\"status\":\"ok\"}");

	printDataToSerial();

	//tell task manager to wake up
	xTaskNotifyGive(taskManagerHandle);
}

void uploadApiState() {}

void printDataToSerial() {

	for (int i = 0; i < MAX_APIS; i++) {
		Serial.printf("--- API %d ---\nOrder Number: %d\nCooldown: %d\nTitle: %s\nApi Link: %s\nJSON Key: %s\nSuffix: %s\nColor: %06X\n\n",
			i,
			state[i].refreshCooldown,
			state[i].title,
			state[i].apiLink,
			state[i].jsonKey,
			state[i].suffix,
			state[i].displayColor
		);
	}

}


