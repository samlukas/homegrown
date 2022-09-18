#include <Arduino_JSON.h>

#include <SPI.h>
#include <RH_RF95.h>
RH_RF95 rf95(8, 3);

int led = LED_BUILTIN;

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  while (!Serial);
  if (!rf95.init()) {
    Serial.println("init failed");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(led, LOW);
  while (Serial.available() == 0);
  String cmd = Serial.readStringUntil('\n');
  cmd.trim();

  uint8_t data[cmd.length() + 1];
  cmd.getBytes(data, cmd.length() + 1);
  rf95.send(data, sizeof(data));
  rf95.waitPacketSent();
  
  if (rf95.waitAvailableTimeout(3000)) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len)) {
      digitalWrite(led, HIGH);
      Serial.println((char *)buf);
    }
    else {
      Serial.println("recv failed");
    }
  }
}
