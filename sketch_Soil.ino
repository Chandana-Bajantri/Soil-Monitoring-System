#define BLYNK_TEMPLATE_ID "TMPL3APQZ3WAC"
#define BLYNK_TEMPLATE_NAME "Soil Monitoring System"
#define BLYNK_AUTH_TOKEN "us7RFNaYa5N3Xcyn8M5jZkfHewkt_Mb2"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

char ssid[] = "Galaxy A13";
char pass[] = "onetonine";

#define DHTPIN 4
#define DHTTYPE DHT11
#define SOIL 34
#define BUZZER 5

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensorData() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  int soilValue = analogRead(SOIL);
  int moisturePercent = map(soilValue, 0, 4095, 100, 0);

  // ✅ Print values on Serial Monitor
  Serial.print("🌡 Temperature: ");
  Serial.print(t);
  Serial.print(" °C | 💧 Humidity: ");
  Serial.print(h);
  Serial.print(" % | 🌱 Soil Moisture: ");
  Serial.print(moisturePercent);
  Serial.println(" %");

  // ✅ Send values to Blynk
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);
  Blynk.virtualWrite(V0, moisturePercent);

  // ✅ Irrigation Logic
  if (moisturePercent < 30) {
    digitalWrite(BUZZER, HIGH);
    Blynk.logEvent("soil_dry", "Soil is Dry! Irrigation Needed");
    Blynk.virtualWrite(V3, "Soil is Dry - Irrigation Needed 🚨");
  } else {
    digitalWrite(BUZZER, LOW);
    Blynk.virtualWrite(V3, "Soil moisture is enough ✅");
  }
}

void setup() {
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();

  timer.setInterval(2000L, sendSensorData); // every 2 seconds
}

void loop() {
  Blynk.run();
  timer.run();
}