QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    json_reader.cpp \
    json_value.cpp \
    json_writer.cpp \
    main.cpp \
    mainwidget.cpp \
    projectmodel.cpp \
    sidemenu.cpp \
    taskmodel.cpp \
    tmfactory.cpp \
    usermodel.cpp \
    window.cpp

HEADERS += \
    allocator.h \
    assertions.h \
    config.h \
    forwards.h \
    json.h \
    json_features.h \
    json_tool.h \
    json_valueiterator.inl \
    mainwidget.h \
    projectmodel.h \
    reader.h \
    sidemenu.h \
    taskmodel.h \
    tmfactory.h \
    usermodel.h \
    value.h \
    version.h \
    window.h \
    writer.h

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
