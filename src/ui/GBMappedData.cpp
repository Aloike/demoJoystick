#include "GBMappedData.h"

#include <QHBoxLayout>

#include "ui/common/AxesTester.h"


GBMappedData::GBMappedData(QWidget *parent)
    :   QGroupBox(parent)
    ,   m_atPosition( new AxesTester( this ) )
{
    this->setTitle( tr( "Mapped data" ) );

    this->setLayout( new QHBoxLayout );
    this->layout()->addWidget( this->m_atPosition );

    this->m_atPosition->setAxeY( 16000 );
}

void GBMappedData::on_CDataMapper_output(const QString &argKeyOut, const QVariant &argData)
{
    if( argKeyOut == "pan" )
    {
        this->m_atPosition->setAxeX( argData.toInt() );
    }
    else if( argKeyOut == "tilt" )
    {
        this->m_atPosition->setAxeY( argData.toInt() );
    }
}
