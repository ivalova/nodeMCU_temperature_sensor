#include "credentials.h"
#include "form_filler.h"

#include <ESP8266WiFi.h>

formFiller form_filler(google_form_id);

unsigned long lastTime = 0;
unsigned long timerDelay = 60000;


void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  if ((millis() - lastTime) > timerDelay) {

    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {

      form_filler.sendData(String(15.3));
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
