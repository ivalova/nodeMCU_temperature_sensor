#include "configuration.h"

#include <EEPROM.h>
#include "WString.h"

#define EEPROM_CONFIG_OK (0xAA)
#define EEPROM_START_ADDRESS (0x00)

Configuration& Configuration::Instance(void)
{
  static Configuration instance;

  return instance;
}

Configuration::Configuration() 
{
  EEPROM.begin(sizeof(ConfigurationData));
  EEPROM.get(EEPROM_START_ADDRESS, configuration_data);
}

ConfigurationData Configuration::getConfigData(void)
{
  return configuration_data;
}
