#include "CDataMapper.h"

#include <limits>

#include <QDomDocument>
#include <QFile>

#include "CDataMapperListener.h"


#define DBG(format,...) qDebug( "%s ==> \t"format, __PRETTY_FUNCTION__, ##__VA_ARGS__ )
#define ERR(format,...) qWarning( "%s ==> \t"format, __PRETTY_FUNCTION__, ##__VA_ARGS__ )


namespace Tags
{
    static const QString DataMapListElement = "data_map_list";
}


/* ########################################################################## */
/* ########################################################################## */

CDataMapper::CDataMapper()
{
}

/* ########################################################################## */
/* ########################################################################## */

bool    CDataMapper::addEntryToMap(CDataMapEntry argEntry)
{
    bool    ret = true;

    //--------------------------------------------------------------------------
    //  Check pre-conditions
    //--------------------------------------------------------------------------
    if( argEntry.keyIn().isNull() ) {
        DBG( "NULL key for `in`" );
        ret = false;
    }

    if( ret && argEntry.keyOut().isNull() ) {
        DBG( "NULL key for `out`" );
        ret = false;
    }



    if( ret && ! m_map.keys().contains( argEntry.keyIn() ) )
    {
        /* The IN key is has not already been declared in the map */
        m_map.insert( argEntry.keyIn(), argEntry );
        ret = true;
    }
    else if( ret && ! m_map.values( argEntry.keyIn() ).contains( argEntry ) )
    {
        /* The map contains the IN key but the output keys associated to it
         * don't contain the OUT key defined in argEntry, so add a new IN key
         * entry with the argEntry's OUT key as output. */
        m_map.insertMulti( argEntry.keyIn(), argEntry );
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

/* ########################################################################## */
/* ########################################################################## */

void    CDataMapper::clearMap()
{
    this->m_map.clear();
}

/* ########################################################################## */
/* ########################################################################## */

QString    CDataMapper::elementName()
{
    return Tags::DataMapListElement;
}

/* ########################################################################## */
/* ########################################################################## */

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

/* ########################################################################## */
/* ########################################################################## */

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

/* ########################################################################## */
/* ########################################################################## */

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

    QDomElement mapElmt
            = argRootElt.firstChildElement( CDataMapEntry::tagName() );
    while( ! mapElmt.isNull() )
    {
        DBG( "Found dataMapElement !" );

        CDataMapEntry mapEntry;

        if( mapEntry.loadFromElement( mapElmt ) )
        {
            DBG( "+-- Decoded element:" );
            DBG( "    +-- In     : `%s`", mapEntry.keyIn().toStdString().c_str() );
            DBG( "    +-- Out    : `%s`", mapEntry.keyOut().toStdString().c_str() );

            this->addEntryToMap( mapEntry );
        }


        mapElmt = mapElmt.nextSiblingElement( CDataMapEntry::tagName() );
    }

    return true;
}

/* ########################################################################## */
/* ########################################################################## */

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

/* ########################################################################## */
/* ########################################################################## */

bool    CDataMapper::transmitData(const QString &argInKey, const QVariant &argData)
{
    if( ! this->m_map.keys().contains( argInKey ) ) {
//        DBG( "Unknown 'in' key `%s`", argInKey.toStdString().c_str() );
        return false;
    }




    foreach( CDataMapEntry lMapEntry, m_map.values( argInKey ) )
    {
        bool    ok  = false;
        QVariant transmittedData;
        transmittedData
                = lMapEntry.applyFactor( argData.toDouble( &ok ) );
        if( ! ok )
        {
            transmittedData = argData;
        }

        for( int l = 0 ; l < m_listeners.count() ; ++l )
        {
            m_listeners.at( l )->on_CDataMapper_output( argInKey,
                                                        argData,
                                                        lMapEntry.keyOut(),
                                                        transmittedData );
        }
    }

    return true;
}

/* ########################################################################## */
/* ########################################################################## */
