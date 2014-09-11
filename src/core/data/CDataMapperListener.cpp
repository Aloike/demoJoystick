#include "CDataMapperListener.h"

CDataMapperListener::CDataMapperListener()
{
}

void    CDataMapperListener::on_CDataMapper_output( const QString &argKeyOut,
                                                    const QVariant &argData )
{
    Q_UNUSED(argData)
    Q_UNUSED(argKeyOut)
}
