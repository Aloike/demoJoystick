#ifndef GBGRAPHFACTORANALYZER_H
#define GBGRAPHFACTORANALYZER_H

#include <QGroupBox>


class QCustomPlot;


class GBGraphFactorAnalyzer : public QGroupBox
{
    Q_OBJECT

public:

    explicit GBGraphFactorAnalyzer(QWidget *parent = 0);

    void    addPoint( const double &argX, const double &argY );
    

protected:
private:

    void    create_layout(void);



public:
protected:
private:

    QCustomPlot *m_graph;
    
};

#endif // GBGRAPHFACTORANALYZER_H
