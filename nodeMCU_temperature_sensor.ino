#include "configuration.h"
#include "form_filler.h"
#include "analogRead.h"

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

Adafruit_AHTX0 environment_sensor;
WiFiUDP ntp_udp;
NTPClient time_client(ntp_udp);


void connectToWiFi(void);

//Calculates sleep duration in microseconds
unsigned long calculateSleepTime(void);

void setup() {
  Serial.begin(115200);

  while (!Serial) {
    Serial.print("x");
  }
  Serial.println("System started");
  connectToWiFi();

  time_client.begin();
}

void loop() {
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {

    bool is_begin_successful = environment_sensor.begin();
    if (true == is_begin_successful)
    {
      float voltage = getSupplyVoltage();
      Serial.print("Voltage: ");
      Serial.println(voltage);

      sensors_event_t humidity, temp;
      bool is_successful = environment_sensor.getEvent(&humidity, &temp);

      if (true == is_successful)
      {
        Serial.print("Temperature: ");
        Serial.println(temp.temperature);
        Serial.print("Humidity: ");
        Serial.println(humidity.relative_humidity);

        FormFiller form_filler("entry.1308355757",
                               "entry.20675324",
                               "entry.851959826",
                               "entry.1563906843",
                               "entry.1884689726");

        form_filler.sendData(String(voltage), String(temp.temperature), String(humidity.relative_humidity), String());
      }
      else
      {
        Serial.println("Could not find a valid AHT10 sensor.");
      }
    }
    else
    {
      Serial.println("Could not find a valid AHT10 sensor.");
    }
  }
  else {
    Serial.println("WiFi Disconnected!");
    connectToWiFi();
  }

  unsigned long sleep_time_us = calculateSleepTime();

  Serial.print("Sleeping for ");
  Serial.print(sleep_time_us / (1e6));
  Serial.println(" seconds...");

  ESP.deepSleep(sleep_time_us);
}

void connectToWiFi(void)
{
  String ssid = (const char*) & (Configuration::Instance().getConfigData().wifi_ssid[0]);
  String password = (const char*) & (Configuration::Instance().getConfigData().wifi_password[0]);
  Serial.print("Connecting to wifi ");
  Serial.println(WiFi.SSID());
  wl_status_t wifi_status = WiFi.begin(ssid, password);
  Serial.print("Connection status:");
  Serial.println(wifi_status);
  while (WiFi.waitForConnectResult()  != WL_CONNECTED) {
    Serial.println(WiFi.status());
  }
  //todo handle error 4 and 6
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

unsigned long calculateSleepTime(void)
{
  const unsigned long microseconds_in_a_second = 1e6;
  const unsigned long configured_update_period_s = Configuration::Instance().getConfigData().update_interval_s;

  bool is_ntp_update_successful = time_client.forceUpdate();
  if (is_ntp_update_successful)
  {
    unsigned long sleep_time_s = configured_update_period_s - (time_client.getEpochTime() % configured_update_period_s);

    sleep_time_s = sleep_time_s * Configuration::Instance().getConfigData().sleep_correction_factor;

    return sleep_time_s * microseconds_in_a_second;
  }
  else
  {
    Serial.println("NTP time update failed, using default sleep period.");
    return configured_update_period_s * microseconds_in_a_second;
  }
}
