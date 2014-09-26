#include "MainWindow.h"

#include <QFile>
#include <QInputDialog>
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>

#include "core/data/CDataMapper.h"
#include "core/input/joystick/CJoystick.h"

#include "GBGraphFactorAnalyzer.h"
#include "GBMappedData.h"
#include "GBRawData.h"

#define DBG(format,...) qDebug( "%s ==>"format, __PRETTY_FUNCTION__, ##__VA_ARGS__ )
#define ERR(format,...) qWarning( "%s ==>"format, __PRETTY_FUNCTION__, ##__VA_ARGS__ )

namespace Tags
{
    static const QString JoystickElement    = "joystick";
    namespace Joystick
    {
        static const QString DeviceElement      = "device";
    }
}


MainWindow::MainWindow(QWidget *parent)
    :   QMainWindow(parent)
    ,   CDataMapperListener()
    ,   CJoystickListener()
    ,   m_actionJoystickClose( 0 )
    ,   m_actionJoystickOpen( 0 )
    ,   m_dataMapper( new CDataMapper )
    ,   m_gbFactorAnalyzer( new GBGraphFactorAnalyzer( this ) )
    ,   m_gbMappedData( new GBMappedData( this ) )
    ,   m_gbRawData( new GBRawData( this ) )
    ,   m_joystick( new CJoystick )
{
    this->create_layout();
    this->create_menus();

    this->m_joystick->listenerAdd( this );
    this->m_dataMapper->listenerAdd( this );
    this->m_dataMapper->listenerAdd( this->m_gbMappedData );



    this->configLoadFromFile( "conf_js.xml" );
}

MainWindow::~MainWindow()
{
    if( this->m_joystick->isOpen() ) {
        this->m_joystick->closeJs();
    }

    this->m_dataMapper->listenerRemove( this );
    this->m_joystick->listenerRemove( this );

    delete m_dataMapper;
}

bool    MainWindow::configLoadFromFile(const QString &argFileName)
{
#if 0
    if( this->m_dataMapper->loadFromFile(  ) )  {
        qDebug( "dataMapper file load OK" );
    } else {
        qWarning( "dataMapper file load ERROR" );
    }
#endif

    QFile file( argFileName );
    if( ! file.open( QFile::ReadOnly ) ) {
        DBG( "Can't open file" );
        return false;
    }

    QDomDocument domDoc;
    QString errorString;
    int errorColumn = -1;
    int errorLine   = -1;
    if( ! domDoc.setContent( &file, &errorString, &errorLine, &errorColumn ) ) {
        ERR( "Error while loading content : %s",
                  errorString.toStdString().c_str() );
        ERR( "Line     == %d", errorLine );
        ERR( "Column   == %d", errorColumn );
        file.close();
        return false;
    }
    file.close();


    QDomElement joystickElmt
            = domDoc.firstChildElement( Tags::JoystickElement );
    if( joystickElmt.isNull() ) {
        DBG( "No joystick element found in config file" );
        return false;
    }


    QDomElement deviceElmt
            = joystickElmt.firstChildElement( Tags::Joystick::DeviceElement );
    if( deviceElmt.isNull() ) {
        DBG( "No device set in config file" );
    } else {
        this->openJoystick( deviceElmt.text() );
    }

    QDomElement listElmt
            = joystickElmt.firstChildElement( CDataMapper::elementName() );
    if( listElmt.isNull() ) {
        DBG( "No map list set in config file" );
    } else {
        if( ! this->m_dataMapper->loadFromDomElement( listElmt ) ) {
            return false;
        }
    }

    return true;
}

void    MainWindow::create_layout()
{
    QVBoxLayout *layoutMain = new QVBoxLayout;

    layoutMain->addWidget( this->m_gbRawData );
    layoutMain->addWidget( this->m_gbMappedData );
    layoutMain->addWidget( this->m_gbFactorAnalyzer );

    this->setCentralWidget( new QWidget( this ) );
    this->centralWidget()->setLayout( layoutMain );
}


