#ifndef FORM_FILLER_H_
#define FORM_FILLER_H_

#include "WString.h"

class formFiller {
  public:
    //Creates form filled with correct url to google sheets
    //param_in form_id id of the google form where data is pushed
    formFiller(const String& form_id);

    void sendData(const String temperature = "" , const String humidity = "" , const String pressure = "");

  private:
    const String serverName;
};


#endif //FORM_FILLER_H_
