#include "DHTesp.h"
#include "WiFi.h"

#define LED 2
#define DHT_PIN 19
#define ECHO 22
#define TRIG 23


DHTesp dhtSensor;
const char *ssid = "Wokwi-GUEST";
const char *parola = "";

void initWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, parola);
  Serial.println("Conectare la WiFi");
  while(WiFi.status() != WL_CONNECTED) continue;
  Serial.println(WiFi.localIP());
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);
  Serial.begin(115200);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
}

void loop() {
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  int distance = pulseIn(ECHO, HIGH) / 58;

  if (data.temperature < 25 && distance > 50)
    digitalWrite(LED, LOW);
  else digitalWrite(LED, HIGH);

}
