/*
  Open Aquarium - Data logger
*/
const int TEMPERATURE_SENSOR = A0;
const int LIGHT_SENSOR = A1;

void setup()
{
  Serial.begin(9600);

  pinMode(TEMPERATURE_SENSOR, INPUT);
  pinMode(LIGHT_SENSOR, INPUT);
}

void loop()
{
  log("Open Aquarium");

  String lightText = "Light: ";
  lightText.concat(readLight(TEMPERATURE_SENSOR));
  log(lightText);

  String temperatureText = "Temperature: ";
  temperatureText.concat(readTemperatureC(LIGHT_SENSOR));
  log(temperatureText);

  delay(1000);
}

void log(String value)
{
  Serial.println(value);
}

int readLight(int port)
{
  return analogRead(port);
}

float readTemperatureC(int port)
{
  float temperatureVoltage = 0;
  float temperatureC = 0;
  temperatureVoltage = (analogRead(port) * 0.004882814);
  temperatureC = ((temperatureVoltage - 0.5) * 100);
  return temperatureC;
}