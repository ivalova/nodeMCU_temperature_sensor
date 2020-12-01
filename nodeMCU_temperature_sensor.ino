#include "credentials.h"
#include "form_filler.h"
#include "analogRead.h"

#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

formFiller form_filler(google_form_id,
                       "entry.851959826",
                       "entry.1563906843",
                       "entry.1884689726");

const uint8_t temperature_sensor_pin{13};
OneWire one_wire(temperature_sensor_pin);
DallasTemperature temperature_sensor(&one_wire);

unsigned long last_time = 0;
unsigned long timer_delay = 300000;

void connectToWiFi(void);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  connectToWiFi();
}

void loop() {

  if ((millis() - last_time) > timer_delay) {

    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {

      temperature_sensor.begin();
      uint8_t device_count = temperature_sensor.getDS18Count();
      if (1 == device_count)
      {
        float voltage = getSupplyVoltage();
        Serial.println(voltage);

        temperature_sensor.requestTemperatures();
        float temperature = temperature_sensor.getTempCByIndex(0);
        Serial.println(temperature);
        form_filler.sendData(String(temperature));
      }
      else
      {
        Serial.println("No 1DS8B20 sensor detected.");
      }
    }
    else {
      Serial.println("WiFi Disconnected");
      connectToWiFi();
    }
    last_time = millis();
  }
}

void connectToWiFi(void)
{
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP Address: ");
  Serial.println(WiFi.localIP());
}
