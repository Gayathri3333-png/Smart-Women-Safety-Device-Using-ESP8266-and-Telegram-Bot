#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// WiFi Credentials
const char* ssid = "";
const char* password = "";

// Telegram Bot Token
const char* botToken = "";
const String chatID = "";

// Initialize Telegram bot
WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);

// GPS Setup
static const int RXPin = 8, TXPin = 7;
static const uint32_t GPSBaud = 4800;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

//Push button setup
#define BUTTON_PIN 4 // GPIO4 (D2)
bool buttonPressed = false;

void setup() {
  Serial.begin(115200);
  ss.begin(GPSBaud);
  
//pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...\n");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected!\n");
  
  client.setInsecure();  // Required for HTTPS requests
}

void loop() {
  // if (digitalRead(BUTTON_PIN) == LOW) {  // Button pressed
  //   buttonPressed = true;
  //   delay(500);
  // }

  Serial.println(ss.readString());

  while (ss.available() > 0) {
  
    if (gps.encode(ss.read())) {
      if (gps.location.isUpdated()) {
          float latitude = gps.location.lat();
          float longitude = gps.location.lng();

          // Debugging: Print to Serial Monitor
          Serial.print("Latitude: ");
          Serial.println(latitude, 6);
          Serial.print("Longitude: ");
          Serial.println(longitude, 6);

          // Create Google Maps location link
          String locationLink = "https://www.google.com/maps?q=" + String(latitude, 6) + "," + String(longitude, 6);

          // Send location to Telegram
          bot.sendMessage(chatID, "⚠ Emergency Alert! ⚠\nLive Location: " + locationLink, "");

          Serial.println("Location Sent to Telegram!");
      }
    }}


}

    
