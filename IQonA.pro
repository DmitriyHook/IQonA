#-------------------------------------------------
#
# Project created by QtCreator 2020-02-10T15:15:52
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = IQonA
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        IQAnalizer/iqanalyzer.cpp \
        IQAnalizer/iqbuffer.cpp \
        IQAnalizer/iqcontainer.cpp \
        IQAnalizer/iqnco.cpp \
        IQVisualizer/iqvisualizer.cpp \
        IQVisualizer/iqwaterfall.cpp \
        IQVisualizer/qcustomplot.cpp \
        abstractudp.cpp \
        iqon.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        IQAnalizer/iqanalyzer.h \
        IQAnalizer/iqbuffer.h \
        IQAnalizer/iqcontainer.h \
        IQAnalizer/iqnco.h \
        IQVisualizer/iqvisualizer.h \
        IQVisualizer/iqwaterfall.h \
        IQVisualizer/qcustomplot.h \
        abstractudp.h \
        iqon.h \
        mainwindow.h

FORMS += \
        IQVisualizer/iqvisualizer.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -L/usr/include -lfftw3

RESOURCES += \
    resourses.qrc

DISTFILES += \
    README.md
