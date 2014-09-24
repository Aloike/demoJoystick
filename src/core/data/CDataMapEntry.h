#ifndef CDATAMAPENTRY_H
#define CDATAMAPENTRY_H

#include <QDomElement>
#include <QString>


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



protected:
private:


public:
protected:
private:

    QString m_key_in;                                                           /**< The identifier of the data source. */
    QString m_key_out;                                                          /**< The identifier of the data destination, where the data from m_key_in shall be redirected. */


};

#endif // CDATAMAPENTRY_H
