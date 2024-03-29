QT -= gui
QT += widgets
QT += core
CONFIG += c++11
CONFIG += console
QT += charts

TEMPLATE = app

TARGET = test

QT += network
QT += datavisualization

#CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        datamodel.cpp \
        datatranslator.cpp \
        homepage.cpp \
        main.cpp \
        plotter.cpp \
        tripdata.cpp

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

FORMS += \
    datamodel.ui \
    homepage.ui \
    tripdata.ui

HEADERS += \
    datamodel.h \
    datatranslator.h \
    homepage.h \
    plotter.h \
    tripdata.h

RESOURCES += \
    graphics.qrc


