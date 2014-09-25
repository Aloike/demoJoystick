#include "CDataMapEntryFactor.h"

#include <limits>


#define DBG(format,...) qDebug( "%s ==>"format, __PRETTY_FUNCTION__, ##__VA_ARGS__ )
#define ERR(format,...) qWarning( "%s ==>"format, __PRETTY_FUNCTION__, ##__VA_ARGS__ )

#define C_NL_DOUBLE_MAX std::numeric_limits<double>::max()

static const QString C_ATTRIBUTE_FLOOR  = "floor";
static const QString C_ATTRIBUTE_OFFSET = "offset";
static const QString C_ATTRIBUTE_VALUE  = "value";

/* ########################################################################## */
/* ########################################################################## */

QString CDataMapEntryFactor::tagName()
{
    return "factor";
}

/* ########################################################################## */
/* ########################################################################## */
/* ########################################################################## */
/* ########################################################################## */

CDataMapEntryFactor::CDataMapEntryFactor()
    :   value( C_NL_DOUBLE_MAX )
    ,   floor( C_NL_DOUBLE_MAX )
    ,   offset( C_NL_DOUBLE_MAX )
{
}

/* ########################################################################## */
/* ########################################################################## */

bool CDataMapEntryFactor::operator ==(const CDataMapEntryFactor &argOther) const
{
    return (this->floor == argOther.floor);
}

/* ########################################################################## */
/* ########################################################################## */

bool CDataMapEntryFactor::operator <(const CDataMapEntryFactor &argOther) const
{
    return ( this->floor < argOther.floor );
}

/* ########################################################################## */
/* ########################################################################## */

bool    CDataMapEntryFactor::loadFromElement(const QDomElement &argRootElement)
{
    bool    ok  = false;
    bool    ret = true;

    if( argRootElement.tagName() != this->tagName() )
    {
        ERR( "invalid root element !" );
        ret = false;
    }

    /* ++++ POINT DE SORTIE ++++ */
    if( ! ret ) {
        return false;
    }
    /* <<<<<<<<<<<<<<<<<<<<<<<<< */


    if( ! argRootElement.hasAttribute( C_ATTRIBUTE_FLOOR ) )
    {
        ERR( "A `floor` element is missing a `value` attribute !" );
        ret = false;
    }
    else
    {
        this->value
                = argRootElement.attribute( C_ATTRIBUTE_VALUE )
                .toDouble( &ok );
        if( ! ok )
        {
            ERR( "invalid conversion of value from text `%s`",
                 argRootElement.attribute( C_ATTRIBUTE_VALUE )
                 .toStdString().c_str() );

            this->value = C_NL_DOUBLE_MAX;
            ret = false;
        }
    }


    /* ++++ POINT DE SORTIE ++++ */
    if( ! ret ) {
        return false;
    }
    /* <<<<<<<<<<<<<<<<<<<<<<<<< */


    if( argRootElement.hasAttribute( C_ATTRIBUTE_FLOOR ) )
    {
        this->floor
                = argRootElement.attribute( C_ATTRIBUTE_FLOOR )
                .toDouble( &ok );
        if( ! ok )
        {
            ERR( "invalid conversion of floor value from text `%s`",
                 argRootElement.attribute( C_ATTRIBUTE_FLOOR )
                 .toStdString().c_str() );

            this->floor = C_NL_DOUBLE_MAX;
            ret = false;
        }
    }


    if( argRootElement.hasAttribute( C_ATTRIBUTE_OFFSET ) )
    {
        this->offset
                = argRootElement.attribute( C_ATTRIBUTE_OFFSET )
                .toDouble( &ok );
        if( ! ok )
        {
            ERR( "invalid conversion of floor value from text `%s`",
                 argRootElement.attribute( C_ATTRIBUTE_OFFSET )
                 .toStdString().c_str() );

            this->offset = C_NL_DOUBLE_MAX;
            ret = false;
        }
    }


    /* ++++ POINT DE SORTIE ++++ */
    return ret;
    /* <<<<<<<<<<<<<<<<<<<<<<<<< */
}

/* ########################################################################## */
/* ########################################################################## */

QString CDataMapEntryFactor::toString(const QString &argSeparator) const
{
    QString ret;

    ret +=  argSeparator + "floor==" + QString::number( this->floor );
    ret +=  argSeparator + "value==" + QString::number( this->value );
    ret +=  argSeparator + "offset==" + QString::number( this->offset );

    return ret;
}

/* ########################################################################## */
/* ########################################################################## */
