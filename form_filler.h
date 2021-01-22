#ifndef FORM_FILLER_H_
#define FORM_FILLER_H_

#include "WString.h"

class formFiller {
  public:
    //Creates form filled with correct url to google sheets
    //param_in form_id              id of the google form where data is pushed
    //param_in device_html_id       html element id for device ID text field
    //param_in voltage_html_id      html element id for voltage text field
    //param_in temperature_html_id  html element id for temperature text field
    //param_in humidity_html_id     html element id for humidity text field
    //param_in pressure_html_id     html element id for pressure text field
    formFiller(const String& form_id,
               const String& device_html_id,
               const String& voltage_html_id,
               const String& temperature_html_id,
               const String& humidity_html_id,
               const String& pressure_html_id);

    //sends arguments to google sheets
    void sendData(const String& voltage = "",
                  const String& temperature = "",
                  const String& humidity = "",
                  const String& pressure = "");

  private:
    const int device_id_EEPROM_address{0x00};
    const String server_name;
    const String device_html_id;
    const String voltage_html_id;
    const String temperature_html_id;
    const String humidity_html_id;
    const String pressure_html_id;
    
    String device_id;
};


#endif //FORM_FILLER_H_
