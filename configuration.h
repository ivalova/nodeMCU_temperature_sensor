#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <Arduino.h>

//Structure for data that is located in EEPROM
//param_in config_key_start         magic cookie to verify that expected structure is read from EEPROM
//param_in device_id                unique ID for this device
//param_in update_interval_s        time period in seconds for reporting measurements
//param_in sleep_correction_factor  factor used to compensate for RTC clock drift
//param_in wifi_password            password for wifi acccess point
//param_in google_form_id           unique google form id where data is sent
//param_in wifi_ssid                name of the wifi access point
//param_in crc                      crc used to verify integrity of data

struct ConfigurationData
{
  uint8_t   config_magic_bytes;
  uint8_t   device_id;
  uint16_t  update_interval_s;
  float     sleep_correction_factor;
  int8_t    wifi_password[64];
  int8_t    google_form_id[64];
  int8_t    wifi_ssid[32];
  uint32_t  crc;
} __attribute__((packed));

class Configuration {
  public:
    static Configuration& Instance(void);

    //returns configuration data from EEPROM
    ConfigurationData getConfigData(void);

  private:
    Configuration();

    ConfigurationData configuration_data;
};

#endif //CONFIGURATION_H_
