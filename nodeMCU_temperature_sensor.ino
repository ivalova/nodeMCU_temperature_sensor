#include "credentials.h"
#include "form_filler.h"
#include "analogRead.h"

#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

unsigned long timer_period_s{120};
const uint8_t temperature_sensor_pin{D7};

formFiller form_filler(google_form_id,
                       "entry.20675324",
                       "entry.851959826",
                       "entry.1563906843",
                       "entry.1884689726");

OneWire one_wire(temperature_sensor_pin);
DallasTemperature temperature_sensor(&one_wire);
WiFiUDP ntp_udp;
NTPClient time_client(ntp_udp);


void connectToWiFi(void);

void setup() {
  Serial.begin(115200);

  connectToWiFi();

  time_client.begin();
}

void loop() {
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {

    temperature_sensor.begin();
    uint8_t device_count = temperature_sensor.getDS18Count();
    if (1 == device_count)
    {
      float voltage = getSupplyVoltage();
      Serial.print("Voltage: ");
      Serial.println(voltage);

      temperature_sensor.requestTemperatures();
      float temperature = temperature_sensor.getTempCByIndex(0);
      Serial.print("Temperature: ");
      Serial.println(temperature);
      form_filler.sendData(String(voltage),String(temperature));
    }
    else
    {
      Serial.println("No 1DS8B20 sensor detected.");
    }
  }
  else {
    Serial.println("WiFi Disconnected!");
    connectToWiFi();
  }

  time_client.forceUpdate();//todo evaluate retval
  Serial.print("Time: ");
  Serial.println(time_client.getFormattedTime());
  unsigned long sleep_time_s = timer_period_s - (time_client.getEpochTime() % timer_period_s);

  Serial.print("Sleeping for ");
  Serial.print(sleep_time_s);
  Serial.println(" seconds...");

  ESP.deepSleep(sleep_time_s * 1e6); //deepSleep receives us
}

void connectToWiFi(void)
{
  WiFi.begin(ssid, password);
  Serial.print("Connecting to wifi ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
