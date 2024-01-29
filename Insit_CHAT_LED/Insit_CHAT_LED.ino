#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "IIT(BHU)";
const char* username = "21024006";
const char* user_password = "Joker0000";
const char* identity = "21024006";
const char* anonymous_identity = "";
const char* ca_cert = "";

const int INT_LED = 2;
bool ledState = LOW;  // Initial LED state is off
AsyncWebServer server(80);

void handleRoot(AsyncWebServerRequest *request) {
  String html = "<html><body>";
  html += "<h2>Captive Portal Login Page</h2>";
  html += "<form action='/login' method='post'>";
  html += "Username: <input type='text' name='username'><br>";
  html += "Password: <input type='password' name='password'><br>";
  html += "<input type='submit' value='Submit'>";
  html += "</form>";
  html += "<p><a href='/led/on'>Turn LED On</a></p>";
  html += "<p><a href='/led/off'>Turn LED Off</a></p>";
  html += "</body></html>";
  request->send(200, "text/html", html);
}

void handleLogin(AsyncWebServerRequest *request) {
  if (request->hasParam("username", true) && request->hasParam("password", true)) {
    String enteredUsername = request->getParam("username", true)->value();
    String enteredPassword = request->getParam("password", true)->value();

    // Your authentication logic here

    request->send(200, "text/plain", "Login successful!");
  } else {
    request->send(400, "text/plain", "Bad Request");
  }
}

void handleLedOn(AsyncWebServerRequest *request) {
  ledState = HIGH;
  digitalWrite(INT_LED, ledState);
  request->send(200, "text/plain", "LED turned on");
}

void handleLedOff(AsyncWebServerRequest *request) {
  ledState = LOW;
  digitalWrite(INT_LED, ledState);
  request->send(200, "text/plain", "LED turned off");
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
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(F("."));
  }

  Serial.println("Connected");
  digitalWrite(INT_LED, ledState);
  Serial.println(WiFi.localIP());

  // Handle login requests
  server.on("/", HTTP_GET, handleRoot);
  server.on("/login", HTTP_POST, handleLogin);

  // LED control endpoints
  server.on("/led/on", HTTP_GET, handleLedOn);
  server.on("/led/off", HTTP_GET, handleLedOff);

  // Start server
  server.begin();
}

void loop() {
  // No need for explicit handling of client requests in the loop with AsyncWebServer
}
