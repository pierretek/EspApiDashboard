# ESP Api Dashboard &nbsp; <img src="https://github.com/user-attachments/assets/f0baf001-bbd4-4cb8-9915-fd82dad5cd95" width="40" height="40" align="top" />

A configurable ESP32 Api dashboard, where users can create, delete, and reorder up to five api endpoints on a TFT display
 
## Details 
- Main program is in `src/main.cpp`, project libraries are in `lib/`, the system state and website files are in `include/`
- This project uses a [128x160 ST7735 TFT Display](https://www.ebay.com/itm/191786659555?siteid=0&customid=&toolid=20012)
- The ESP32 hosts a webserver where the user controls which API endpoints are displayed on a TFT display
- Each API is given a dedicated FreeRTOS task to periodically update its data
- Check out the [demo page!](https://pierretek.github.io/EspApiDashboard/)


## Requirements 

### Hardware
- `ESP32 Devkit V1`
- [128x160 ST7735 TFT Display Module](https://www.ebay.com/itm/191786659555?siteid=0&customid=&toolid=20012)
- Breadboard and connector wires
### Software
- VS Code with the platformIO extension


## How to Use
1. Connect your ESP32 to the display module according to the diagram below
   
<p align="center">
  <img  height="300" alt="Image" src="https://github.com/user-attachments/assets/4ceb34f4-631c-40fd-82cf-f3fed8c41a20" />
</p>

1. Import the project folder in **VS Code** with **platformIO** (platformio.ini must be in root)
2. Navigate to `lib/WebserverHandler/WebserverHandler.h` and fill in your WIFI details (lines 14-15)
3. Connect your ESP32 via USB and upload the project
4. Open the serial monitor and connect to the IP mentioned using your browser
   
<p align="center">
  <img height="30" alt="Image" src="https://github.com/user-attachments/assets/9eeb037b-3998-4c3a-b172-cca2382de419" />
</p>

5. Use the web interface to customise the API layout however you like


## Demo

#### Displaying three unique API's

<img height="300" alt="Image" src="https://github.com/user-attachments/assets/e67a2c0c-a62f-44d2-b849-c5edeb4e001d" />

#### Displaying five unique API's

<img  height="300" alt="Image" src="https://github.com/user-attachments/assets/c03f8d62-4f30-4ca8-bff5-f6d1bed2f4b9" />

#### The web interface
<img  height="300" alt="Image" src="https://github.com/user-attachments/assets/755624c1-6f6e-450d-aab8-43bc144dc5cd" />


## Future
- Im planning on making the api configuration save to an SD card using the onboard SD slot on the display module.
  - This will prevent having to retype all the api information everytime the ESP32 is turned off

---
_thanks for reading_


