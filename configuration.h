#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <Arduino.h>

struct ConfigurationData
{
  uint8_t   config_key_start;
  uint8_t   device_id;
  uint16_t  update_interval_s;
  int8_t   wifi_password[64];
  int8_t   google_form_id[64];
  int8_t   wifi_ssid[32];
  uint32_t  crc;
} __attribute__((packed));

class Configuration {
  public:
    static Configuration& Instance(void);

    ConfigurationData getConfigData();

  private:
    Configuration();

    ConfigurationData configuration_data;
};

#endif //CONFIGURATION_H_
