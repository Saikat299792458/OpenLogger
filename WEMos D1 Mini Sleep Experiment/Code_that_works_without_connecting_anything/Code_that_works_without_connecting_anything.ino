#include <Wire.h>
#include <RTClib.h> // Using the Adafruit RTClib

RTC_DS3231 rtc;

void setup() {
  // 1. Initialize Hardware
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin(); // Default SDA/SCL for Wemos are D2/D1
  
  // 2. Visual confirmation of wake-up
  digitalWrite(LED_BUILTIN, LOW); // LED ON (Wemos LED is active low)
  delay(100);                     // Short burst to see the blink
  digitalWrite(LED_BUILTIN, HIGH); // LED OFF

  // 3. Initialize RTC
  if (!rtc.begin()) {
    while (1); // Trap if RTC not found
  }

  // 4. Clear existing Alarms and Flags
  rtc.disableAlarm(1);
  rtc.clearAlarm(1);
  
  // 5. Set Alarm 1 to fire every second
  // The 'DS3231_A1_PerSecond' mode triggers when seconds match (every sec)
  rtc.setAlarm1(rtc.now(), DS3231_A1_PerSecond);

  // 6. Go to Deep Sleep
  // We set the WDT for 1.5 seconds so the DS3231 Alarm (at 1.0s) 
  // hits while the chip is in the "Zombie" state.
  ESP.deepSleep(1500000); 
}

void loop() {
  // loop is never reached in Deep Sleep projects
}