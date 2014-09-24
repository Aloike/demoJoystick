#include "GBRawData.h"

#include <QGridLayout>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>

#include "ui/common/GLed.h"


GBRawData::GBRawData(QWidget *argParent)
    :   QGroupBox( argParent )
    ,   m_layoutAxis( new QGridLayout )
    ,   m_layoutButtons( new QGridLayout )
{
    this->setTitle( tr( "Raw data" ) );

    this->create_layout();
}

/* ######################################################################### */
/* ######################################################################### */

void    GBRawData::create_layout()
{
    QGridLayout *layoutMain = new QGridLayout;
    this->setLayout( layoutMain );

    QVBoxLayout *layoutDisplay  = new QVBoxLayout;
    layoutDisplay->addLayout( this->m_layoutAxis, 90 );
    layoutDisplay->addItem( new QSpacerItem( 10, 10, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding ) );
    layoutDisplay->addLayout( this->m_layoutButtons, 10);


    layoutMain->addLayout( layoutDisplay, 0, 0, 2, 1 );
    // layoutMain->addWidget( this->m_te, 0, 1, 2, 1 );

}

void    GBRawData::on_joystickEvent(const CJoystickEvent &argEvent)
{
    if( argEvent.type() == CJoystickEvent::TypeAxis )
    {
        this->m_mapAxisValue.insert( argEvent.number(), argEvent.value() );
    }
    else if( argEvent.type() == CJoystickEvent::TypeButton )
    {
        this->m_mapButtonsState.insert( argEvent.number(), argEvent.value() );
    }

    this->update_ui();
}


/* ######################################################################### */
/* ######################################################################### */

void    GBRawData::update_ui()
{
    //-------------------------------------------------------------------------
    //  Update the axis
    //-------------------------------------------------------------------------
    foreach (int axisNbr, this->m_mapAxisValue.keys())
    {
        if( ! this->m_mapAxisIndicators.contains( axisNbr ) )
        {
            QProgressBar    *p_indicator    = new QProgressBar;
            this->m_mapAxisIndicators.insert( axisNbr, p_indicator );
            p_indicator->setRange( -32768, 32768 );

            this->m_layoutAxis->addWidget( new QLabel( "Axis " + QString::number( axisNbr ) ), axisNbr, 0, 1, 1 );
            this->m_layoutAxis->addWidget( p_indicator, axisNbr, 1, 1, 1 );
        }

        this->m_mapAxisIndicators.value( axisNbr )
                ->setValue( this->m_mapAxisValue.value( axisNbr ) );
    }


    //-------------------------------------------------------------------------
    //  Update the buttons
    //-------------------------------------------------------------------------
    foreach( int buttonNbr, this->m_mapButtonsState.keys() )
    {
//        int buttonNbr   = this->m_mapButtonsState.keys().at( i );

        int row = buttonNbr / 8;
        int col = buttonNbr % 8;

        if( this->m_layoutButtons->itemAtPosition( row, col ) == 0 )
        {
            /* The indicator doesn't exist, create it */
            QVBoxLayout *layoutIndicator    = new QVBoxLayout;
            GLed*   p_indicator = new GLed( this );
            this->m_mapButtonsIndicators.insert( buttonNbr, p_indicator );
            p_indicator->setColor( Qt::green );

            layoutIndicator->addWidget( p_indicator );
            layoutIndicator
                    ->addWidget( new QLabel( "Button "
                                             + QString::number( buttonNbr ) ) );

            this->m_layoutButtons->addLayout( layoutIndicator, row, col,1, 1 );
        }

        this->m_mapButtonsIndicators.value( buttonNbr )
                ->setActivated( this->m_mapButtonsState.value( buttonNbr ) );
    }
}

/* ######################################################################### */
/* ######################################################################### */
