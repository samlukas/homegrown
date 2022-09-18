#include <Arduino_JSON.h>

#include <SPI.h>
#include <RH_RF95.h>
#define RFM95_CS 4
#define RFM95_RST 2
#define RFM95_INT 3
RH_RF95 rf95(RFM95_CS, RFM95_INT);

#include <DHT_U.h>
#include <DHT.h>
#define DHTTYPE DHT11
#define DHTPIN 7
DHT dht(DHTPIN, DHTTYPE);

#define LIGHT A0
#define GROW 12

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  if (!rf95.init()) {
    Serial.println("wireless init failed");
  }

  dht.begin();

  pinMode(LIGHT, INPUT);
  pinMode(GROW, OUTPUT);
  digitalWrite(GROW, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len)) {
      Serial.print("got cmd: ");
      Serial.println((char*)buf);

      int light = analogRead(LIGHT);
      if (light > 400) {
        digitalWrite(GROW, HIGH);
      }
      else {
        digitalWrite(GROW, LOW);
      }
    
      JSONVar dataObj;
      dataObj["ham"] = "K3CL/VE3";
      dataObj["unit"] = "1";
      dataObj["light"] = light;
      dataObj["humidity"] = humidity;
      dataObj["temperature"] = temperature;
      String dataStr = JSON.stringify(dataObj);
      uint8_t data[dataStr.length() + 1];
      dataStr.toCharArray(data, dataStr.length() + 1);
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
    }
    else {
      Serial.println("no data");
    }
  }
}
