#include <Wifi.h>
#include <Webserver.h>
#include <ArduinoJson.h>
#include <FastLED.h>

//Config
#define LED_PIN     5
#define NUM_LEDS    //Number of LEDs you have on the strip
#define BRIGHTNESS  128 //Max is about 255 for most of them

const char* ssid = "WIFI_SSID";
const char* password = "PASSWORD";

//Global
CRBG leds[NUM_LEDS];
WebServer server(80);

String currentPattern = "off";
unsigned long lastUpdate = 0;
int chaseIndex = 0;

//Wifi
void connectWiFi(){
    WiFi.beign(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nConnected. IP: " + WiFi.localIP().toString());
}

//Handlers
void handleColor(){
    StaticJsonDocument<100> doc;
    DeserializationError err = deserializationJson(doc, server.arg("plain"));
    if (err){
        server.send(400, "application/json", "{\"error\":\"Invalid JSON!!\"}"); //WHAT THE DUCKING, shitty
        return;
}

String hex = doc["color"];
if (hex.length() != 7 || hex[0] != '#'){
    server.send(400, "application/json", "{\"error\":\"Invalid color format\"}");
    return;
}

  int r = strtol(hex.substring(1, 3).c_str(), nullptr, 16);
  int g  = strtol(hex.substring(3,5).c_str(), nullptr, 16);
  int b = strtol(hex.substring(5, 7).c_str(), nullptr, 16);

  fill_solid(leds, NUM_LEDS, CRBG(r,g,b));
  FastLED.show();

  void handlePattern(){
      //Wanna do this in the future; Not required rn; Not much of a usecase at home.
  }

  //Setup. To Talk with my GoLang backend
void setup(){
  Serial.begin(115200);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  
  connectWiFi();

  server.on("/color", HTTP_POST, handleColor);
  server.begin();

  Serial.println("Ready to Use!");
}

//handle loop
void loop(){
    server.handleClient(); //Ig
}
