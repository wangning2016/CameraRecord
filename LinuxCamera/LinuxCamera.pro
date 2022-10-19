QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets





LIBS += -L/usr/local/lib -lSDL2
LIBS += -L/usr/local/lib -lavformat -lavcodec -lavdevice -lavfilter -lavutil -lswresample -lswscale
CONFIG += c++11

LIBS += -lasound

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    caudiodevice.cpp \
    ccameradevice.cpp \
    ccolorconverter.cpp \
    cdevicemgr.cpp \
    comfunction.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    caudiodevice.h \
    ccameradevice.h \
    ccolorconverter.h \
    cdevicemgr.h \
    comdef.h \
    comfunction.h \
    device_def.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
