#ifndef CJOYSTICKEVENT_H
#define CJOYSTICKEVENT_H

#include <linux/joystick.h>


/**
 * @brief The CJoystickEvent class holds a joystick event's data.
 *
 * It is initialized from a js_event given to the \ref decode method.
 * Retrieved data is:
 *  + \c Type : To know if this is an axis or button event;
 *  + \c number : This is the ID of the button/axis for which the value applies;
 *  + \c value : the actual value of the event.
 */
class CJoystickEvent
{

public:

    enum    Type
    {
        TypeUnknown = 0x00,

        TypeButton  = JS_EVENT_BUTTON,
        TypeAxis    = JS_EVENT_AXIS
    };


    CJoystickEvent();


    void    decode( const struct js_event argEvent );


    bool    isInit(void) const;

    Type    type(void) const;

    int     number(void) const;

    int16_t value(void) const;



private:

    bool    m_isInit;
    int     m_number;
    Type    m_type;
    int16_t m_value;

};

#endif // CJOYSTICKEVENT_H
