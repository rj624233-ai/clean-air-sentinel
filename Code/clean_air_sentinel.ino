Clean air
#include <WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define MQ2_PIN 34
#define DHTPIN 4
#define DHTTYPE DHT11
#define LED_PIN 18
#define BUZZER_PIN 27

const char* ssid = "AndroidAP";
const char* password = "@mrjana78";

unsigned long channelID =  3237903;
const char* writeAPIKey = "ETW2OMTN52NICJA6";

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  dht.begin();
  Wire.begin(21,22);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);

  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  ThingSpeak.begin(client);
}

void loop() {
  int air = analogRead(MQ2_PIN);
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  Serial.print("Air: "); Serial.print(air);
  Serial.print(" | Temp: "); Serial.print(temp);
  Serial.print(" | Hum: "); Serial.println(hum);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Clean Air Sentinel");

  display.setTextSize(3);
  display.setCursor(20,16);
  display.print(air);

  display.setTextSize(1);
  display.setCursor(0,48);
  display.print("T:"); display.print(temp); display.print("C ");
  display.print("H:"); display.print(hum); display.print("%");

  display.setCursor(90,48);

  if (air > 4000) {
    display.print("BAD");
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    display.print("OK");
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }

  display.display();

  ThingSpeak.setField(1, air);
  ThingSpeak.setField(2, temp);
  ThingSpeak.setField(3, hum);
  ThingSpeak.writeFields(channelID, writeAPIKey);

  delay(15000);
}
