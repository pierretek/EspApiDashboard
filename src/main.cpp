#include <Arduino.h>

//key library files
#include "WebserverHandler.h"
#include "TaskManager.h"
#include "DisplayHandler.h"

void setup() {

  Serial.begin(9600);

  //setting up the TFT display
  setupDisplay();

  //setting up wifi and webserver callbacks
  setupWebserver();

  //setting up the api tasks and the task manager
  setupApiTasks();

  //preload a set of testing api's
  preloadTestApis();
}

void loop() {
  // put your main code here, to run repeatedly:
}


