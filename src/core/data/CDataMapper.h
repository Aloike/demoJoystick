#ifndef CDATAMAPPER_H
#define CDATAMAPPER_H


#include <QDomElement>
#include <QMap>
#include <QString>
#include <QVariant>

class CDataMapperListener;
#include "CDataMapEntry.h"


class CDataMapper
{

public:

    CDataMapper();


    //--------------------------------------------------------------------------
    //  Listeners management
    //--------------------------------------------------------------------------
    bool    listenerAdd( CDataMapperListener* argListener );
    bool    listenerRemove( CDataMapperListener* argListener );


    //--------------------------------------------------------------------------
    //  Config load/save
    //--------------------------------------------------------------------------
    static QString  elementName(void);
    bool    loadFromFile( const QString& argFileName );
    bool    loadFromDomElement( const QDomElement& argRootElt );


    //--------------------------------------------------------------------------
    //  Mapping management
    //--------------------------------------------------------------------------
    bool    addEntryToMap(CDataMapEntry argEntry);
    bool    removeEntryFromMap(CDataMapEntry argEntry);
    void    clearMap(void);


    //--------------------------------------------------------------------------
    //  Data transmission methods
    //--------------------------------------------------------------------------
    bool    transmitData( const QString& argInKey, const QVariant& argData );



private:



public:
protected:
private:

    QList<CDataMapperListener*> m_listeners;
    QMap<QString,CDataMapEntry> m_map;
};

#endif // CDATAMAPPER_H
