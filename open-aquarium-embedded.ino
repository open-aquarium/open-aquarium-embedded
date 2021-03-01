#include "src/OpenAquarium.h"
OpenAquarium app;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  app.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("A");
  app.loop();
}
