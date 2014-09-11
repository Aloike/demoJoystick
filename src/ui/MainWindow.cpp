#include "MainWindow.h"

#include <QInputDialog>
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>

#include "core/data/CDataMapper.h"
#include "core/input/joystick/CJoystick.h"

#include "GBMappedData.h"
#include "GBRawData.h"


MainWindow::MainWindow(QWidget *parent)
    :   QMainWindow(parent)
    ,   CDataMapperListener()
    ,   CJoystickListener()
    ,   m_actionJoystickClose( 0 )
    ,   m_actionJoystickOpen( 0 )
    ,   m_dataMapper( new CDataMapper )
    ,   m_gbMappedData( new GBMappedData( this ) )
    ,   m_gbRawData( new GBRawData( this ) )
    ,   m_joystick( new CJoystick )
{
    this->create_layout();
    this->create_menus();

    this->m_joystick->listenerAdd( this );
    this->m_dataMapper->listenerAdd( this );
    this->m_dataMapper->listenerAdd( this->m_gbMappedData );

    if( this->m_dataMapper->loadFromFile( "conf_js.xml" ) )  {
        qDebug( "dataMapper file load OK" );
    } else {
        qWarning( "dataMapper file load ERROR" );
    }
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

void    MainWindow::create_layout()
{
    QVBoxLayout *layoutMain = new QVBoxLayout;

    layoutMain->addWidget( this->m_gbRawData );
    layoutMain->addWidget( this->m_gbMappedData );

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

void MainWindow::on_CDataMapper_output( const QString &argKeyOut,
                                        const QVariant &argData )
{
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
        qDebug( "Selected joystick : `%s`", text.toStdString().c_str() );

        if( ! this->m_joystick->openJs( text.toStdString() ) )
        {
            qWarning( "Can't open joystick !" );
        }
        else
        {
            this->statusBar()->showMessage( tr( "Connected to `%1`" )
                                            .arg( text ) );
        }
        m_actionJoystickClose->setEnabled( m_joystick->isOpen() );
        m_actionJoystickOpen->setEnabled( ! m_joystick->isOpen() );
    }
}
