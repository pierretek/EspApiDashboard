#include "TaskManager.h"

TaskHandle_t apiTaskHandles[MAX_APIS]{};
TaskHandle_t taskManagerHandle{};

//Helper function to create the required RTOS tasks
void setupApiTasks() {

	xTaskCreate(api1, "Api1", 8000, NULL, 1, &apiTaskHandles[0]);
	xTaskCreate(api2, "Api2", 8000, NULL, 1, &apiTaskHandles[1]);
	xTaskCreate(api3, "Api3", 8000, NULL, 1, &apiTaskHandles[2]);
	xTaskCreate(api4, "Api4", 8000, NULL, 1, &apiTaskHandles[3]);
	xTaskCreate(api5, "Api5", 8000, NULL, 1, &apiTaskHandles[4]);

	xTaskCreate(
		taskManager,
		"Manage Api Tasks",
		4000,
		NULL,
		2,
		&taskManagerHandle
	);
}

//This function suspends and resumes the api tasks depending on if theyre being used
void taskManager(void* parameter) {

	bool update{};
	while (true) {

		//Only activate when the webserver recives an updates
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		for (uint16_t i{0}; i < MAX_APIS; i++) {

			//If api is inactive, suspend the task
			if (state[i].apiLink[0] == '\0' ||
				state[i].jsonKey[0] == '\0' ||
				state[i].title[0] == '\0') {

				vTaskSuspend(apiTaskHandles[i]);

			} else {

				//Otherwise run it, if it wasnt already
				vTaskResume(apiTaskHandles[i]);
			}



		}


	}
}

//Creating the five unique api tasks that will periodically retrieve and parse data from the provided links
void api1(void* parameter) {

	vTaskSuspend(NULL);
	char val[40];
	const int id{0};

	while (true) {

		int resultCode = parseJson(state[id].apiLink, state[id].jsonKey, val);

		if (xSemaphoreTake(displaySemaphore, 500 / portTICK_PERIOD_MS) == pdTRUE) {
			printToDisplay(id, val, resultCode);
			xSemaphoreGive(displaySemaphore);
		}

		uint32_t cooldown = state[id].refreshCooldown > 0 ? state[id].refreshCooldown : 1000;
		vTaskDelay(cooldown / portTICK_PERIOD_MS);

	}

}
void api2(void* parameter) {
	vTaskSuspend(NULL);
	char val[40];
	const int id{1};

	while (true) {

		int resultCode = parseJson(state[id].apiLink, state[id].jsonKey, val);

		if (xSemaphoreTake(displaySemaphore, 500 / portTICK_PERIOD_MS) == pdTRUE) {
			printToDisplay(id, val, resultCode);
			xSemaphoreGive(displaySemaphore);
		}


		uint32_t cooldown = state[id].refreshCooldown > 0 ? state[id].refreshCooldown : 1000;
		vTaskDelay(cooldown / portTICK_PERIOD_MS);

	}

}
void api3(void* parameter) {
	vTaskSuspend(NULL);
	char val[40];
	const int id{2};

	while (true) {

		int resultCode = parseJson(state[id].apiLink, state[id].jsonKey, val);

		if (xSemaphoreTake(displaySemaphore, 500 / portTICK_PERIOD_MS) == pdTRUE) {
			printToDisplay(id, val, resultCode);
			xSemaphoreGive(displaySemaphore);
		}


		uint32_t cooldown = state[id].refreshCooldown > 0 ? state[id].refreshCooldown : 1000;
		vTaskDelay(cooldown / portTICK_PERIOD_MS);

	}

}
void api4(void* parameter) {
	vTaskSuspend(NULL);
	char val[40];
	const int id{3};

	while (true) {

		int resultCode = parseJson(state[id].apiLink, state[id].jsonKey, val);

		if (xSemaphoreTake(displaySemaphore, 500 / portTICK_PERIOD_MS) == pdTRUE) {
			printToDisplay(id, val, resultCode);
			xSemaphoreGive(displaySemaphore);
		}


		uint32_t cooldown = state[id].refreshCooldown > 0 ? state[id].refreshCooldown : 1000;
		vTaskDelay(cooldown / portTICK_PERIOD_MS);

	}

}
void api5(void* parameter) {
	vTaskSuspend(NULL);
	// Serial.println("You should NOT be seing this");
	char val[40];
	const int id{4};

	while (true) {

		int resultCode = parseJson(state[id].apiLink, state[id].jsonKey, val);

		if (xSemaphoreTake(displaySemaphore, 500 / portTICK_PERIOD_MS) == pdTRUE) {
			printToDisplay(id, val, resultCode);
			xSemaphoreGive(displaySemaphore);
		}

		uint32_t cooldown = state[id].refreshCooldown > 0 ? state[id].refreshCooldown : 1000;
		vTaskDelay(cooldown / portTICK_PERIOD_MS);

	}

}

//Helper function to parse an API JSON value into a provided `val` parameter
int parseJson(const char* link, const char* key, char* val) {
	if (link[0] == '\0' || key[0] == '\0') return CONNECTION_FAILED;
	if (WiFi.status() != WL_CONNECTED) return WIFI_FAILED;

	int status{PARSE_SUCCESS};

	WiFiClientSecure client; //this saves memory apparently
	client.setInsecure();

	//Beginning the HTTP connection
	HTTPClient http;
	if (!http.begin(link)) return CONNECTION_FAILED;

	if (http.GET() <= 0) {
		Serial.println("REQUEST FAILED");
		status = CONNECTION_FAILED;

	} else {
		String rawData = http.getString();

		//Parsing the json
		JsonDocument doc;
		deserializeJson(doc, rawData);

		//splitting key on every '.'
		JsonVariant jsonNode = doc.as<JsonVariant>();
		char keyBuf[50];
		strncpy(keyBuf, key, sizeof(keyBuf));

		char* token = strtok(keyBuf, ".");

		//finding the actual node
		while (token != NULL) {
			jsonNode = jsonNode[token];
			token = strtok(NULL, ".");
		}

		//coping the node value into 'val'
		if (jsonNode.isNull()) {
			status = KEY_NOT_FOUND;
		} else {
			String data = jsonNode.as<String>();
			strcpy(val, data.c_str());
		}

	}

	http.end(); //IMPORTANT TO SAVE RESOURCES
	return status;

}
