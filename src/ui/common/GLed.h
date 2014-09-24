#ifndef GLED_H
#define GLED_H

/*!
 * \file GLed.h
 * \brief Objet représentant un diode electro-luminescente
 * \author MGO
 * \version 1.0
 */

#include <QColor>
#include <QWidget>

/*! \class  GLed
   * \brief Classe représentant une DEL.
   *
   * Cette classe est un widget sur lequel est dessiné une diode
   * electro-luminescente.
   */
class GLed : public QWidget
{
    Q_OBJECT
public:

    /* Macros to have the properties available in QtDesigner */
    Q_PROPERTY(bool Activated READ isActivated WRITE setActivated)
    Q_PROPERTY(QColor ColorWhenActivated READ color WRITE setColor)

    /**
     * @brief Default constructor
     * @param parent    The parent widget
     */
    explicit GLed(QWidget *parent = 0);

    /**
     * @brief Constructor
     * @param argMinWidth   The minimum led's width
     * @param argMinHeight  The minimum led's height
     * @param parent    The parent widget
     */
    explicit GLed(const int argMinWidth, const int argMinHeight,
                  QWidget *parent = 0);

    /**
     * @brief Defines the color the LED takes when activated
     * @param argColor  The led's color
     */
    void setColor(const QColor &argColor);
    /**
     * @brief   The color the LED takes when activated
     * @return const QColor The LED's color
     */
    const QColor& color(void) {return this->_colorActivatedState;}

    /**
     * @brief   Sets the LED's current state
     * @param argState  The LED's state
     */
    void setActivated(const bool &argState);
    /**
     * @brief   Returns the current state of the LED
     * @return bool The state of the led (Activated/deactivated)
     */
    bool isActivated(void) { return this->_isActivated;}

    
protected:
    void paintEvent(QPaintEvent *);

private:
    bool _isActivated;
    QColor _colorActivatedState;
    QColor _colorDeactivatedState;
    QColor _colorCurrent;

signals:
    
public slots:
    
};

#endif // GLED_H
