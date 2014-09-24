#ifndef GBRAWDATA_H
#define GBRAWDATA_H

#include <QGroupBox>
#include "core/input/joystick/CJoystickListener.h"

#include <QMap>

class QGridLayout;
class QProgressBar;

class GLed;


class GBRawData :   public QGroupBox
                ,   public CJoystickListener
{
    Q_OBJECT

public:

    explicit GBRawData(QWidget *argParent = 0);


    virtual void    on_joystickEvent(const CJoystickEvent &);



private:

    void    create_layout(void);

    void    update_ui(void);



private:

    QGridLayout             *m_layoutAxis;
    QGridLayout             *m_layoutButtons;
    QMap<int,int16_t>       m_mapAxisValue;
    QMap<int,QProgressBar*> m_mapAxisIndicators;
    QMap<int,GLed*>         m_mapButtonsIndicators;
    QMap<int,bool>          m_mapButtonsState;


};

#endif // GBRAWDATA_H
