#include <strings_en.h>
#include <WiFiManager.h>
#include <wm_consts_en.h>
#include <wm_strings_en.h>
#include <wm_strings_es.h>

#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "IIT(BHU)";
const char* username = "21024006";
const char* user_password = "Joker0000";
const char* identity = "21024006";
const char* anonymous_identity = "";
const char* ca_cert = "";

const int INT_LED = 2;
AsyncWebServer server(80);

const char* loginPage = R"rawliteral(
<!DOCTYPE html>
<html>
<body>

<h2>Login Page</h2>

<form action="/login" method="post">
  Username: <input type="text" name="username"><br>
  Password: <input type="password" name="password"><br>
  <input type="submit" value="Submit">
</form>

</body>
</html>
)rawliteral";

void handleLogin(AsyncWebServerRequest *request) {
  if (request->hasParam("username", true) && request->hasParam("password", true)) {
    String enteredUsername = request->getParam("username", true)->value();
    String enteredPassword = request->getParam("password", true)->value();

    if (enteredUsername.equals(username) && enteredPassword.equals(user_password)) {
      request->send(200, "text/plain", "Login successful!");
      digitalWrite(INT_LED, HIGH);
    } else {
      request->send(403, "text/plain", "Access Denied");
    }
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
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE); // Clear previous settings
  WiFi.begin(ssid, user_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  digitalWrite(INT_LED, HIGH);
  Serial.println("Connected");

  // Handle login requests
  server.on("/login", HTTP_POST, [](AsyncWebServerRequest *request){
    handleLogin(request);
  });

  // Serve login page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", loginPage);
  });

  // Start server
  server.begin();
}

void loop() {
  // Your loop code here
}
