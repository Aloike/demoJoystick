#include "CDataMapEntry.h"


#define DBG(format,...) qDebug( "%s ==>"format, __PRETTY_FUNCTION__, ##__VA_ARGS__ )
#define ERR(format,...) qWarning( "%s ==>"format, __PRETTY_FUNCTION__, ##__VA_ARGS__ )


namespace Tags
{
    static const QString MapEntryElement    = "MapEntry";
    namespace MapEntry
    {
        static const QString InElement  = "key_in";
        static const QString OutElement = "key_out";
    }
}


/* ########################################################################## */
/* ########################################################################## */

CDataMapEntry::CDataMapEntry()
{
}

/* ########################################################################## */
/* ########################################################################## */

QString CDataMapEntry::keyIn() const
{
    return this->m_key_in;
}

/* ########################################################################## */
/* ########################################################################## */

QString CDataMapEntry::keyOut() const
{
    return this->m_key_out;
}

/* ########################################################################## */
/* ########################################################################## */

bool    CDataMapEntry::loadFromElement(const QDomElement &argRootElement)
{
    this->m_key_in  = QString();
    this->m_key_out = QString();


    if( argRootElement.tagName() != this->tagName() ) {
        return false;
    }


    QDomElement elmtIn
            = argRootElement.firstChildElement( Tags::MapEntry::InElement );
    QString lIn;
    if( elmtIn.isNull() ) {
        DBG( "No in" );
        return false;
    } else {
        lIn = elmtIn.text();
        if( lIn.isEmpty() ) {
            ERR( "Empty 'in' key !" );
            return false;
        }
    }


    QDomElement elmtOut
            = argRootElement.firstChildElement( Tags::MapEntry::OutElement );
    QString lOut;
    if( elmtOut.isNull() ) {
        DBG( "No out");
        return false;
    } else {
        lOut    = elmtOut.text();
        if( lOut.isEmpty() ) {
            ERR( "Empty 'out' key !" );
            return false;
        }
    }


    this->setKeyIn( lIn );
    this->setKeyOut( lOut );

    return true;
}

/* ########################################################################## */
/* ########################################################################## */

bool    CDataMapEntry::operator ==(const CDataMapEntry& argOther)
{
    bool    ret =   true;

    ret &= ( this->keyIn() == argOther.keyIn() );
    ret &= ( this->keyOut() == argOther.keyOut() );

    return ret;
}

/* ########################################################################## */
/* ########################################################################## */

void    CDataMapEntry::setKeyIn(const QString &argKeyIn)
{
    this->m_key_in  = argKeyIn;
}

/* ########################################################################## */
/* ########################################################################## */

void    CDataMapEntry::setKeyOut(const QString &argKeyOut)
{
    this->m_key_out = argKeyOut;
}

/* ########################################################################## */
/* ########################################################################## */

QString CDataMapEntry::tagName()
{
    return  Tags::MapEntryElement;
}

/* ########################################################################## */
/* ########################################################################## */
