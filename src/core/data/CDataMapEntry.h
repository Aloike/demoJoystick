#ifndef CDATAMAPENTRY_H
#define CDATAMAPENTRY_H

#include <QDomElement>
#include <QList>
#include <QString>

#include "CDataMapEntryFactor.h"


class CDataMapEntry
{
public:

    static QString tagName(void);


    CDataMapEntry();


    bool    operator ==(const CDataMapEntry& argOther);


    //--------------------------------------------------------------------------
    //  Config load/save
    //--------------------------------------------------------------------------
    bool    loadFromElement(const QDomElement& argRootElement);


    //--------------------------------------------------------------------------
    //  Map management
    //--------------------------------------------------------------------------
    QString keyIn(void) const;
    void    setKeyIn(const QString& argKeyIn);

    QString keyOut(void) const;
    void    setKeyOut(const QString& argKeyOut);

    QList<CDataMapEntryFactor>  factors(void) const;
    bool                        factorInsert( const CDataMapEntryFactor& argFactor );


    //--------------------------------------------------------------------------
    //  Convenience data functions
    //--------------------------------------------------------------------------

    double  applyFactor(double argValue) const;



protected:
private:


public:
protected:
private:

    QString m_key_in;                                                           /**< The identifier of the data source. */
    QString m_key_out;                                                          /**< The identifier of the data destination, where the data from m_key_in shall be redirected. */

    QList<CDataMapEntryFactor>  __m_factorsList;                                /**< The list containing all the factors. @warning To keep its data integrity, it shall only be accessed through its dedicated methods. */


};

#endif // CDATAMAPENTRY_H
