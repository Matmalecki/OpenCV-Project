
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Project
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        opencvmanager.cpp

HEADERS += \
        mainwindow.h \
        opencvmanager.h

FORMS += \
        mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../opencv/build/x64/vc15/lib/ -lopencv_world346
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../opencv/build/x64/vc15/lib/ -lopencv_world346d

INCLUDEPATH += $$PWD/../../../../../../opencv/build/include
DEPENDPATH += $$PWD/../../../../../../opencv/build/include
