#include "GLed.h"

#include <QPainter>
#include <QRadialGradient>
#include <QRect>


GLed::GLed(QWidget *parent) :
    QWidget(parent)
  ,_isActivated(false)
  ,_colorActivatedState(Qt::red)
  ,_colorDeactivatedState(Qt::gray)
  ,_colorCurrent(_colorDeactivatedState)
{
    this->setMinimumWidth(10);
    this->setMinimumHeight(10);

    this->_colorDeactivatedState.setRgb(100,100,100);

    this->setActivated(false);
    this->repaint();
}

GLed::GLed(const int argMinWidth, const int argMinHeight, QWidget *parent) :
    QWidget(parent)
  ,_isActivated(false)
  ,_colorActivatedState(Qt::red)
  ,_colorDeactivatedState(Qt::gray)
  ,_colorCurrent(_colorDeactivatedState)
{
    this->setMinimumWidth(argMinWidth);
    this->setMinimumHeight(argMinHeight);

    this->_colorDeactivatedState.setRgb(100,100,100);

    this->setActivated(false);
    this->repaint();
}

void GLed::setColor(const QColor &argColor)
{
    this->_colorActivatedState = argColor;
    this->repaint();
}

void GLed::paintEvent(QPaintEvent *)
{
    QPainter painter;

    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    /* Select the current color depending on the current state */
    if( this->_isActivated)
        this->_colorCurrent = this->_colorActivatedState;
    else
        this->_colorCurrent = this->_colorDeactivatedState;

    /* Calculate the position */
    int side = (this->width() < this->height()? this->width():this->height());
    int posX = (this->width()/2)-(side/2);
    int posY = (this->height()/2)-(side/2);
    QRect contour(posX,posY,side, side);

    /* Draw the contour */
    painter.setPen(Qt::black);
    painter.setBrush(this->_colorCurrent);
    painter.drawEllipse(contour);

    /* Draw the gradient to make it more realistic */
    QRadialGradient gradient(posX + (side/1.65), posY + (side/2.5), side/2 );
    gradient.setColorAt(0.0, (this->isActivated() ? Qt::white : Qt::lightGray));
    gradient.setColorAt((this->isActivated()? 0.7 : 0.4), this->_colorCurrent);
    gradient.setColorAt(1.0, Qt::transparent);

    painter.setBrush(gradient);
    painter.drawEllipse(contour);
}

void GLed::setActivated(const bool &argState)
{
    this->_isActivated = argState;
    this->repaint();
}
