QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    basetest.cpp \
    main.cpp \
    mainpresenter.cpp \
    mainwindow.cpp \
    model.cpp \
    session.cpp \
    sessiondialog.cpp

HEADERS += \
    basetest.hpp \
    mainpresenter.hpp \
    mainview.hpp \
    mainwindow.hpp \
    json.h \
    model.hpp \
    session.hpp \
    sessiondialog.hpp \
    settings.h \
    viewtype.h

FORMS += \
    mainwindow.ui \
    sessiondialog.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
  resources.qrc