void    MainWindow::create_menus()
{
    QMenu   *p_menuJoystick = this->menuBar()->addMenu( tr( "Joystick" ) );

    m_actionJoystickOpen
            = p_menuJoystick->addAction( tr( "Open..." ),
                                         this, SLOT(on_m_actionJoystickOpen_triggered()) );
    m_actionJoystickOpen->setShortcut( QKeySequence("Ctrl+O" ) );

    m_actionJoystickClose
            = p_menuJoystick->addAction( tr( "Close" ),
                                         this, SLOT(on_m_actionJoystickClose_triggered()) );
    m_actionJoystickClose->setEnabled( false );
}

void MainWindow::on_CDataMapper_output( const QString   &argKeyIn,
                                        const QVariant  &argDataIn,
                                        const QString   &argKeyOut,
                                        const QVariant  &argDataOut )
{
    Q_UNUSED( argKeyIn )

    if( argKeyOut == "factor_analyzer" )
    {
        bool    ok  = false;
        double  x   = 0;
        double  y   = 0;

        x   = argDataIn.toDouble( &ok );
        if( ! ok ) {
            DBG( "Ne parvient pas à convertir argDataIn (`%s`)",
                 argDataIn.toString().toStdString().c_str() );
            return;
        }

        y   = argDataOut.toDouble( &ok );
        if( ! ok ) {
            DBG( "Ne parvient pas à convertir argDataOut (`%s`)",
                 argDataOut.toString().toStdString().c_str() );
            return;
        }


        this->m_gbFactorAnalyzer->addPoint( x, y );
    }

    // qDebug( "Value on argKeyOut `%s`", argKeyOut.toStdString().c_str() );
}

void MainWindow::on_joystickEvent(const CJoystickEvent &argEvent)
{
    //--------------------------------------------------------------------------
    //  Send the raw data to widgets who need it
    //--------------------------------------------------------------------------
    this->m_gbRawData->on_joystickEvent( argEvent );


    //--------------------------------------------------------------------------
    //  Create the "input key" of the event's value
    //--------------------------------------------------------------------------

    QString lKeyIn;
    switch( argEvent.type() )
    {
        case CJoystickEvent::TypeAxis:
            lKeyIn  += "axis";
            break;


        case CJoystickEvent::TypeButton:
            lKeyIn  += "button";
            break;


        case CJoystickEvent::TypeUnknown:
            lKeyIn  += "unknown";
            break;


        default:
            lKeyIn  += "__default_";
            break;
    }

    lKeyIn  +=  QString::number( argEvent.number() );


    //--------------------------------------------------------------------------
    //  Map the data to its corresponding command through the dataMapper
    //--------------------------------------------------------------------------
    this->m_dataMapper->transmitData( lKeyIn, argEvent.value() );
}

void MainWindow::on_m_actionJoystickClose_triggered()
{
    qDebug( "Closing joystick" );

    if( this->m_joystick->isOpen() ) {
        m_joystick->closeJs();
    }

    m_actionJoystickClose->setEnabled( m_joystick->isOpen() );
    m_actionJoystickOpen->setEnabled( ! m_joystick->isOpen() );

    if( ! m_joystick->isOpen() )
        this->statusBar()->showMessage( tr( "Not connected" ) );
}

void MainWindow::on_m_actionJoystickOpen_triggered()
{
    QStringList entryList   = CJoystick::available();

    bool ok;
    QString text = QInputDialog::getItem( this,
                                          tr( "Open Joystick" ),
                                          tr( "Joystick name:" ),
                                          entryList,
                                          0, false, &ok );
    if (ok && !text.isEmpty()) {
        this->openJoystick( text );
    }
}

bool    MainWindow::openJoystick(const QString &argDevice)
{
    bool    ret = true;
    DBG( "Selected joystick device : `%s`", argDevice.toStdString().c_str() );

    if( ! this->m_joystick->openJs( argDevice.toStdString() ) )
    {
        ERR( "Can't open joystick !" );
        ret = false;
    }
    else
    {
        this->statusBar()->showMessage( tr( "Connected to `%1`" )
                                        .arg( argDevice ) );
        ret = true;
    }
    m_actionJoystickClose->setEnabled( m_joystick->isOpen() );
    m_actionJoystickOpen->setEnabled( ! m_joystick->isOpen() );

    return ret;
}
