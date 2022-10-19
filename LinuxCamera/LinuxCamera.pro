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
    aacencodemgr.cpp \
    aacencoder.cpp \
    audioresampler.cpp \
    caudiodevice.cpp \
    ccameradevice.cpp \
    ccolorconverter.cpp \
    cdevicemgr.cpp \
    cmp3encode.cpp \
    comfunction.cpp \
    main.cpp \
    mainwindow.cpp \
    x264encoder.cpp

HEADERS += \
    aacencodemgr.h \
    aacencoder.h \
    audioresampler.h \
    caudiodevice.h \
    ccameradevice.h \
    ccolorconverter.h \
    cdevicemgr.h \
    cmp3encode.h \
    comdef.h \
    comfunction.h \
    device_def.h \
    mainwindow.h \
    x264encoder.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
