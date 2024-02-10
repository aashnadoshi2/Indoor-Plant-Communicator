/*************************************************************
Plant Communicator Code
By: Aashna, Kruti, Rishi
*************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL2vgLwE_8c"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN"2Cej41EiNhIsTP8bYlyMw0E1rqiSfnzo"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_LTR390.h>
#include "time.h"
#include "esp_sntp.h"


// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Hotspot";
char pass[] = "TempPassword";


// PINS
const int capacitiveMoisturePin = 4;
const int motorOutput = 5;
const int ledStripPin = 6;
const int manualSwitchPin = 7;

// State Variables
bool ledState = false;
bool manualOverride = false;
int motorManualSwitch = 0;


Adafruit_LTR390 ltr = Adafruit_LTR390(); // Initialize UV


// Timer constants
const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 18000;
const int daylightOffset_sec = 3600;


BlynkTimer timer; // Initialize Timer

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl","https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");


  Serial.println("Connected");
}

// This function is called every time the Virtual Pin 0 state changes - MANUAL OVERRIDE SWITCH data from blynk
BLYNK_WRITE(V0)
{
  motorManualSwitch = param.asInt();
  Blynk.virtualWrite(V0, motorManualSwitch);
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);

  // Initialize Pins
  pinMode(motorOutput, OUTPUT); // initializing motor GPIO pin
  pinMode(manualSwitchPin, INPUT_PULLUP); // Assuming a pull-up switch configuration

  // Detect UV Sensor 
  if (!ltr.begin()) {
    Serial.println("Failed to find LTR390 chip");
    while (1) { delay(10); }
  }
  Serial.println("LTR390 Found!");
  ltr.setMode(LTR390_MODE_UVS);

  //Configure RTC Module
  configTime(-5 * 3600, 0, "pool.ntp.org", "time.nist.gov");
}

// RTC Helper Function
int getCurrentHour() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return -1;
  }
  return timeinfo.tm_hour; // returns the current hour
}

void loop()
{
  Blynk.run();
  timer.run();
  
  // ---------------- UV SENSING ------------------

  int uvIndex;
  int currentHour;

  /* 
  * UV data Info:
  * SDA = Pin 23
  * SCL = Pin 22
  * Not using INT
  */

  if(ltr.newDataAvailable()) {
    uvIndex = ltr.readUVS();
  }  
  Blynk.virtualWrite(V5, uvIndex);                   // Assuming V5 is the Virtual Pin for UV data

  if (uvIndex < 50) {
    currentHour = getCurrentHour();
    if(currentHour >= 6 && currentHour <= 18) {      // Check if time is between 6 AM and 6 PM
      Blynk.virtualWrite(V1, 1);                     // Write 1 to dashboard if both conditions are met
    } else {
      Blynk.virtualWrite(V1, 0);                     // Else, write 0 to dashboard
    }
  } else {
    Blynk.virtualWrite(V1, 0);                       // If UV index is not less than 50, write 0
  }

  Blynk.virtualWrite(V4, currentHour);

  Serial.println(currentHour);


  // --- CAPACITIVE MOISTURE SENSOR AND WATER PUMP AUTOMATION----
  
  int capacitiveMoistureLevel;

  /* 
  * Moisture Level Info: 
  * Approx 2500 for dry
  * Approx 1200-1300 for wet
  */

  capacitiveMoistureLevel = analogRead(capacitiveMoisturePin);      //connect sensor to Analog 0
  Blynk.virtualWrite(V6, 3000- capacitiveMoistureLevel);                  // write the capacitive moisture level to Blynk V6 virtual pin

  if((motorManualSwitch == 1) || (capacitiveMoistureLevel > 1700)){ // if motor override or level > 1500
    digitalWrite(motorOutput, HIGH);                                // turn motor on
  } else {
    digitalWrite(motorOutput, LOW);                                 // turn motor off
  }

}