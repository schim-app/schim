cmake_minimum_required(VERSION 3.5)

project(schim VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Gui Widgets Xml Svg PrintSupport REQUIRED)
add_executable(schim
    fileio/database.cpp 
    fileio/dxf.cpp 
    fileio/miscfiles.cpp 
    fileio/pdf.cpp 
    fileio/xml.cpp 
    global.cpp 
    main.cpp 
    model/component.cpp 
    model/compositeobject.cpp 
    model/objectarray.cpp 
    model/special/corner.cpp 
    model/device.cpp 
    model/header.cpp 
    model/line.cpp 
    model/object.cpp 
    model/project.cpp 
    model/rect.cpp 
    model/sheet.cpp 
    model/special/linearobjectarray.cpp 
    model/text.cpp 
    model/variable.cpp 
    ui/commands.cpp 
    ui/mainwindow.cpp 
    ui/objects/gcomponent.cpp 
    ui/objects/gcompositeobject.cpp 
    ui/objects/gheader.cpp 
    ui/objects/gline.cpp 
    ui/objects/gobject.cpp 
    ui/objects/grect.cpp 
    ui/objects/gtext.cpp 
    ui/objects/special/glinearobjectarray.cpp 
    ui/operations.cpp 
    ui/sheetscene.cpp 
    ui/sheetview.cpp 
    ui/vimlike.cpp 
    ui/widgets/componentlist.cpp 
    ui/widgets/componentcompleter.cpp 
    ui/widgets/variableeditor.cpp 
    ui/windows/componentsettings.cpp 
    ui/windows/sheetsettings.cpp 
    ui/windows/textsettings.cpp
### HEADERS ###
    fileio/database.h 
    fileio/dxf.h 
    fileio/miscfiles.h 
    fileio/pdf.h 
    fileio/xml.h 
    global.h 
    model/component.h 
    model/compositeobject.h 
    model/objectarray.h 
    model/special/corner.h 
    model/device.h 
    model/header.h 
    model/line.h 
    model/object.h 
    model/project.h 
    model/rect.h 
    model/sheet.h 
    model/special/linearobjectarray.h 
    model/text.h 
    model/variable.h 
    ui/commands.h 
    ui/mainwindow.h 
    ui/objects/gcomponent.h 
    ui/objects/gcompositeobject.h 
    ui/objects/gheader.h 
    ui/objects/gline.h 
    ui/objects/gobject.h 
    ui/objects/grect.h 
    ui/objects/gtext.h 
    ui/objects/special/glinearobjectarray.h 
    ui/operations.h 
    ui/sheetscene.h 
    ui/sheetview.h 
    ui/vimlike.h 
    ui/widgets/componentlist.h 
    ui/widgets/componentcompleter.h 
    ui/widgets/variableeditor.h 
    ui/windows/componentsettings.h 
    ui/windows/sheetsettings.h 
    ui/windows/textsettings.h
### FORMS ###
    ui/mainwindow.ui 
    ui/widgets/variableeditor.ui 
    ui/windows/componentsettings.ui 
    ui/windows/sheetsettings.ui 
    ui/windows/textsettings.ui
)

target_link_libraries(schim
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Xml
    Qt5::Svg
    Qt5::PrintSupport
)
if (UNIX)
    target_link_libraries(schim dxflib)
endif ()
