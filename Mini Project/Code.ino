#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
// Replace these with your WiFi credentials
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";
// Replace this with your Blynk authentication token
char auth[] = "YOUR_BLYNK_AUTH_TOKEN";
// Pin connected to the relay module to control the bulb
const int relayPin = D1;
// Pin connected to the LED to indicate connection status
const int statusLedPin = D2;
// Variable to store the current state ofthe bulb
int bulbState = 0;
// Blynk setup
BlynkTimer timer;
WidgetLED ledWidget(V3);
void setup() {
// Initialize Serial communication for debugging purposes
Serial.begin(115200);
// Initialize the relay pin and status LED pin as outputs
23
pinMode(relayPin, OUTPUT);
pinMode(statusLedPin, OUTPUT);
// Connect to Wi-Fi
connectToWiFi();
}
void loop() {
// Run Blynk communication
Blynk.run();
// Execute Blynk timer tasks
timer.run();
}
// Function to connect to Wi-Fi
void connectToWiFi() {
Serial.println("Connecting to Wi-Fi...");
WiFi.begin(ssid, pass);
// Attempt to connect to Wi-Fi
int attempts = 0;
while (WiFi.status() != WL_CONNECTED && attempts < 10) {
digitalWrite(statusLedPin, HIGH); // Blink status LED while connecting
delay(500);
digitalWrite(statusLedPin, LOW);
delay(500);
attempts++;
}
24
// Check if connected to Wi-Fi
if (WiFi.status() == WL_CONNECTED) {
Serial.println("Wi-Fi connected!");
digitalWrite(statusLedPin, HIGH); // Turn on status LED
// Initialize Blynk connection
Blynk.begin(auth, ssid, pass);
// Setup a timer to check Blynk connection status every 10 seconds
timer.setInterval(10000L, checkBlynkConnection);
} else {
Serial.println("Failed to connect to Wi-Fi!");
while (true) {} // Endless loop if Wi-Fi connection fails
}
}
// Blynk virtual pin handler for controlling the bulb
BLYNK_WRITE(V1) {
int newState = param.asInt(); // Get value fromthe app (0 or 1)
// Check ifthe new state is different from the current state
if (newState != bulbState) {
// Update the current state ofthe bulb
bulbState = newState;
// Turn the bulb on or off based on the app command
digitalWrite(relayPin, bulbState);
Serial.print("Bulb turned ");
Serial.println(bulbState ? "on" : "off");
}
}
25
// Blynk connection error handler
void checkBlynkConnection() {
if (!Blynk.connected()) {
Serial.println("Blynk disconnected!");
digitalWrite(statusLedPin, LOW); // Turn offstatus LED
connectToWiFi(); // Attempt to reconnect
}
}
// Blynk connection error handler
BLYNK_CONNECTED() {
Serial.println("Blynk connected!");
digitalWrite(statusLedPin, HIGH); // Turn on status LED
// Update Blynk LED widget to indicate connection
ledWidget.on();
}
// Blynk disconnection error handler
BLYNK_DISCONNECTED() {
Serial.println("Blynk disconnected!");
digitalWrite(statusLedPin, LOW); // Turn offstatus LED
// Update Blynk LED widget to indicate disconnection
ledWidget.off();
}