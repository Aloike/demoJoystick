#ifndef GBMAPPEDDATA_H
#define GBMAPPEDDATA_H

#include <QGroupBox>
#include "core/data/CDataMapperListener.h"

class AxesTester;


class GBMappedData  :   public  QGroupBox
                    ,   public  CDataMapperListener
{
    Q_OBJECT

public:

    explicit GBMappedData(QWidget *parent = 0);

    void    on_CDataMapper_output(const QString &argKeyOut, const QVariant &argData);


private:

    AxesTester  *m_atPosition;

};

#endif // GBMAPPEDDATA_H
