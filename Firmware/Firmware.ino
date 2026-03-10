#include <Wire.h>
#include <RTClib.h> // Using the Adafruit RTClib
#include "LittleFS.h"

RTC_DS3231 rtc;
const int FLASH_ACK = D5;
const int FLASH_CONF = D6;
struct Session {
  uint32_t magic;
  int interval;
};

void setup() {
  pinMode(FLASH_ACK, INPUT_PULLUP);
  delay(100); // Debounce

  if (digitalRead(FLASH_ACK) == LOW) {
    pinMode(FLASH_CONF, OUTPUT);
    digitalWrite(FLASH_CONF, LOW);
    flashData();
    digitalWrite(FLASH_CONF, HIGH);
  } else {
    // Read Session Data from RTC RAM
    Session sessionData;
    int activeInterval;
    ESP.rtcUserMemoryRead(0, (uint32_t*) &sessionData, sizeof(sessionData));
    if (sessionData.magic == 0xDEADBEEF) { // Data is valid
      activeInterval = sessionData.interval;
    } else { // Data not found in RTC, maybe crashed, recover data from wemos Flash, also log an error report to flash-todo
      LittleFS.begin();
      File f = LittleFS.open("/config.txt", "r");
      if (f) {
        activeInterval = f.readStringUntil('\n').toInt();
        f.close();
        
        // Flush the data to RTC RAM
        sessionData.magic = 0xDEADBEEF;
        sessionData.interval = activeInterval;
      } else {
        // No config found anywhere -> go to DeepSleep forever
        ESP.deepSleep(0);
      }
    }
    float temp = readTemp();
    // Append the Data byte-array into sessionData struct-toDo
    ESP.rtcUserMemoryWrite(0, (uint32_t*)&sessionData, sizeof(sessionData));
  }
}

float readTemp() {
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