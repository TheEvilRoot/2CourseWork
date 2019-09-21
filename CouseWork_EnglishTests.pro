QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    basetest.cpp \
    main.cpp \
    mainwindow.cpp \
    session.cpp

HEADERS += \
    basetest.hpp \
    mainview.hpp \
    mainwindow.hpp \
    json.h \
    session.hpp \
    viewtype.h

FORMS += \
    mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
