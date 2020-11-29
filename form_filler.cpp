#include "form_filler.h"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <HardwareSerial.h>

formFiller::formFiller(const String& form_id)
  : serverName("https://docs.google.com/forms/d/e/" + form_id + "/formResponse")
{
}

void formFiller::sendData(const String temperature, const String humidity, const String pressure)
{
  HTTPClient http;
  WiFiClientSecure client;

  Serial.println("Sending data to: " + serverName);

  client.setInsecure();
  auto resp =  client.connect(serverName.c_str(), 443);
  http.begin(client, serverName.c_str());

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  resp = http.POST("entry.20675324=55");

  Serial.print("post response:");
  Serial.print((resp));
  Serial.println(http.errorToString(resp));

  http.end();
}
