/*
Tutorial:
https://home.et.utwente.nl/slootenvanf/2025/03/13/simulation-of-anxiety/
*/
#include <Wire.h>
#include <VL53L1X.h>
#include <FastLED.h>

// Define LED strip parameters
#define NUM_LEDS 8
#define DATA_PIN 6
CRGB leds[NUM_LEDS];

// Define distance thresholds
#define NORMAL_DISTANCE 300
#define MAX_DISTANCE 400
#define MIN_DISTANCE 100
#define MIN_HEARTBEAT_INTERVAL 100 // ms
#define MAX_HEARTBEAT_INTERVAL 1000 // ms

VL53L1X sensor;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {}

  // Initialize I2C communication
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

  // Initialize VL53L1X sensor
  sensor.setTimeout(500);
  if (!sensor.init()) {
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
  }

  // Configure VL53L1X sensor
  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(50000);
  sensor.startContinuous(50); // Start continuous readings

  // Initialize LED strip
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
}

void loop() {
  // Read distance from VL53L1X sensor
  unsigned int distance = sensor.read();
  if (sensor.timeoutOccurred()) {
    Serial.println("TIMEOUT");
    return;
  }
  Serial.println(distance);

  // Calculate heartbeat interval based on distance
  int heartbeatInterval;
  if (distance > MAX_DISTANCE) {
    // Turn off LEDs if distance is greater than MAX_DISTANCE
    FastLED.clear();
    FastLED.show();
    return;
  } else if (distance < MIN_DISTANCE) {
    heartbeatInterval = MIN_HEARTBEAT_INTERVAL;
  } else {
    heartbeatInterval = map(distance, MIN_DISTANCE, MAX_DISTANCE, MIN_HEARTBEAT_INTERVAL, MAX_HEARTBEAT_INTERVAL);
  }

  // Display heartbeat pattern with pulsing effect
  for (int pulse = 0; pulse < 255; pulse += 5) {
    for (int i = 0; i < NUM_LEDS; ++i) {
      int brightness = map(abs(NUM_LEDS / 2 - i), 0, NUM_LEDS / 2, 255, 50);
      leds[i] = CHSV(map(i, 0, NUM_LEDS - 1, 0, 255), 255, brightness * (pulse / 255.0));
    }
    FastLED.show();
    delay(heartbeatInterval / 40);
  }

  for (int pulse = 255; pulse > 0; pulse -= 5) {
    for (int i = 0; i < NUM_LEDS; ++i) {
      int brightness = map(abs(NUM_LEDS / 2 - i), 0, NUM_LEDS / 2, 255, 50);
      leds[i] = CHSV(map(i, 0, NUM_LEDS - 1, 0, 255), 255, brightness * (pulse / 255.0));
    }
    FastLED.show();
    delay(heartbeatInterval / 40);
  }
}
