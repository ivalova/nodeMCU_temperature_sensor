#include "credentials.h"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const std::string serverName = "https://docs.google.com/forms/d/e/" + google_form_id + "/formResponse";


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

      HTTPClient http;
      WiFiClientSecure client;
      
      client.setInsecure();
      auto resp =  client.connect(serverName.c_str(), 443);
      http.begin(client, serverName.c_str());

      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      resp = http.POST("entry.20675324=11");

      Serial.print("post response:");
      Serial.print((resp));
      Serial.println(http.errorToString(resp));

      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
