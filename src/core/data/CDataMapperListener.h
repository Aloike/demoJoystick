#ifndef CDATAMAPPERLISTENER_H
#define CDATAMAPPERLISTENER_H

#include <QString>
#include <QVariant>


class CDataMapperListener
{
public:

    CDataMapperListener();

    virtual void    on_CDataMapper_output( const QString& argKeyIn,
                                           const QVariant &argDataIn,
                                           const QString &argKeyOut,
                                           const QVariant &argDataOut );
};

#endif // CDATAMAPPERLISTENER_H
