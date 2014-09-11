#ifndef CJOYSTICK_H
#define CJOYSTICK_H

#include <QObject>

#include <stdio.h>
#include <string>
#include <vector>

class QTimer;

#include "CJoystickListener.h"


class CJoystick : public QObject
{

    Q_OBJECT

public:

    static QStringList  available(void);

    CJoystick();
    ~CJoystick();

    void    listenerAdd( CJoystickListener* argListener );
    void    listenerRemove( CJoystickListener* argListener );

    bool    openJs( const std::string argName );
    bool    closeJs();
    bool    isOpen(void) const;

    void    setUpdateInterval( int argMs );



private:

    int                             m_jsHandler;
    std::vector<CJoystickListener*> m_listeners;
    QTimer                          *m_timerReadPending;

    void    readPendingEvents();



private slots:

    void    on_m_timerReadPending_timeout();



};

#endif // CJOYSTICK_H
