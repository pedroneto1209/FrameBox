#include <ESPAsyncWebServer.h>
#include <WiFi.h>

#define LED 2
#define WIFI_NAME "NET.AP 901 2Ghz"
#define WIFI_PASSWORD "31415926"

unsigned long previousMillis = 0;
unsigned long interval = 30000;
bool isOpen = false;

AsyncWebServer server(80);

void buzzer() {
  if (isOpen) {
    Serial.println("Opening sound");
  } else {
    Serial.println("Closing sound");
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

void display() {
  if (isOpen) {
    Serial.println("Opening animation");
  } else {
    Serial.println("Closing animation");
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);

  // Wifi setup
  Serial.print("Yellow led");
  Serial.println(WIFI_NAME);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("Blue led");
  Serial.println(WiFi.localIP());

  // GET endpoint
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    // Set open/closed state
    isOpen = !isOpen;
    digitalWrite(LED, isOpen);

    buzzer();
    servo();
    led();
    display();

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
