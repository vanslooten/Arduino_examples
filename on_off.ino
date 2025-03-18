// Use a button to execute a process: eg. process data/read sensors.
// The button turns the processing on or off. An LED indicates the process: it blinks while processing is performed,
// and is off when nothing happens.
// To ensure smooth operation, debouncing and timing is all done based on millis(), no delays used.
//
// Parts used from:
// https://roboticsbackend.com/arduino-turn-led-on-and-off-with-button/
// minor changes:
// Line 6: Use LED on Pin 13 (built in LED: the LED on the Arduino board)
// Line 17: Use INPUT_PULLUP (button without resistor) https://docs.arduino.cc/tutorials/generic/digital-input-pullup/

#define LED_PIN 13
#define BUTTON_PIN 7

byte lastButtonState = LOW;
byte doSomething = LOW;
byte ledState = LOW;

long blinkPrevMillis = 0;  // will store last time LED was updated

unsigned long debounceDuration = 50;  // millis
unsigned long lastTimeButtonStateChanged = 0;
unsigned long interval = 500;  // blink time

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastTimeButtonStateChanged > debounceDuration) {
    byte buttonState = digitalRead(BUTTON_PIN);
    if (buttonState != lastButtonState) {
      lastTimeButtonStateChanged = millis();
      lastButtonState = buttonState;
      if (buttonState == LOW) {
        doSomething = (doSomething == HIGH) ? LOW : HIGH;
        digitalWrite(LED_PIN, LOW);
      }
    }
  }

  // do something while blinking the led:
  if (doSomething) {  // process data/read sensors here...
    // blink led:
    if (currentMillis - blinkPrevMillis > interval) {
      // save the last time you blinked the LED
      blinkPrevMillis = currentMillis;
      // if the LED is off turn it on and vice-versa:
      ledState = !ledState;
      // set the LED with the ledState of the variable:
      digitalWrite(LED_PIN, ledState);
    }
  }
}
