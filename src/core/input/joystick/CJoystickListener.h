#ifndef CJOYSTICKLISTENER_H
#define CJOYSTICKLISTENER_H

#include "CJoystickEvent.h"


class CJoystickListener
{

public:

    CJoystickListener();


    virtual void    on_joystickEvent(const CJoystickEvent&) {}

};

#endif // CJOYSTICKLISTENER_H
