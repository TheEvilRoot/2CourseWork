QT       += gui core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS

RC_ICONS += icn.ico
ICON = icn.icns

SOURCES += main.cpp \
    model/data/result.cpp \
    model/loader/storeworker.cpp \
    model/session/sessionstate.cpp \
    model/settings.cpp \
    presenter/main/mainpresenter.cpp \
    view/main/mainwindow.cpp \
    model/model.cpp \
    model/session/session.cpp \
    view/qfloatingwidget.cpp \
    view/qresultwidget.cpp \
    view/sessiondialog/sessiondialog.cpp \
    model/loader/wordsfileloader.cpp

HEADERS += \
    api/randomgenerator.hpp \
    api/utils.hpp \
    model/data/CEFR.hpp \
    model/data/basetest.hpp \
    model/data/checktest.hpp \
    model/data/choicetest.hpp \
    model/data/inputtest.hpp \
    model/data/result.hpp \
    model/loader/storeworker.hpp \
    model/session/sessionstate.hpp \
    presenter/main/mainpresenter.hpp \
    view/mainview.hpp \
    view/main/mainwindow.hpp \
    model/model.hpp \
    model/session/session.hpp \
    view/qfloatingwidget.hpp \
    view/qresultwidget.hpp \
    view/sessiondialog/sessiondialog.hpp \
    model/settings.hpp \
    view/viewtype.hpp \
    model/loader/wordsfileloader.hpp

FORMS += \
    mainwindow.ui \
    sessiondialog.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
  resources.qrc
