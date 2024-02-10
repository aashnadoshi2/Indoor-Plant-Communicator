/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL2vgLwE_8c"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN"2Cej41EiNhIsTP8bYlyMw0E1rqiSfnzo"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_LTR390.h>
#include "time.h"
#include "esp_sntp.h"

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Kruti";
char pass[] = "Krish1409";

// PINS
const int capacitiveMoisturePin = 4;
const int motorOutput = 5;
const int ledStripPin = 6;
const int manualSwitchPin = 7;

// State Variables
bool ledState = false;
bool manualOverride = false;
int motorManualSwitch = 0;

Adafruit_LTR390 uvSensor; // UV Sensor instance


// Timer constants
const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 18000;
const int daylightOffset_sec = 3600;

/*
// Not used right now but we can use to average 10 values
int get_average_moisture() {  
  int tempValue = 0; // variable to temporarily store moisture value
  // make an average of 10 values to be more accurate 
  for (int a = 0; a < 10; a++) {  
    tempValue += analogRead(moisturePin);  
    delay(100);  
  }  
  return tempValue / 10;  
}
*/

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes - MANUAL OVERRIDE SWITCH data from blynk
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  motorManualSwitch = param.asInt();
  // Update state
  Blynk.virtualWrite(V0, motorManualSwitch);
  //Serial.println(motorManualSwitch);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl","https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");

  Serial.println("Connected");
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

  // Detect sensor connections
  if (!uvSensor.begin()) { // UV Sensor
  Serial.println("Failed to find LTR390 sensor!");
  }


  //configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
  configTime(-5 * 3600, 3600, "pool.ntp.org", "time.nist.gov"); // UTC-5 for Eastern Time, +1 hour for DST

  // Initialize LED Strip Pin
  pinMode(ledStripPin, OUTPUT);
  digitalWrite(ledStripPin, LOW); // Start with the LED strip off

  // Initialize Manual Override Switch
  pinMode(manualSwitchPin, INPUT_PULLUP); // Assuming a pull-up switch configuration
}
}

void controlLED() {
  // Check manual override switch
  manualOverride = digitalRead(manualSwitchPin) == LOW; // Switch pressed

  // Only control LED based on time if manual override is not active
  if (!manualOverride) {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }

    int currentHour = timeinfo.tm_hour; // Get the current hour

    // Assuming you want the LED to be on from 6 PM to 6 AM
    if (currentHour >= 18 || currentHour < 6) {
      digitalWrite(ledStripPin, HIGH); // Turn on LED strip
      ledState = true;
      Serial.println("LED ON");
    } else {
      digitalWrite(ledStripPin, LOW); // Turn off LED strip
      ledState = false;
      Serial.println("LED OFF");
    }
  } else {
    // Manual override is active
    digitalWrite(ledStripPin, ledState ? HIGH : LOW); // Set LED strip according to ledState
  }
}

void loop()
{
  Blynk.run();
  timer.run();

  /*
  // ---------------- UV SENSING ------------------

  // Read UV data
  // SDA = Pin 23
  // SCL = Pin 22
  // Not using INT
  uint32_t uvIndex = uvSensor.readUVS(); // THIS WORKS
  
  // Send UV data to Blynk
  Blynk.virtualWrite(V5, uvIndex); // Assuming V5 is the Virtual Pin for UV data
  
  //Serial.print("UV Index: ");
  //Serial.println(uvIndex);

  // --- CAPACITIVE MOISTURE SENSOR AND WATER PUMP ----

  int capacitiveMoistureLevel;

  // Approx 2500 for dry
  // Approx 1200-1300 for wet

  capacitiveMoistureLevel = analogRead(capacitiveMoisturePin); //connect sensor to Analog 0
  // Serial.println(val); //print the value to serial port
  Blynk.virtualWrite(V6, capacitiveMoistureLevel); // write the capacitive moisture level to Blynk V6 virtual pin

  if((motorManualSwitch == 1) || (capacitiveMoistureLevel > 1500)){ // if motor override or level > 1500
    digitalWrite(motorOutput, HIGH); // turn motor on
  } else {
    digitalWrite(motorOutput, LOW); // turn motor off
  }

  */

  // ----- LED LIGHT PROGRAM -----

  // get UV sensor values in sunlight
  // currently indoors UV sensor just reads 0 so we are implementing using buffer values


  controlLED();

  delay(100); // delay between readings
}