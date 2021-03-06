#include "form_filler.h"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <HardwareSerial.h>
#include <EEPROM.h>

formFiller::formFiller(const String& form_id,
                       const String& device_html_id,
                       const String& voltage_html_id,
                       const String& temperature_html_id,
                       const String& humidity_html_id,
                       const String& pressure_html_id)
  : server_name("https://docs.google.com/forms/d/e/" + form_id + "/formResponse"),
    device_html_id(device_html_id),
    voltage_html_id(voltage_html_id),
    temperature_html_id(temperature_html_id),
    humidity_html_id(humidity_html_id),
    pressure_html_id(pressure_html_id)
{
  EEPROM.begin(1);  
  device_id = String(EEPROM.read(device_id_EEPROM_address));
}

void formFiller::sendData(const String& voltage,
                          const String& temperature,
                          const String& humidity,
                          const String& pressure)
{    
  HTTPClient http;
  WiFiClientSecure client;

  Serial.println("Connecting to: " + server_name);

  client.setInsecure();
  auto resp =  client.connect(server_name.c_str(), 443);

  Serial.print("SSL response: ");
  Serial.println(resp);

  http.begin(client, server_name.c_str());

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String body_data = device_html_id + "=" +  device_id + "&" + \
                     voltage_html_id + "=" +  voltage + "&" + \
                     temperature_html_id + "=" +  temperature + "&" + \
                     humidity_html_id + "=" +  humidity + "&" +
                     pressure_html_id + "=" +  pressure;

  Serial.println("data :" + body_data);

  resp = http.POST(body_data);

  if (200 == resp)
  {
    Serial.println("Data successfuly sent!");
  }
  else
  {
    Serial.print("Error sending data to the cloud! Response: ");
    Serial.println((resp));
    Serial.println(http.errorToString(resp));
  }


  http.end();
}
