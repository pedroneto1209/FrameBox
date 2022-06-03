#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESPAsyncWebServer.h>
#include <stdio.h>
#include <string.h>

// Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// Devices
#define LED 2
#define BUZZER 18

// Wifi
#define WIFI_NAME "VIVOFIBRA-B190"
#define WIFI_PASSWORD "9A93A32AA7"

// Variables
unsigned long previousMillis = 0;
unsigned long interval = 30000;
bool isOpen = false;
const unsigned char epd_bitmap_teste[] PROGMEM = {};

// Classes
AsyncWebServer server(80);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void buzzer() {
  if (isOpen) {
    Serial.println("Opening sound");
    ledcAttachPin(BUZZER, 1);
    ledcWriteNote(1, NOTE_C, 8);
    delay(100);
    ledcDetachPin(BUZZER);
    delay(50);
    ledcAttachPin(BUZZER, 1);
    ledcWriteNote(1, NOTE_C, 8);
    delay(100);
    ledcDetachPin(BUZZER);
    delay(50);

  } else {
    Serial.println("Closing sound");
    ledcAttachPin(BUZZER, 1);
    ledcWriteNote(1, NOTE_C, 8);
    delay(100);
    ledcDetachPin(BUZZER);
    delay(50);
    ledcAttachPin(BUZZER, 1);
    ledcWriteNote(1, NOTE_C, 6);
    delay(100);
    ledcDetachPin(BUZZER);
    delay(50);
  }
}

void servo() {
  if (isOpen) {
    Serial.println("Opening box");
  } else {
    Serial.println("Closing box");
  }
}

void led() {
  if (isOpen) {
    Serial.println("Green led");
  } else {
    Serial.println("Blue led");
  }
}

void updateDisplay() {
  if (isOpen) {
    Serial.println("Opening animation");
  } else {
    Serial.println("Closing animation");
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Wifi setup
  Serial.print("Yellow led");
  Serial.println(WIFI_NAME);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  IPAddress ip = WiFi.localIP();
  Serial.println("Blue led");
  Serial.println(ip);

  // Display setup
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(110, 0);
  display.println("teste");
  // display.println(ip[3]);

  // GET endpoint
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    // Set open/closed state
    isOpen = !isOpen;
    digitalWrite(LED, isOpen);

    buzzer();
    servo();
    led();
    updateDisplay();

    request->send(200, "text/plain", "message received");
  });

  // POST display endpoint
  server.on("/", HTTP_POST, [](AsyncWebServerRequest* request) {
    int paramsNr = request->params();
    Serial.println(paramsNr);

    for (int i = 0; i < paramsNr; i++) {
      AsyncWebParameter* p = request->getParam(i);
      Serial.print("Param name: ");
      Serial.println(p->name());
      Serial.print("Param value: ");
      Serial.println(p->value());
      Serial.println("------");
    }

    // animate display

    // display.drawBitmap(0, 16, epd_bitmap_teste, 128, 64, WHITE);
    // display.display();

    request->send(200, "text/plain", "message received");
  });

  server.begin();
}

void loop() {
  unsigned long currentMillis = millis();

  // Tries reconnecting when Wifi fails
  if ((WiFi.status() != WL_CONNECTED) &&
      (currentMillis - previousMillis >= interval)) {
    Serial.print(millis());
    Serial.println("Yellow led");
    WiFi.disconnect();
    WiFi.reconnect();
    previousMillis = currentMillis;
  } else if (WiFi.status() == WL_CONNECTED) {
    // sets led to current state
  }
}
