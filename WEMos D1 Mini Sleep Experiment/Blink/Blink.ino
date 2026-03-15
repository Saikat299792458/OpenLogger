
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Blink to show we are awake
  digitalWrite(LED_BUILTIN, LOW); 
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);

  ESP.deepSleep(2e6, WAKE_RF_DISABLED); 
}

void loop() {
  // Never reached
}