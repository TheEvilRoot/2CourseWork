QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    model/data/basetest.cpp \
    main.cpp \
    model/data/choicetest.cpp \
    presenter/main/mainpresenter.cpp \
    view/main/mainwindow.cpp \
    model/model.cpp \
    model/session/session.cpp \
    view/sessiondialog/sessiondialog.cpp \
    model/loader/wordsfileloader.cpp

HEADERS += \
    model/data/basetest.hpp \
    model/data/choicetest.h \
    presenter/main/mainpresenter.hpp \
    view/mainview.hpp \
    view/main/mainwindow.hpp \
    api/json.h \
    model/model.hpp \
    model/session/session.hpp \
    view/sessiondialog/sessiondialog.hpp \
    model/settings.h \
    view/viewtype.h \
    model/loader/wordsfileloader.h

FORMS += \
    mainwindow.ui \
    sessiondialog.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
  resources.qrc