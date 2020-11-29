#include "form_filler.h"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <HardwareSerial.h>

formFiller::formFiller(const String& form_id,
                       const String& temperature_id,
                       const String& humidity_id,
                       const String& pressure_id)
  : server_name("https://docs.google.com/forms/d/e/" + form_id + "/formResponse"),
    temperature_id(temperature_id),
    humidity_id(humidity_id),
    pressure_id(pressure_id)
{
}

void formFiller::sendData(const String& temperature, const String& humidity, const String& pressure)
{
  HTTPClient http;
  WiFiClientSecure client;

  Serial.println("Sending data to: " + server_name);

  client.setInsecure();
  auto resp =  client.connect(server_name.c_str(), 443);
  http.begin(client, server_name.c_str());

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String body_data = temperature_id + "=" +  temperature + "&" + \
                     humidity_id + "=" +  humidity + "&" +
                     pressure_id + "=" +  pressure + "&";

  Serial.println("data:" + body_data);

  resp = http.POST(body_data);

  Serial.print("post response:");
  Serial.print((resp));
  Serial.println(http.errorToString(resp));

  http.end();
}
