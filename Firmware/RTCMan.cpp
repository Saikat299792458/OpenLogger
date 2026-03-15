#include <Wire.h>
#include <RTClib.h> // Using the Adafruit RTClib

RTC_DS3231 rtc;

void sleepUntil(int interval) {
  if (interval == 0) {
    // Sleep Forever
    ESP.deepSleep(0);
  }
  // Assuming interval will be shorter than one minute............................................
  Wire.begin(); // Default SDA/SCL for Wemos are D2/D1
  // 3. Initialize RTC
  if (!rtc.begin()) {
    while (1); // Trap if RTC not found
  }
  // 4. Clear existing Alarms and Flags
  rtc.disableAlarm(1);
  rtc.clearAlarm(1);
  // 5. Set Alarm 1 to fire every second
  rtc.setAlarm1(rtc.now() + TimeSpan(0, 0, 0, interval + 1), DS3231_A1_Minute);
  // 6. Go to Deep Sleep
  ESP.deepSleep(interval * exp(6), WAKE_RF_DISABLED); 
}

float readTemp() {
  delay(1000
}