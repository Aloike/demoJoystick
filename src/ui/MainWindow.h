#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "core/data/CDataMapperListener.h"
#include "core/input/joystick/CJoystickListener.h"

class QAction;

class CDataMapper;
class CJoystick;
class GBMappedData;
class GBRawData;


class MainWindow    :   public  QMainWindow
                    ,   public  CDataMapperListener
                    ,   public  CJoystickListener
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = 0);
    ~MainWindow();


    void    on_CDataMapper_output(const QString &argKeyOut, const QVariant &argData);



private:

    void    create_layout(void);
    void    create_menus(void);



public slots:

    virtual void    on_joystickEvent(const CJoystickEvent&);



private slots:

    void    on_m_actionJoystickClose_triggered(void);
    void    on_m_actionJoystickOpen_triggered(void);



private:

    QAction         *m_actionJoystickClose;
    QAction         *m_actionJoystickOpen;
    CDataMapper     *m_dataMapper;
    GBMappedData    *m_gbMappedData;
    GBRawData       *m_gbRawData;
    CJoystick       *m_joystick;

};

#endif // MAINWINDOW_H
