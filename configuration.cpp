#include "configuration.h"

#include <EEPROM.h>
#include <CRC32.h>
#include "WString.h"

#define EEPROM_MAGIC_BYTES (0xAA)
#define EEPROM_START_ADDRESS (0x00)

Configuration& Configuration::Instance(void)
{
  static Configuration instance;

  return instance;
}

Configuration::Configuration() 
{
  ConfigurationData configuration;

  EEPROM.begin(sizeof(configuration));
  EEPROM.get(EEPROM_START_ADDRESS, configuration);

  uint32_t calculated_crc = CRC32::calculate((const uint8_t*)&configuration, sizeof(configuration) - sizeof(configuration.crc));

  if(calculated_crc == configuration.crc) 
  {
    if(EEPROM_MAGIC_BYTES != configuration.config_magic_bytes)
    {
       Serial.println("Configuration magic bytes do not match. Configuration might be incorrect.");
    }
    configuration_data = configuration;
  }
  else
  {
    Serial.println("Configuration CRC check failed. Calculated CRC: " + String(calculated_crc) + "Configured CRC: " + configuration.crc);
    Serial.println("Using default configuration.");

    ConfigurationData default_configuration{0};

    default_configuration.update_interval_s = 600;
    default_configuration.sleep_correction_factor  = 1.0;
    
    configuration_data = default_configuration;
  }
}

ConfigurationData Configuration::getConfigData(void)
{
  return configuration_data;
}
