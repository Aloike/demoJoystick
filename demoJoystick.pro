#-------------------------------------------------
#
# Project created by QtCreator 2014-08-19T02:09:13
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = demoJoystick
TEMPLATE = app


DESTDIR     = run

MOC_DIR     = build/moc
OBJECTS_DIR = build/obj
UI_DIR      = build/ui
RCC_DIR     = build/rcc


INCLUDEPATH +=  $$PWD/src

SOURCES +=\
    src/main.cpp \
    src/ui/MainWindow.cpp \
    src/core/input/joystick/CJoystick.cpp \
    src/core/input/joystick/CJoystickEvent.cpp \
    src/core/input/joystick/CJoystickListener.cpp \
    src/ui/GBRawData.cpp \
    src/ui/common/GLed.cpp \
    src/core/data/CDataMapper.cpp \
    src/core/data/CDataMapperListener.cpp \
    src/ui/common/AxesTester.cpp \
    src/ui/GBMappedData.cpp \
    src/core/data/CDataMapEntry.cpp \
    src/core/data/CDataMapEntryFactor.cpp

HEADERS  += \
    src/ui/MainWindow.h \
    src/core/input/joystick/CJoystick.h \
    src/core/input/joystick/CJoystickEvent.h \
    src/core/input/joystick/CJoystickListener.h \
    src/ui/GBRawData.h \
    src/ui/common/GLed.h \
    src/core/data/CDataMapper.h \
    src/core/data/CDataMapperListener.h \
    src/ui/common/AxesTester.h \
    src/ui/GBMappedData.h \
    src/core/data/CDataMapEntry.h \
    src/core/data/CDataMapEntryFactor.h

OTHER_FILES += \
    run/conf_js.xml
