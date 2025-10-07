#define BLYNK_DEVICE_NAME "ESP32"
#define BLYNK_TEMPLATE_ID "TMPL4zg0nUGPB"
#define BLYNK_TEMPLATE_NAME "Project final"
#define BLYNK_AUTH_TOKEN "8jdF5vRVd30FrPMmP-_Lo7XTld4GY5xV"

#include <Grove_LED_Bar.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"
#include <ArduinoHttpClient.h>

char auth[] = "8jdF5vRVd30FrPMmP-_Lo7XTld4GY5xV";
char ssid[] = "4G-Gateway-E6F8";
char pass[] = "EE5NTMT4TGB";

const char server[] = "192.168.8.101";  // CHANGE DEPENDING ON LAPTOP'S IPV4 ipconfig
int port = 3001;
WiFiClient wifi;
HttpClient client = HttpClient(wifi, server, port);

void sendSensorData();

#define BUZZER_PIN 19
#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
const int buttonPin = 2;
const int FAN_PIN = 7;

Grove_LED_Bar bar(4, 5, 1);

float h = 0;
float t = 0;
bool t_fan = false;
int i=0;
bool warning = false;
unsigned long lastBeepTime = 0;
bool beepState = false;
bool muteBuzzer = false;
int buttonState = HIGH;
int lastButtonState = HIGH;

BlynkTimer timer;
rgb_lcd lcd;

void sendSensorData() {

  t_fan = false;
  warning = false;
  h = dht.readHumidity();
  t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }


  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.println("°C");


  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

  if (t>=50){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("!!WARNING!!");
    lcd.setCursor(0, 1);
    lcd.print("T too high");
    warning = true;
    t_fan = true;
  } else if (h>=70){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("!!WARNING!!");
    lcd.setCursor(0, 1);
    lcd.print("H too high");
    warning = true;
  }else if (t<=0){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("!!WARNING!!");
    lcd.setCursor(0, 1);
    lcd.print("T too low");
    warning = true;
  }else if (h<=20){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("!!WARNING!!");
    lcd.setCursor(0, 1);
    lcd.print("H too low");
    warning = true;
  }else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print((int)t);
    lcd.print(" C     ");

    lcd.setCursor(0, 1);
    lcd.print("H:");
    lcd.print((int)h);
    lcd.print(" %     ");
  }

  if (!warning) {
    muteBuzzer = false;
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(FAN_PIN, LOW);

    //stop the tone playing:
    noTone(BUZZER_PIN);

    beepState = false;

  }
}

void setup() {
  Serial.begin(115200);
   while (!Serial);
  Serial.println("Boot OK, entering setup...");
  dht.begin();

  Wire.begin(17, 18);
  lcd.begin(16, 2);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(FAN_PIN, LOW);

  Serial.begin(115200);
  bar.begin();
  Serial.println("LED Bar initialized");

  Blynk.begin(auth, ssid, pass);

  timer.setInterval(2000L, sendSensorData);

// Network stuff starts here
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" connected!");
    Serial.print("Arduino IP: ");
    Serial.println(WiFi.localIP()); //get arduino's own wifi

    // Connection diagnostics
    Serial.println("Testing connections...");
    WiFiClient testClient;

    // pinging google to make sure the internet works and is not the problem :)
    if (testClient.connect("google.com", 80)) {
        Serial.println("✅ Internet connection works");
        testClient.stop();
    } else {
        Serial.println("❌ No internet connection");
    }

    if (testClient.connect(server, port)) {
        Serial.println("✅ Can reach your server");
        testClient.stop();
    } else {
        Serial.println("❌ Cannot reach your server");
    }

}

void loop() {
  Blynk.run();
  timer.run();

  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && lastButtonState == HIGH) {
    muteBuzzer = true;
    digitalWrite(BUZZER_PIN, LOW);
    beepState = false;
  }

  int level = map((int)t, 0, 50, 0, 10);
  level = constrain(level, 0, 10);

  bar.setLevel(level);

  if (warning && !muteBuzzer) {
    unsigned long currentTime = millis();
    if (currentTime - lastBeepTime >= 500) {
      beepState = !beepState;
      digitalWrite(BUZZER_PIN, beepState ? HIGH : LOW);
      lastBeepTime = currentTime;
      if (t_fan == true){
        Serial.println("Turning fan ON...");
        digitalWrite(FAN_PIN, HIGH);
      }
    }
  } else if (!warning) {
    muteBuzzer = false;   // reset mute when warning clears
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("Turning fan OFF...");
    digitalWrite(FAN_PIN, LOW);
    beepState = false;
  }

  lastButtonState = buttonState;

  // NETWORKING STUFF
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected! Reconnecting...");
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" reconnected!");
  }

  Serial.print("Arduino IP: ");
  Serial.println(WiFi.localIP()); //get arduino's own wifi
  // prepare JSON payload
  String payload = "{\"temperature\":" + String(t) + ",\"humidity\":" + String(h) + "}";
  Serial.print("Sending payload: ");
  Serial.println(payload);

  // stop any existing connection and make new request
  client.stop();
  delay(100);

  // send POST request
  Serial.println("Making HTTP request...");
  int result = client.post("/api/mkr-routers/data", "application/json", payload);
  Serial.print("POST result: ");
  Serial.println(result);

  if (result == 0) {
      int statusCode = client.responseStatusCode();
      String response = client.responseBody();

      Serial.print("✅ Status code: ");
      Serial.println(statusCode);
      Serial.print("Response: ");
      Serial.println(response);
  } else {
      Serial.print("❌ HTTP request failed with error: ");
      Serial.println(result);

      // debugging for common error codes
      if (result == -1) {
          Serial.println("  (-1 = connection failed)");
      } else if (result == -2) {
          Serial.println("  (-2 = send header failed)");
      } else if (result == -3) {
          Serial.println("  (-3 = send payload failed)");
      }
  }

  Serial.println("--- End of loop iteration ---");
  delay(2000);
}
