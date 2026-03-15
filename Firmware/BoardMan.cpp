#include "LittleFS.h"
#include "BoardMan.h"
#include "config.h"

WiFiClient client;
struct Session {
  uint32_t magic;
  int interval;
};

int connectToHub() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      delay(500);
      attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) && client.connect(SERVER_IP, SERVER_Port) {
    while (client.connected()) {
      if (client.available()) {
          String cmd = client.readStringUntil('\n');
          cmd.trim();
          return (cmd == "DOWNLOAD")?CMD_DOWN:(cmd == "CONFIGURE")?CMD_CONFIG:CMD_FAIL
      }
    }
  }
}

bool handleDownload() {
  if(!LittleFS.begin()){
        Serial.println("LittleFS Mount Failed");
        return 0;
      }
}

bool handleConfig() {
  if(!LittleFS.begin()){
        Serial.println("LittleFS Mount Failed");
        return 0;
      }
})

int appendData(float temp) {
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
      sleepUntil(0);
    }
  }
  // Append the Data byte-array into sessionData struct-toDo
  ESP.rtcUserMemoryWrite(0, (uint32_t*)&sessionData, sizeof(sessionData));
  // If rtcRAM memoryOverflow, write to LittleFS
}