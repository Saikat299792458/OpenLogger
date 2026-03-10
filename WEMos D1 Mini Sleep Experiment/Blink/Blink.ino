
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Blink to show we are awake
  digitalWrite(LED_BUILTIN, LOW); 
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);

  ESP.deepSleep(0); 
}

void loop() {
  // Never reached
}