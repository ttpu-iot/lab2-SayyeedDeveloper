#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* readURL = "http://api.thingspeak.com/channels/3109942/feeds.json?results=1";

#define RED_LED     26
#define GREEN_LED   27
#define BLUE_LED    14
#define YELLOW_LED  12

// ---------- Setup ----------
void setup() {
  Serial.begin(115200);
  
  Serial.println("Asadullo, Lab 2");

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);

  // Connect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// ---------- Loop ----------
void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected, reconnecting...");
    WiFi.begin(ssid, password);
    delay(1000);
    return;
  }

  HTTPClient http;

  http.begin(readURL);

  int httpCode = http.GET();

  if (httpCode != 200) {
    Serial.print("HTTP error: ");
    Serial.println(httpCode);
  } 
  else {
    String payload = http.getString();
    Serial.println("---- Raw JSON Response ----");
    Serial.println(payload);
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      Serial.print("JSON Parsing Error: ");
      Serial.println(error.c_str());
    } 
    else {
      String field1 = doc["feeds"][0]["field1"];
      String field2 = doc["feeds"][0]["field2"];
      String field3 = doc["feeds"][0]["field3"];
      String field4 = doc["feeds"][0]["field4"];
      String field5 = doc["feeds"][0]["field5"];

      if (field1 == "LOW"){
          digitalWrite(RED_LED,LOW);
      }
      else if(field1 == "HIGH"){
        digitalWrite(RED_LED, HIGH);
      }

      if (field2 == "LOW"){
          digitalWrite(GREEN_LED,LOW);
      }
      else if(field2 == "HIGH"){
        digitalWrite(GREEN_LED, HIGH);
      }
      if (field3 == "LOW"){
          digitalWrite(BLUE_LED,LOW);
      }
      else if(field3 == "HIGH"){
        digitalWrite(BLUE_LED, HIGH);
      }
      if (field4 == "LOW"){
          digitalWrite(YELLOW_LED,LOW);
      }
      else if(field4 == "HIGH"){
        digitalWrite(YELLOW_LED, HIGH);
      }
      Serial.println("User: " + field5);
    }
  }

  http.end();
  delay(5000);
}
