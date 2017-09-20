#define LED_PIN 13

#define UART_SPEED 57600

/*
 * This code is for stationary device
 */

uint8_t counter = 0; 

void setup() {
   delay(2000);
   pinMode(LED_PIN, OUTPUT);
   Serial.begin(UART_SPEED);
}

enum dataStates {
  IDLE,
  HEADER_RECEIVED,
  DATA_RECEIVED,
  CRC
};

uint8_t protocolState = IDLE;
uint8_t dataBytesReceived = 0;
uint32_t incomingData;
bool doRetransmit = false;

void loop() {

  if (Serial.available()) {

    uint8_t data = Serial.read();

    if (protocolState == IDLE && data == 0xff) {
      protocolState = HEADER_RECEIVED;
      dataBytesReceived = 0;
      incomingData = 0;
    } else if (protocolState == HEADER_RECEIVED) {

      if (dataBytesReceived == 0) {
        incomingData = data;
      } else {
        //Shift data to the left
        incomingData = (uint32_t) incomingData << 8;
        incomingData = (uint32_t) ((uint32_t) incomingData) + data;
      }

      dataBytesReceived++;

      if (dataBytesReceived == 4) {
        protocolState = IDLE;
        doRetransmit = true;
        digitalWrite(LED_PIN, HIGH);
      }
    }
  }

  if (doRetransmit) {
    Serial.write(0xfe);

    uint32_t toSend = incomingData;

    byte buf[4];
    buf[0] = toSend & 255;
    buf[1] = (toSend >> 8) & 255;
    buf[2] = (toSend >> 16) & 255;
    buf[3] = (toSend >> 24) & 255;
    Serial.write(buf, sizeof(buf));
    
    Serial.end();
    delay(30);
    digitalWrite(LED_PIN, LOW);
    
    Serial.begin(UART_SPEED);
    doRetransmit = false;
  }

}
