#include "credentials.h"
#include "form_filler.h"
#include "analogRead.h"

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

const uint8_t sensor_address{0x38}; //aht10 address
const unsigned long timer_period_s{600};


formFiller form_filler(google_form_id,
                       "entry.1308355757",
                       "entry.20675324",
                       "entry.851959826",
                       "entry.1563906843",
                       "entry.1884689726");
Adafruit_AHTX0 environment_sensor;
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

    bool is_begin_successful = environment_sensor.begin();
    if (true == is_begin_successful)
    {
      float voltage = getSupplyVoltage();
      Serial.print("Voltage: ");
      Serial.println(voltage);

      sensors_event_t humidity, temp;
      bool is_successful = environment_sensor.getEvent(&humidity, &temp);

      if(true == is_successful)
      {
        Serial.print("Temperature: ");
        Serial.println(temp.temperature);
        Serial.print("Humidity: ");
        Serial.println(humidity.relative_humidity);
  
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
