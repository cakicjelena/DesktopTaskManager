QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    commentmodel.cpp \
    main.cpp \
    mainwidget.cpp \
    projectmodel.cpp \
    session.cpp \
    sidemenu.cpp \
    tasklist.cpp \
    taskmodel.cpp \
    tmfactory.cpp \
    usermodel.cpp \
    window.cpp

HEADERS += \
    commentmodel.h \
    mainwidget.h \
    projectmodel.h \
    session.h \
    sidemenu.h \
    tasklist.h \
    taskmodel.h \
    tmfactory.h \
    usermodel.h \
    window.h \

FORMS += \
    mainwidget.ui \
    sidemenu.ui \
    window.ui

TRANSLATIONS += \
    DesktopTaskManager_sr_RS.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
