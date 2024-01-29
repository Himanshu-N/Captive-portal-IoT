#include <Arduino.h>
#include "WiFi.h"

#define WIFI_NETWORK "realme"
#define WIFI_PASSWORD "123456789"
#define WIFI_TIMEOUT_MS 20000
const int LED_PIN = 2;

WiFiServer server(80);

void connectToWifi() {
  Serial.print("Connecting to WIFI");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);

  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS) {
    Serial.print(".");
    delay(100);
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Failed!");
  } else {
    Serial.print("Connection Successful");
    Serial.println(WiFi.localIP());
    server.begin();
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  connectToWifi();
  Serial.println("turn");
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String req = client.readStringUntil('/r');
    
    client.flush();

    if (req.indexOf("POST /led/on") != -1) {
      digitalWrite(LED_PIN, HIGH);
    } else if (req.indexOf("POST /led/off") != -1) {
      digitalWrite(LED_PIN, LOW);
    }

    // Send a dynamic HTML response to the client
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<html><body>");
    client.println("<h1>ESP32 LED Control</h1>");
    client.println("<form action='/led/on' method='POST'><input type='submit' value='Turn On'></form>");
    client.println("<form action='/led/off' method='POST'><input type='submit' value='Turn Off'></form>");
    client.println("</body></html>");

    // Close the connection
    client.stop();
  }
  
}
