#ifndef FORM_FILLER_H_
#define FORM_FILLER_H_

#include "WString.h"

class formFiller {
  public:
    //Creates form filled with correct url to google sheets
    //param_in form_id              id of the google form where data is pushed
    //param_in temperature_id   html element id for temperature text field
    //param_in humidity_id id   html element id for humidity text field
    //param_in pressure_id id   html element id for pressure text field
    formFiller(const String& form_id,
               const String& temperature_id,
               const String& humidity_id,
               const String& pressure_id);

    void sendData(const String& temperature = "",
                  const String& humidity = "",
                  const String& pressure = "");

  private:
    const String server_name;
    const String temperature_id;
    const String humidity_id;
    const String pressure_id;
};


#endif //FORM_FILLER_H_
