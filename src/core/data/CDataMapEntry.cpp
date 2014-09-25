#include "CDataMapEntry.h"

#include <limits>


#define DBG(format,...) qDebug( "%s ==>"format, __PRETTY_FUNCTION__, ##__VA_ARGS__ )
#define ERR(format,...) qWarning( "%s ==>"format, __PRETTY_FUNCTION__, ##__VA_ARGS__ )

#define C_NL_DOUBLE_MAX std::numeric_limits<double>::max()


namespace Tags
{
    static const QString MapEntryElement    = "MapEntry";
    namespace MapEntry
    {
        static const QString InElement      = "key_in";
        static const QString OutElement     = "key_out";
        static const QString FactorElement  = "factor";
    }
}


/* ########################################################################## */
/* ########################################################################## */

CDataMapEntry::CDataMapEntry()
{
}

/* ########################################################################## */
/* ########################################################################## */
/**
 * @brief CDataMapEntry::applyFactor
 * @param argValue
 * @return
 *
 *
 * Si aucun facteur dans la liste __m_factorsList, retourne argValue.
 *
 */
double  CDataMapEntry::applyFactor(double argValue) const
{
    double  lFactorToApply  = 1;
    double  lOffsetToApply  = 0;

    if( this->factors().count() == 0 )
    {
        return argValue;
    }


    //--------------------------------------------------------------------------
    //  Détermination de quel facteur+offset appliquer
    //--------------------------------------------------------------------------
    if( argValue < 0 )
    {
        //----------------------------------------------------------------------
        //  Cas où la valeur d'entrée est strictement inférieure à zéro
        //----------------------------------------------------------------------

        for(int i = 0 ; i < this->factors().count() ; ++i )
        {
            CDataMapEntryFactor lFactor = this->factors().at( i );

            if( lFactor.floor > 0 )
            {
                /* On est arrivés au bout de la liste des facteurs ayant comme
                 * floor une valeur negative */
                break;
            }
            else
            {
                if( lFactor.floor < argValue )
                {
                    /* La valeur est comprise entre le floor du facteur et zero,
                     * on l'appliquera si l'on ne trouve pas de facteur plus
                     * proche de zero pour lequel floor < argValue < 0.
                     * */
                    lFactorToApply  = lFactor.value;
                    lOffsetToApply  = lFactor.offset;
                }
            }
        }
    }
    else if( argValue > 0 )
    {
        //----------------------------------------------------------------------
        //  Cas où la valeur d'entrée est strictement supérieure à zéro
        //----------------------------------------------------------------------

        int i = 0;

        while(      i < this->factors().count()
               &&   this->factors().at( i ).floor < 0 )
        {
            ++i;
            if( i >= this->factors().count() )
            {
                /* Pas de facteur dont le floor est superieur à 0 */
                break;
            }
        }

        for( ; i < this->factors().count() ; ++i )
        {
            CDataMapEntryFactor lFactor = this->factors().at( i );

            if( lFactor.floor < argValue )
            {
                continue;
            }
            else
            {
                lFactorToApply  = lFactor.value;
                lOffsetToApply  = lFactor.offset;

                break;
            }
        }
    }
    else /* argValue == 0 */
    {
        //----------------------------------------------------------------------
        //  Cas où la valeur d'entrée est égale à zéro
        //----------------------------------------------------------------------

        CDataMapEntryFactor factor_zero;
        factor_zero.floor   = 0;
        if( this->factors().contains( factor_zero ) )
        {
            factor_zero
                    = this->factors().at( this->factors()
                                          .indexOf( factor_zero ) );

            lFactorToApply  = factor_zero.value;
            lOffsetToApply  = factor_zero.offset;
        }
    }



    //--------------------------------------------------------------------------
    //  Application du facteur+offset selectionné
    //--------------------------------------------------------------------------
    if( lFactorToApply  != C_NL_DOUBLE_MAX )
    {
        argValue    *= lFactorToApply;
    }

    if( lOffsetToApply != C_NL_DOUBLE_MAX )
    {
        argValue    += lOffsetToApply;
    }


    return argValue;
}

/* ########################################################################## */
/* ########################################################################## */

bool    CDataMapEntry::factorInsert(const CDataMapEntryFactor &argFactor)
{
    if( this->__m_factorsList.contains( argFactor ) )
    {
        DBG( "The list already contains argFactor, replacing it by the new one." );


        /* We chose to replace an existing factor by the new factor. */
        this->__m_factorsList
                .removeAt( this->__m_factorsList.indexOf( argFactor ) );
    }

    this->__m_factorsList.prepend( argFactor );
    qSort( this->__m_factorsList );

    return true;
}

/* ########################################################################## */
/* ########################################################################## */

QList<CDataMapEntryFactor>  CDataMapEntry::factors() const
{
    return this->__m_factorsList;
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


    QDomElement elmtFactor
            = argRootElement.firstChildElement( Tags::MapEntry::FactorElement );
    while( ! elmtFactor.isNull() )
    {
        CDataMapEntryFactor factor;

        if( ! factor.loadFromElement( elmtFactor ) )
        {
            ERR( "Error while loading factor" );
        }
        else
        {
            DBG( "found factor --> (%s)",
                 factor.toString().toStdString().c_str() );
            this->factorInsert( factor );
        }

        /* go to next factor in the list */
        elmtFactor
                = elmtFactor
                .nextSiblingElement( Tags::MapEntry::FactorElement );
    }

    DBG( "+-- Found %d factors", this->factors().count() );
    for( int i = 0 ; i < this->factors().count() ; ++i )
    {
        DBG( "    +-- %s",
             this->factors().at( i ).toString().toStdString().c_str() );
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
