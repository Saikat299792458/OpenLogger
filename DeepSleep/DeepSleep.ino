#include <ESP8266WiFi.h>

// Most ESP8266 boards have the LED on GPIO2 (D4) or GPIO16 (D0).
// On the Wemos D1 Mini and NodeMCU, LED_BUILTIN is usually defined correctly.
const int chipLed = LED_BUILTIN; 

void setup() {
  // 1. Initialize the LED pin
  pinMode(chipLed, OUTPUT);
  
  // 2. Turn LED ON (Note: Most ESP8266 LEDs are active LOW, so LOW = ON)
  digitalWrite(chipLed, LOW);
  delay(1000); // Keep it on for 100ms so we can see it
  
  // 3. Turn LED OFF
  digitalWrite(chipLed, HIGH);

  // 4. Force WiFi to stay OFF to save battery during this brief wake period
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();

  // 5. Go to Deep Sleep for 1 second (1,000,000 microseconds)
  // WAKE_RF_DISABLED ensures the WiFi radio doesn't draw 100mA on boot
  ESP.deepSleep(3e6);
}

void loop() {
}