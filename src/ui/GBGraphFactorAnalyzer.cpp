#include "GBGraphFactorAnalyzer.h"

#include <QHBoxLayout>

#include "ui/common/qcustomplot.h"


#define DBG(format,...) qDebug( "%s ==>"format, __PRETTY_FUNCTION__, ##__VA_ARGS__ )
#define ERR(format,...) qWarning( "%s ==>"format, __PRETTY_FUNCTION__, ##__VA_ARGS__ )


GBGraphFactorAnalyzer::GBGraphFactorAnalyzer(QWidget *parent)
    :   QGroupBox(parent)
    ,   m_graph( new QCustomPlot( this ) )
{
    this->setTitle( tr( "Factor analyzer" ) );
    this->setMinimumHeight( 200 );

    this->m_graph->addGraph();


    this->create_layout();
}

/* ########################################################################## */
/* ########################################################################## */

void    GBGraphFactorAnalyzer::addPoint(const double &argX, const double &argY)
{
    DBG( "Entering function (argX==%lf|argY==%lf)", argX, argY );
    QCPGraph    *p_graph    = m_graph->graph();
    Q_CHECK_PTR( p_graph );

    p_graph->addData( argX, argY );
    m_graph->replot();
    m_graph->rescaleAxes();
}

/* ########################################################################## */
/* ########################################################################## */

void    GBGraphFactorAnalyzer::create_layout()
{
    QHBoxLayout *p_layoutMain   = new QHBoxLayout;

    this->setLayout( p_layoutMain );

    p_layoutMain->addWidget( this->m_graph );
}

/* ########################################################################## */
/* ########################################################################## */
