#define LED_PIN 13

uint8_t counter = 0; 

void setup() {
   delay(2000);
   pinMode(LED_PIN, OUTPUT);
}

void loop() {

  Serial.begin();
  Serial.write(counter);
  Serial.end();

  digitalWrite(LED_PIN, HIGH);
  delay(50);
  digitalWrite(LED_PIN, LOW);

  delay(50);
  counter++;
}
