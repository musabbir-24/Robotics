#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// Define hardware type and number of devices
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4 // Number of 8x8 matrices (8x32 = 4 devices)

// Define SPI pins
#define CLK_PIN 18
#define DATA_PIN 23
#define CS_PIN 5

// Initialize LED matrix
MD_Parola display = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// WiFi credentials
const char* ssid = "Musabbir Hossain";
const char* password = "altaf_bapex2004";

// NTP setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 6 * 3600, 60000); // UTC+6 with 60s update interval

// Blinking colon variables
bool showColon = true;
unsigned long previousMillis = 0;
const long interval = 500; // Blink every 500ms

void setup() {
  // Start Serial Monitor
  Serial.begin(115200);

  // Initialize the display
  display.begin();
  display.setIntensity(5); // Set brightness (0-15)
  display.displayClear();
  display.setTextAlignment(PA_CENTER); // Center-align the text

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Initialize NTP client
  timeClient.begin();
}

void loop() {
  timeClient.update(); // Update time from NTP server

  // Get current time
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();

  // Convert to 12-hour format
  if (currentHour == 0) {
    currentHour = 12;
  } else if (currentHour > 12) {
    currentHour -= 12;
  }

  // Create time string (HH:MM) with leading zeros
  char timeString[6];
  snprintf(timeString, sizeof(timeString), "%02d%c%02d", currentHour, showColon ? ':' : ' ', currentMinute);

  // Blink colon
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    showColon = !showColon; // Toggle colon visibility
  }

  // Display time (center-aligned)
  display.displayClear();
  display.print(timeString);
  display.displayAnimate();
  delay(100); // Small delay for smooth updates
}
