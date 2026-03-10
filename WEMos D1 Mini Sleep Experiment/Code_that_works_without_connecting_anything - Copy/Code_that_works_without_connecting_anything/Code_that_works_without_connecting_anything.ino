#include <Wire.h>
#include <RTClib.h> // Using the Adafruit RTClib

RTC_DS3231 rtc;

void setup() {
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
  if (!rtc.setAlarm1(rtc.now() + TimeSpan(0, 0, 0, 6), DS3231_A1_Minute)) {
      Serial.println("Error setting alarm");
  }

  // 6. Go to Deep Sleep

  // Set pins to INPUT_PULLUP to match the DS3231's pull-up voltage
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  ESP.deepSleep(5e6, WAKE_RF_DISABLED); 
}

void loop() {
  // loop is never reached in Deep Sleep projects
}