#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define LDRPIN 3

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(F("Dovahkiin!"));

  pinMode(LED_BUILTIN, OUTPUT);

  // DHT
  dht.begin();

  // LDR (photoresistor)
  pinMode(LDRPIN, INPUT);
}

void loop() {
  Serial.println(F("FUS RO DAH!"));
  digitalWrite(LED_BUILTIN, LOW);
  delay(2000);
  digitalWrite(LED_BUILTIN, HIGH);

  // DHT
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hic = dht.computeHeatIndex(t, h, false);

  // LDR
  int light = digitalRead(LDRPIN);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(F("  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(F("  Light: "));
  Serial.println(light);
}
