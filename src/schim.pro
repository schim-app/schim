QT       += core gui xml svg printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fileio/database.cpp \
    fileio/dxf.cpp \
    fileio/miscfiles.cpp \
    fileio/pdf.cpp \
    fileio/xml.cpp \
    global.cpp \
    main.cpp \
    model/component.cpp \
    model/compositeobject.cpp \
    model/device.cpp \
    model/header.cpp \
    model/line.cpp \
    model/object.cpp \
    model/project.cpp \
    model/rect.cpp \
    model/sheet.cpp \
    model/text.cpp \
    model/variable.cpp \
    ui/commands.cpp \
    ui/mainwindow.cpp \
    ui/objects/gcomponent.cpp \
    ui/objects/gcompositeobject.cpp \
    ui/objects/gheader.cpp \
    ui/objects/gline.cpp \
    ui/objects/gobject.cpp \
    ui/objects/grect.cpp \
    ui/objects/gtext.cpp \
    ui/operations.cpp \
    ui/sheetscene.cpp \
    ui/sheetview.cpp \
    ui/vimlike.cpp \
    ui/widgets/componentlist.cpp \
    ui/widgets/componentcompleter.cpp \
    ui/widgets/variableeditor.cpp \
    ui/windows/componenteditor.cpp \
    ui/windows/sheetsettings.cpp

HEADERS += \
    fileio/database.h \
    fileio/dxf.h \
    fileio/miscfiles.h \
    fileio/pdf.h \
    fileio/xml.h \
    global.h \
    model/component.h \
    model/compositeobject.h \
    model/device.h \
    model/header.h \
    model/line.h \
    model/object.h \
    model/project.h \
    model/rect.h \
    model/sheet.h \
    model/text.h \
    model/variable.h \
    ui/commands.h \
    ui/componentlist.h \
    ui/mainwindow.h \
    ui/objects/gcomponent.h \
    ui/objects/gcompositeobject.h \
    ui/objects/gheader.h \
    ui/objects/gline.h \
    ui/objects/gobject.h \
    ui/objects/grect.h \
    ui/objects/gtext.h \
    ui/operations.h \
    ui/sheetscene.h \
    ui/sheetview.h \
    ui/vimlike.h \
    ui/widgets/componentlist.h \
    ui/widgets/componentcompleter.h \
    ui/widgets/variableeditor.h \
    ui/windows/componenteditor.h \
    ui/windows/sheetsettings.h

FORMS += \
    ui/mainwindow.ui \
    ui/widgets/variableeditor.ui \
    ui/windows/componenteditor.ui \
    ui/windows/sheetsettings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix: LIBS += -ldxflib
