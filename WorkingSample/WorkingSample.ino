#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;
const int RTC_VCC_PIN = D6;
const int interval = 5;

void setup() {
  // 1. Immediate Power Management
  pinMode(RTC_VCC_PIN, OUTPUT);
  digitalWrite(RTC_VCC_PIN, HIGH); // Power the RTC I2C bus
  delay(10); // Wait for RTC to wake up from Battery Mode

  Serial.begin(115200);
  Wire.begin(D2, D1);
  
  if (!rtc.begin()) {
    Serial.println("RTC fail!");
    while(1);
  }

  // 4. Clear existing Alarms and Flags
  rtc.disableAlarm(1);
  rtc.clearAlarm(1);
  // 5. Set Alarm 1 to fire every second
  rtc.setAlarm1(rtc.now() + TimeSpan(0, 0, 0, 10), DS3231_A1_Minute);
  // 6. Go to Deep Sleep
  Serial.println("Going to Deep Sleep...");
  ESP.deepSleep(9e6, WAKE_RF_DISABLED); 
}

void loop() {}