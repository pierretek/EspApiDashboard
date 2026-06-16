#include "DisplayHandler.h"

TFT_eSPI tft{};

SemaphoreHandle_t displaySemaphore{};

//Initialise the Screen, and the required semaphore to avoid race conditions
void setupDisplay() {

	tft.init();
	tft.setRotation(2);
	tft.fillScreen(TFT_BLACK);
	tft.setTextSize(1);
	tft.setTextColor(TFT_WHITE);

	displaySemaphore = xSemaphoreCreateMutex();
}

void printToDisplay(int id, const char* value, int opCode) {
	if (id < 0 || id >= MAX_APIS) return;  // bounds check

	//gathering all the required variables for the display
	char data[64]{};
	char title[25]{};
	char suffix[20]{};
	strncpy(data, value, sizeof(data) - 1);
	strncpy(title, state[id].title, sizeof(title) - 1);
	strncpy(suffix, state[id].suffix, sizeof(suffix) - 1);

	uint16_t color = tft.color24to16(state[id].displayColor);

	//handling special cases
	switch (opCode) {
		case STARTUP:
			strncpy(title, "Wifi Connected!", sizeof(title) - 1);
			suffix[0] = '\0';
			color = TFT_GREEN;
			break;
		case WIFI_FAILED:
			strncpy(data, "Wifi failed", sizeof(data) - 1);
			suffix[0] = '\0';
			color = TFT_RED;
			break;
		case CONNECTION_FAILED:
			strncpy(data, "Err: bad api link", sizeof(data) - 1);
			suffix[0] = '\0';
			color = TFT_RED;
			break;
		case KEY_NOT_FOUND:
			strncpy(data, "JSON key not found", sizeof(data) - 1);
			suffix[0] = '\0';
			color = TFT_RED;
			break;
	}


	//drawing the boxes
	int sectionX{5};
	int sectionY{5 + 30 * id};
	tft.fillRoundRect(sectionX, sectionY, 118, 29, 5, TFT_BLACK);
	tft.drawRoundRect(sectionX, sectionY, 118, 29, 5, color);
	tft.setTextColor(color);

	//printing the text inside
	int16_t textWidth = tft.textWidth(title);
	tft.drawString(title, (tft.width() / 2 - textWidth / 2), sectionY + 5);

	char valueAndSuffix[64] = {};
	snprintf(valueAndSuffix, sizeof(valueAndSuffix), "%s %s", data, suffix);
	textWidth = tft.textWidth(valueAndSuffix);
	tft.drawString(valueAndSuffix, (tft.width() / 2 - textWidth / 2), sectionY + tft.fontHeight() * 2);
}

//helper function to clear the display
void clearDisplay() {
	tft.fillScreen(TFT_BLACK);
}
