#ifndef AXESTESTER_H
#define AXESTESTER_H

#include <QWidget>

class AxesTester : public QWidget
{
    Q_OBJECT
public:
    explicit AxesTester(QWidget *parent = 0);
    
    void setAxeX(int argValue);
    void setAxeY(int argValue);

    bool    pointR;
    bool    pointG;
    bool    pointB;
    unsigned char alpha;
    unsigned char backgroundAlpha;

private:
    int _posX;
    int _posY;

protected:
    void paintEvent(QPaintEvent *);
signals:
    
public slots:
    
};

#endif // AXESTESTER_H
