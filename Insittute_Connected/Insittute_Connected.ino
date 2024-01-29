#include <Arduino.h>

#include "WiFi.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "esp_wpa2.h" //wpa2 library for connections to Enterprise networks

const char* ssid = "IIT(BHU)";
const char* username = "21024006";
const char* user_password = "Joker0000";
const char* identity = "21024006";
const char* anonymous_identity = "";
const char* ca_cert = "";

const int INT_LED = 2;
AsyncWebServer server(80);

void handleRoot(AsyncWebServerRequest *request) {
  String html = "<html><body>";
  html += "<h2>Captive Portal Login Page</h2>";
  html += "<form action='/login' method='post'>";
  html += "Username: <input type='text' name='username'><br>";
  html += "Password: <input type='password' name='password'><br>";
  html += "<input type='submit' value='Submit'>";
  html += "</form></body></html>";
  request->send(200, "text/html", html);
}

void handleLogin(AsyncWebServerRequest *request) {
  if (request->hasParam("username", true) && request->hasParam("password", true)) {
    String enteredUsername = request->getParam("username", true)->value();
    String enteredPassword = request->getParam("password", true)->value();

    // Your authentication logic here

    request->send(200, "text/plain", "Login successful!");
    digitalWrite(INT_LED, HIGH);
  } else {
    request->send(400, "text/plain", "Bad Request");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(INT_LED, OUTPUT);

  // Connect to WiFi with WPA2-Enterprise
  WiFi.disconnect(true);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.begin(ssid, WPA2_AUTH_PEAP, identity, username, user_password);
  Serial.print("Connecting to ");
//  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(F("."));
  }

  Serial.println("Connected");
  digitalWrite(INT_LED, HIGH);
  Serial.println(WiFi.localIP());
  delay(2000);
  digitalWrite(INT_LED, LOW);

  // Handle login requests
  server.on("/", HTTP_GET, handleRoot);
  server.on("/login", HTTP_POST, handleLogin);

  // Start server
  server.begin();
}

void loop() {
  
  
}
