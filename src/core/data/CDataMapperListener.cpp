#include "CDataMapperListener.h"

CDataMapperListener::CDataMapperListener()
{
}

void    CDataMapperListener::on_CDataMapper_output( const QString   &argKeyIn,
                                                    const QVariant  &argDataIn,
                                                    const QString   &argKeyOut,
                                                    const QVariant  &argDataOut )
{
    Q_UNUSED(argDataIn)
    Q_UNUSED(argDataOut)
    Q_UNUSED(argKeyIn)
    Q_UNUSED(argKeyOut)
}
