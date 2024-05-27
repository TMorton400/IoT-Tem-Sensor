
#define BLYNK_TEMPLATE_ID "TMPL5K1pyzO6K"
#define BLYNK_TEMPLATE_NAME "SmartTempSensor"
#define BLYNK_AUTH_TOKEN "vM-jCpi_CkiXL_y4kI0HMO1tXHSIvg0z"

#include "DHT.h"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h> 

#define LED_PWR   14
#define LED_RED   12
#define LED_GREEN 27
#define LED_BLUE  26

#define DHTPIN 16
#define DHTTYPE DHT11

char ssid[] = "SHELL-483758";
char password[] = "hkfKvfM3MQhk";

char auth[] = BLYNK_AUTH_TOKEN;


DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(LED_PWR, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_PWR, HIGH);

  // Connect to Wi-Fi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  Blynk.begin(auth, ssid, password);

  dht.begin();
  delay(2000); // Adding a delay to allow the sensor to stabilize
}

void led(int red, int green, int blue) {
  analogWrite(LED_RED, 255 - red);
  analogWrite(LED_BLUE, 255 - blue);
  analogWrite(LED_GREEN, 255 - green);
}


void loop() {

  float temp = dht.readTemperature();

  if (!isnan(temp)) {
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" °C");

    if (temp > 30.0) { // If temperature is too hot (adjust threshold as needed)
      led(255, 0, 0); // Red
      Serial.println("Temp too hot please cool down!");
      Blynk.logEvent("temp_too_high");
    } else if (temp < 25.0) { // If temperature is too cold (adjust threshold as needed)
      led(0, 0, 255); // Blue
      Serial.println("Temp too cold please warm up!");
      Blynk.logEvent("temp_too_cold");
    } else { // If temperature is within the normal range
      led(0, 255, 0); // Green

    }

    delay(2000);
  } else {
    Serial.println("Failed to read from DHT sensor!");
    delay (500);
   }

  Blynk.virtualWrite(V0, temp);
}

