#include "AxesTester.h"

#include <QDebug>
#include <QPainter>

static int POINT_SIDE = 10;
static int MAX_VALUE = 32768;

AxesTester::AxesTester(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumWidth(100);
    this->setMinimumHeight(100);

    this->_posX = 0;
    this->_posY = 0;

    this->alpha = 255;
    this->backgroundAlpha = 255;
}

void AxesTester::setAxeX(int argValue)
{
    this->_posX = argValue;
    this->repaint();
}

void AxesTester::setAxeY(int argValue)
{
    this->_posY = -argValue;
    this->repaint();
}

void AxesTester::paintEvent(QPaintEvent *)
{
    QColor color(Qt::white);
    QPainter painter(this);

    painter.setPen(Qt::red);
    color.setAlpha(this->backgroundAlpha);
    painter.setBrush(color);

    /* Draw the contour */
    int side = qMin(this->width(),this->height());
    side -= 10;
    painter.drawRect((this->width()-side)/2,
                     (this->height()-side)/2,
                     side,side);


    /* Draw the point */
    int x = (this->_posX/(float)MAX_VALUE)*(side/2.0f);
    x += side/2 + (this->width()-side)/2;
    x -= POINT_SIDE/2;

    int y = (this->_posY/(float)MAX_VALUE)*(side/2.0f);
    y += side/2 + (this->height()-side)/2;
    y -= POINT_SIDE/2;
    color = Qt::black;
    if(pointR) color.setRed(255);
    if(pointG) color.setGreen(255);
    if(pointB) color.setBlue(255);
    color.setAlpha(this->alpha);
    painter.setBrush(color);
    painter.drawRect(x, y, POINT_SIDE,POINT_SIDE);

//    qDebug()<< "Repainting with x ==" << x << " and y ==" << y;

    painter.end();
}
