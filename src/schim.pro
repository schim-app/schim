QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fileio/dxf.cpp \
    fileio/xml.cpp \
    global.cpp \
    main.cpp \
    model/compositeobject.cpp \
    model/header.cpp \
    model/line.cpp \
    model/object.cpp \
    model/project.cpp \
    model/rect.cpp \
    model/sheet.cpp \
    ui/commands.cpp \
    ui/mainwindow.cpp \
    ui/objects/gcompositeobject.cpp \
    ui/objects/gheader.cpp \
    ui/objects/gline.cpp \
    ui/objects/gobject.cpp \
    ui/objects/grect.cpp \
    ui/sheetscene.cpp \
    ui/sheetsettings.cpp \
    ui/sheetview.cpp \
    ui/vimlike.cpp

HEADERS += \
    fileio/dxf.h \
    fileio/xml.h \
    global.h \
    model/compositeobject.h \
    model/header.h \
    model/line.h \
    model/object.h \
    model/project.h \
    model/rect.h \
    model/sheet.h \
    ui/commands.h \
    ui/mainwindow.h \
    ui/objects/gcompositeobject.h \
    ui/objects/gheader.h \
    ui/objects/gline.h \
    ui/objects/gobject.h \
    ui/objects/grect.h \
    ui/sheetscene.h \
    ui/sheetsettings.h \
    ui/sheetview.h \
    ui/vimlike.h

FORMS += \
    ui/mainwindow.ui \
    ui/sheetsettings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix: LIBS += -ldxflib