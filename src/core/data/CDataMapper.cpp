#include "CDataMapper.h"

#include <limits>

#include <QDomDocument>
#include <QFile>

#include "CDataMapperListener.h"


namespace Tags
{
    static const QString DataMapListElement = "data_map_list";
    static const QString DataMapElement = "data_map";
    namespace DataMap
    {
        static const QString In     = "key_in";
        static const QString Out    = "key_out";
    }
}


CDataMapper::CDataMapper()
{
}

bool    CDataMapper::addEntryToMap(MapEntry argEntry)
{
    bool    ret = true;

    //--------------------------------------------------------------------------
    //  Check pre-conditions
    //--------------------------------------------------------------------------
    if( argEntry.in.isNull() ) {
        qDebug( "NULL key for `in`" );
        ret = false;
    }

    if( ret && argEntry.out.isNull() ) {
        qDebug( "NULL key for `out`" );
        ret = false;
    }



    if( ret && ! m_map.keys().contains( argEntry.in ) )
    {
        /* The IN key is has not already been declared in the map */
        m_map.insert( argEntry.in, argEntry.out );
        ret = true;
    }
    else if( ret && ! m_map.values( argEntry.in ).contains( argEntry.out ) )
    {
        /* The map contains the IN key but the output keys associated to it
         * don't contain the OUT key defined in argEntry, so add a new IN key
         * entry with the argEntry's OUT key as output. */
        m_map.insertMulti( argEntry.in, argEntry.out );
        ret = true;
    }
    else if( ret )
    {
        /* The map already contains the exact argEntry, do nothing and return
         * false. */
        ret = false;
    }


    return ret;
}

void    CDataMapper::clearMap()
{
    this->m_map.clear();
}

struct CDataMapper::MapEntry    CDataMapper::elementToStruct(const QDomElement &argRootElement)
{
    struct MapEntry ret;
    ret.in  = QString();
    ret.out = QString();


    if( argRootElement.tagName() != Tags::DataMapElement ) {
        return ret;
    }


    QDomElement elmtIn  = argRootElement.firstChildElement( Tags::DataMap::In );
    QString lIn;
    if( elmtIn.isNull() ) {
        qDebug( "No in");
        return ret;
    } else {
        lIn = elmtIn.text();
        if( lIn.isEmpty() ) {
            qWarning( "Empty 'in' key !" );
            return ret;
        }
    }


    QDomElement elmtOut  = argRootElement.firstChildElement( Tags::DataMap::Out );
    QString lOut;
    if( elmtOut.isNull() ) {
        qDebug( "No out");
        return ret;
    } else {
        lOut    = elmtOut.text();
        if( lOut.isEmpty() ) {
            qWarning( "Empty 'out' key !" );
            return ret;
        }
    }

    ret.in  = lIn;
    ret.out = lOut;

    return ret;
}

bool CDataMapper::listenerAdd(CDataMapperListener *argListener)
{
    bool    ret = false;

    if( argListener == 0 ) {
        qDebug( "null argListener" );
        ret = false;
    } else if( this->m_listeners.contains( argListener ) ) {
        qDebug( "Listener has already been declared" );
        ret = false;
    } else {
        this->m_listeners.append( argListener );
        ret = true;
    }

    return ret;
}

bool CDataMapper::listenerRemove(CDataMapperListener *argListener)
{
    if( m_listeners.contains( argListener ) ) {
        m_listeners.removeAll( argListener );
        return true;
    } else {
        qDebug( "undeclared argListener" );
        return false;
    }

    return false;
}

bool CDataMapper::loadFromDomElement(const QDomElement &argRootElt)
{
    if( argRootElt.tagName() != Tags::DataMapListElement )
    {
        qDebug( "Bad argRootElement (expected `%s`, got `%s`)",
                Tags::DataMapListElement.toStdString().c_str(),
                argRootElt.tagName().toStdString().c_str() );
        return false;
    }

    this->clearMap();

    QDomElement mapElmt = argRootElt.firstChildElement( Tags::DataMapElement );
    while( ! mapElmt.isNull() )
    {
        qDebug( "Found dataMapElement !" );

        struct MapEntry lStruct = this->elementToStruct( mapElmt );
        qDebug( "+-- Decoded element:" );
        qDebug( "    +-- In     : `%s`", lStruct.in.toStdString().c_str() );
        qDebug( "    +-- Out    : `%s`", lStruct.out.toStdString().c_str() );

        this->addEntryToMap( lStruct );

        mapElmt = mapElmt.nextSiblingElement( Tags::DataMapElement );
    }

    return true;
}

bool CDataMapper::loadFromFile(const QString &argFileName)
{
    QFile file( argFileName );
    if( ! file.open( QFile::ReadOnly ) ) {
        qDebug( "Can't open file" );
        return false;
    }

    QDomDocument domDoc;
    QString errorString;
    int errorColumn = -1;
    int errorLine   = -1;
    if( ! domDoc.setContent( &file, &errorString, &errorLine, &errorColumn ) ) {
        qWarning( "Error while loading content : %s",
                  errorString.toStdString().c_str() );
        qWarning( "Line     == %d", errorLine );
        qWarning( "Column   == %d", errorColumn );
        file.close();
        return false;
    }
    file.close();

    QDomElement listElmt    = domDoc.firstChildElement( Tags::DataMapListElement );
    if( listElmt.isNull() ) {
        return false;
    }

    return this->loadFromDomElement( listElmt );
}


bool    CDataMapper::transmitData(const QString &argInKey, const QVariant &argData)
{
    if( ! this->m_map.keys().contains( argInKey ) ) {
        qDebug( "Unknown 'in' key `%s`", argInKey.toStdString().c_str() );
        return false;
    }


    foreach( QString lOutKey, m_map.values( argInKey ) )
    {
        for( int l = 0 ; l < m_listeners.count() ; ++l )
        {
            m_listeners.at( l )->on_CDataMapper_output( lOutKey, argData );
        }
    }

    return true;
}
