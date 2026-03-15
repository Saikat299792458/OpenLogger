#include "boardManager.h"
#include "RTCMan.h"
#include "config.h"

void setup() {
  pinMode(FLASH_ACK, INPUT_PULLUP);
  delay(100); // Debounce
  int nextInt = 0;

  if (digitalRead(FLASH_ACK) == LOW) { // Config/Download Mode
    pinMode(FLASH_CONF, OUTPUT);
    digitalWrite(FLASH_CONF, HIGH);
    // Try to establish a connection
    if (int status = connectToHub()) { // Success
      if (status == CMD_DOWN) {
        handleDownload(); // read session data and send to server
      } else if (status == CMD_CONFIG) {
        nextInt = handleConfig(); // receive session parameters and write to Flash
        // Set date and time to RTC - ToDo
      }
    }
    digitalWrite(FLASH_CONF, LOW);
  } else { // Periodic Logging Mode
    float temp = readTemp();
    nextInt = appendData(temp);
  }
  sleepUntil(nextInt);
}

void loop() {
  // loop is never reached in Deep Sleep projects
}