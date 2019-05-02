/*
 * Build the circuit (connect sensor pin to pin 2, if you use an other pin, change it below at #define DHTPIN)
 * If not installed yet, install the libraries: "Adafruit Unified Sensor" and "DHT library" (via Sketch > Include Library, search for the name)
 * Change sensor type DHTTYPE to DHT11
 * 
 * Based on basic DHT example (File > Examples > DHT senor library, "DHT_Unified_Sensor"). Try this first if starting with the DHT sensor.
 * Combined with: https://github.com/mathworks/thingspeak-arduino/blob/master/examples/ESP8266/via%20AT%20commands/WriteMultipleFields/WriteMultipleFields.ino
 * 
 */
 
// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

// wifi libraries:
#include "WiFiEsp.h"

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>

// ThingSpeak library
#include "ThingSpeak.h"

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7); // first parameter is I2C bus address, this can be: 0x27, 0x3F or 0x38 (try all values if it does not work)

#define DHTPIN 4     // Digital pin connected to the DHT sensor 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

// global variables and settings for Wifi:

// Emulate EspSerial on pins 2/3 if not present
#ifndef HAVE_HWEspSerial
#include "SoftwareSerial.h"
SoftwareSerial EspSerial(2, 3); // RX, TX
#endif

int status = WL_IDLE_STATUS;     // the Wifi radio's status

// Wifi params:
const char* ssid = "mspot";
const char* password = "*********";

WiFiEspClient client;

// thingspeak variables
unsigned long myChannelNumber = 000000;
const char * myWriteAPIKey = "APIKEYHERE";

void setup() {
  Serial.begin(9600);

  // initialize serial for ESP module
  EspSerial.begin(9600);
  // initialize ESP module
  WiFi.init(&EspSerial);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, password);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  
  // Initialize DHT sensor:
  dht.begin();

  // activate LCD module
  lcd.begin(16,2); // for 16 x 2 LCD module
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH); // turn on backlight

  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    lcd.home(); // set cursor to 0,0 (first character on first line)
    lcd.print("Temp: ");
    lcd.print(event.temperature);
    ThingSpeak.setField(1, event.temperature); // store ThingSpeak field
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    lcd.setCursor (0,1);        // go to start of 2nd line
    lcd.print("Humidity: ");
    lcd.print(event.relative_humidity);
    ThingSpeak.setField(2, event.relative_humidity);  // store ThingSpeak field
  }

  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200)
    Serial.println(F("Channel update OK."));
  else {
    Serial.print(F("Problem updating channel. HTTP error code "));
    Serial.println(String(x));
  }

  // Delay between measurements.
  delay(30000); // delay 30 seconds to limit requests to webserver
}
