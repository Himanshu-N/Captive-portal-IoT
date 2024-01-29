#include <ESP8266WiFi.h>
#include <wpa2_enterprise.h>
#include <ESP8266WebServer.h>
#include "IntroPage.h"
#include "LedPage.h"
// SSID to connect to
char ssid[] = "IIT(BHU)";
char username[] = "21024007";
char identity[] = "21024007";
char password[] = "Aryan@2003";
const int INT_LED = 2;  // GPIO pin for built-in LED on NodeMCU
uint8_t target_esp_mac[6] = {0x24, 0x0a, 0xc4, 0x9a, 0x58, 0x28};

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", root_html);
}

void handleLedControl() {
  server.send(200, "text/html", led_control_html);
}

void handleLedOn() {
  digitalWrite(INT_LED, LOW);  // Turn on the LED
  server.send(200, "text/plain", "LED turned on!");
}

void handleLedOff() {
  digitalWrite(INT_LED, HIGH);  // Turn off the LED
  server.send(200, "text/plain", "LED turned off!");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.setDebugOutput(true);

  pinMode(INT_LED, OUTPUT);  // Set the LED pin as output
  delay(5000); // delay for stabilizing the relay
  WiFi.mode(WIFI_STA);
  wifi_set_opmode(STATION_MODE);

  struct station_config wifi_config;
  memset(&wifi_config, 0, sizeof(wifi_config));
  strcpy((char*)wifi_config.ssid, ssid);
  strcpy((char*)wifi_config.password, password);
  wifi_station_set_config(&wifi_config);
  wifi_set_macaddr(STATION_IF, target_esp_mac);

  wifi_station_set_wpa2_enterprise_auth(1);
  wifi_station_clear_cert_key();
  wifi_station_clear_enterprise_ca_cert();
  wifi_station_clear_enterprise_identity();
  wifi_station_clear_enterprise_username();
  wifi_station_clear_enterprise_password();
  wifi_station_clear_enterprise_new_password();

  wifi_station_set_enterprise_identity((uint8*)identity, strlen(identity));
  wifi_station_set_enterprise_username((uint8*)username, strlen(username));
  wifi_station_set_enterprise_password((uint8*)password, strlen(password));

  wifi_station_connect();

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Blink the LED 3 times when connected
  for (int i = 0; i < 6; i++) {
    digitalWrite(INT_LED, LOW);
    delay(100);
    digitalWrite(INT_LED, HIGH);
    delay(100);
  }

  // Set up web server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/led-onoff", HTTP_GET, handleLedControl);
  server.on("/led/on", HTTP_POST, handleLedOff);
  server.on("/led/off", HTTP_POST, handleLedOn);

  // Start server
  server.begin();
}

void loop() {
  server.handleClient();
  delay(10);  // Allow the server to handle incoming requests
}
