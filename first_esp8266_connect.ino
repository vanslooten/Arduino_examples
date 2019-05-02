/**
 * Based on these examples:
 * https://create.arduino.cc/projecthub/jeffpar0721/add-wifi-to-arduino-uno-663b9e
 * http://yaab-arduino.blogspot.nl/p/wifiesp.html
 * 
 * This sketch uses the library WiFiEsp. If it is not installed yet, install it via Tools > Manage Libraries
 * or get it from https://github.com/bportaluri/WiFiEsp and extract the Zip-file to Documents\Arduino\libraries\WiFiEsp
 * 
 * Check the output in the Serial Monitor.
 */

#include "WiFiEsp.h"

// Emulate EspSerial on pins 2/3 if not present
#ifndef HAVE_HWEspSerial
#include "SoftwareSerial.h"
SoftwareSerial EspSerial(2, 3); // RX, TX
#endif

char ssid[] = "mspot";           // your network SSID (name)
char pass[] = "********";        // your network password (change it)
int status = WL_IDLE_STATUS;     // the Wifi radio's status

void setup() {
  // initialize serial for debugging
  Serial.begin(115200);
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
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
}

void loop()
{
  // check the network connection once every 10 seconds
  Serial.println();
  printCurrentNet();
  printWifiData();
  
  delay(10000);
}

void printWifiData() {
  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print your MAC address
  byte mac[6];
  WiFi.macAddress(mac);
  char buf[20];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
  Serial.print("MAC address: ");
  Serial.println(buf);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to
  byte bssid[6];
  WiFi.BSSID(bssid);
  char buf[20];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", bssid[5], bssid[4], bssid[3], bssid[2], bssid[1], bssid[0]);
  Serial.print("BSSID: ");
  Serial.println(buf);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.println(rssi);
}
