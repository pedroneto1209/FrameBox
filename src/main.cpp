#include <ESPAsyncWebServer.h>
#include <WiFi.h>

#define LED 2
#define WIFI_NAME "NET.AP 901 2Ghz"
#define WIFI_PASSWORD "31415926"

unsigned long previousMillis = 0;
unsigned long interval = 30000;
bool state = false;

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);

  Serial.print("Connecting to ");
  Serial.println(WIFI_NAME);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("Connected!");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    state = !state;
    digitalWrite(LED, state);
    request->send(200, "text/plain", "message received");
  });

  server.begin();
}

void loop() {
  unsigned long currentMillis = millis();

  if ((WiFi.status() != WL_CONNECTED) &&
      (currentMillis - previousMillis >= interval)) {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    previousMillis = currentMillis;
  }
}
