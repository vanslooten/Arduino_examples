/*
This example shows how to take simple range measurements with the VL53L1X. The
range readings are in units of mm.

Tutorials:
https://home.et.utwente.nl/slootenvanf/tag/tof/
*/

#include <Wire.h>
#include <VL53L1X.h>

VL53L1X sensor;

void setup()
{
  while (!Serial) {}
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
  }

  // Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
  // You can change these settings to adjust the performance of the sensor, but
  // the minimum timing budget is 20 ms for short distance mode and 33 ms for
  // medium and long distance modes. See the VL53L1X datasheet for more
  // information on range and timing limits.
  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(50000);

  // Start continuous readings at a rate of one measurement every 50 ms (the
  // inter-measurement period). This period should be at least as long as the
  // timing budget.
  sensor.startContinuous(500); // changed: do measurement every 0.5 seconds
}

void loop()
{
  unsigned int distance = sensor.read(); // read sensor value and store in variable 'distance'
  // print distance only if no timeout occured:
  if (sensor.timeoutOccurred()) Serial.print("TIMEOUT");
  else Serial.print(distance);

  Serial.println();
}
