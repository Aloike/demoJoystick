#ifndef CDATAMAPENTRYFACTOR_H
#define CDATAMAPENTRYFACTOR_H


#include <QDomElement>
#include <QString>


class CDataMapEntryFactor
{
public:

    static QString tagName(void);


    CDataMapEntryFactor();

    bool    operator ==(const CDataMapEntryFactor& argOther) const;
    bool    operator <(const CDataMapEntryFactor& argOther) const;

    //--------------------------------------------------------------------------
    //  load/save
    //--------------------------------------------------------------------------
    bool    loadFromElement(const QDomElement& argRootElement);


    QString toString(const QString& argSeparator="|") const;


    double  value;
    double  floor;
    double  offset;
};

#endif // CDATAMAPENTRYFACTOR_H
