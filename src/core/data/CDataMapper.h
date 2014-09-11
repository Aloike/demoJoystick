#ifndef CDATAMAPPER_H
#define CDATAMAPPER_H


#include <QDomElement>
#include <QMap>
#include <QString>
#include <QVariant>

class CDataMapperListener;


class CDataMapper
{

public:

    struct MapEntry
    {
        QString in;
        QString out;
    };


    CDataMapper();


    //--------------------------------------------------------------------------
    //  Listeners management
    //--------------------------------------------------------------------------
    bool    listenerAdd( CDataMapperListener* argListener );
    bool    listenerRemove( CDataMapperListener* argListener );


    //--------------------------------------------------------------------------
    //  Config load/save
    //--------------------------------------------------------------------------
    bool    loadFromFile( const QString& argFileName );
    bool    loadFromDomElement( const QDomElement& argRootElt );


    //--------------------------------------------------------------------------
    //  Mapping management
    //--------------------------------------------------------------------------
    bool    addEntryToMap(struct MapEntry argEntry);
    bool    removeEntryFromMap(struct MapEntry argEntry);
    void    clearMap(void);


    //--------------------------------------------------------------------------
    //  Data transmission methods
    //--------------------------------------------------------------------------
    bool    transmitData( const QString& argInKey, const QVariant& argData );



private:

    struct MapEntry elementToStruct( const QDomElement& argRootElement );



private:

    QList<CDataMapperListener*> m_listeners;
    QMap<QString,QString>       m_map;
};

#endif // CDATAMAPPER_H
