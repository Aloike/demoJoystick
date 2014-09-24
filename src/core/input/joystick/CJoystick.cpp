#include "CJoystick.h"

#include <fcntl.h>
#include <linux/joystick.h>
#include <unistd.h>

#include <QDir>
#include <QProcess>
#include <QTimer>

#include "CJoystickEvent.h"


#define ERR(format, ...) fprintf( stderr, "ERR---::%s @%d [%s] ==>"format, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__ )

/* ######################################################################### */
/* ######################################################################### */

CJoystick::CJoystick()
    :   QObject()
    ,   m_jsHandler( 0 )
    ,   m_timerReadPending( new QTimer( this) )
{
    this->m_timerReadPending->setInterval( 50 );
    this->m_timerReadPending->setSingleShot( false );
    this->m_timerReadPending->stop();

    connect( this->m_timerReadPending, SIGNAL(timeout()),
             this, SLOT(on_m_timerReadPending_timeout()) );
}

/* ######################################################################### */
/* ######################################################################### */

CJoystick::~CJoystick()
{
    if( this->isOpen() ) {
        this->closeJs();
    }
}

/* ######################################################################### */
/* ######################################################################### */

QStringList CJoystick::available()
{
    QStringList ret;

    QProcess    process;
    process.start( "bash -c \"ls /dev/input/js*\"" /*, args*/ );
    process.waitForFinished(10000);
    while( process.canReadLine() )
    {
        QString line    = process.readLine();
        line.remove( "\n" );
        line.remove( "\r" );
        ret.append( line );
    }
#if 0
    QDir dir( "/dev/input/by-id" );
    dir.setFilter( QDir::NoFilter );
    qDebug( "dirName == %s", dir.path().toAscii().constData() );

    QStringList list;
    list.append( "*-joystick" );
    ret = dir.entryList( list );
#endif
    /*foreach( QString str, ret ) {
        qDebug( "%s", str.toStdString().c_str() );
    }*/

    return ret;
}

/* ######################################################################### */
/* ######################################################################### */

bool    CJoystick::closeJs()
{
    bool    ret = false;

    if( this->m_jsHandler != 0 )
    {
        this->m_timerReadPending->stop();

        if( close( this->m_jsHandler ) == 0 )
        {
            this->m_jsHandler   = 0;
            ret = true;
        }
        else
        {
            ERR( "Error while closing handler" );
            ret = false;
        }
    }
    else
    {
        ret = false;
    }

    return ret;
}

/* ######################################################################### */
/* ######################################################################### */

bool    CJoystick::isOpen() const
{
    if( this->m_jsHandler != 0 ) {
        return true;
    } else {
        return false;
    }
}

/* ######################################################################### */
/* ######################################################################### */

void    CJoystick::listenerAdd(CJoystickListener *argListener)
{
    if( argListener == 0 ) {
        return;
    }


    //-------------------------------------------------------------------------
    //  Check that the listener ain't already registered
    //-------------------------------------------------------------------------
    for( size_t i = 0 ; i < this->m_listeners.size() ; ++i )
    {
        if( this->m_listeners.at( i ) == argListener ) {
            /* The listeners list already contains the one we're trying to
             * add */
            return;
        }
    }


    //-------------------------------------------------------------------------
    //  Add \p argListener to the list
    //-------------------------------------------------------------------------
    this->m_listeners.push_back( argListener );
}

/* ######################################################################### */
/* ######################################################################### */

void    CJoystick::listenerRemove(CJoystickListener *argListener)
{
    if( argListener == 0 ) {
        return;
    }

    for (std::vector<CJoystickListener*>::iterator it = this->m_listeners.begin() ;
         it != this->m_listeners.end(); ++it)
    {
        if( *it == argListener ) {
            this->m_listeners.erase( it );
            return;
        }
    }

    return;
}

/* ######################################################################### */
/* ######################################################################### */

void    CJoystick::on_m_timerReadPending_timeout()
{
    this->readPendingEvents();
}

/* ######################################################################### */
/* ######################################################################### */

bool    CJoystick::openJs(const std::string argName)
{
    //-------------------------------------------------------------------------
    //  If a joystick has been opened, close it
    //-------------------------------------------------------------------------
    if( this->m_jsHandler != 0 )
    {
        if( this->closeJs() != true )
        {
            ERR( "Error while closing handler" );
            return false;
        }
    }


    //-------------------------------------------------------------------------
    //  Open the given joystick
    //-------------------------------------------------------------------------
    this->m_jsHandler   = open( argName.c_str(), O_RDONLY | O_NONBLOCK );
    if( this->m_jsHandler == -1 )
    {
        this->m_jsHandler   = 0;
        ERR( "error while opening handler !" );
        return false;
    }

    this->m_timerReadPending->start();

    return true;
}

/* ######################################################################### */
/* ######################################################################### */

void    CJoystick::readPendingEvents()
{
    if( this->m_jsHandler == 0 )
    {
        return;
    }

    struct js_event e_joystick;


    //-------------------------------------------------------------------------
    //  Read the pending joystick events
    //-------------------------------------------------------------------------
//    ssize_t readSize    = 0;
    while( read( this->m_jsHandler, &e_joystick, sizeof(e_joystick)) > 0 )
    {
//        if( readSize <= 0 ) {
//            continue;
//        }

//        qDebug( "%s : reading pending event", __PRETTY_FUNCTION__ );

        CJoystickEvent jsEvent;
        jsEvent.decode( e_joystick );

        for(size_t i = 0 ; i < this->m_listeners.size() ; ++i )
        {
            (*this->m_listeners.at( i )).on_joystickEvent( jsEvent );
        }
    }
}

/* ######################################################################### */
/* ######################################################################### */

void    CJoystick::setUpdateInterval(int argMs)
{
    this->m_timerReadPending->setInterval( argMs );
}

/* ######################################################################### */
/* ######################################################################### */
