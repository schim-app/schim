cmake_minimum_required(VERSION 3.5)

project(schim VERSION $ENV{VERSION} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(symbols.h.in symbols.h @ONLY)

find_package(Qt5 5.12 COMPONENTS Core Gui Widgets Xml Svg PrintSupport Help REQUIRED)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/dxflib/libdxflib.a
    COMMENT "Building dxflib..."
    COMMAND mkdir -p dxflib
	COMMAND cd dxflib/ && qmake CONFIG+=warn_off ${CMAKE_SOURCE_DIR}/dxflib/ && make
    COMMAND cp ${CMAKE_SOURCE_DIR}/dxflib/src/*.h dxflib/
    # TODO I don't know why it compiles inside release/ on Windows
    COMMAND cp dxflib/release/libdxflib.a dxflib/ || true
)
add_custom_target(dxf DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/dxflib/libdxflib.a)

add_executable(schim
    fileio/database.cpp 
    fileio/dxf.cpp 
    fileio/miscfiles.cpp 
    fileio/pdf.cpp 
    fileio/xml.cpp 
    global.cpp 
    main.cpp 
    model/entity.cpp
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
    model/terminal.cpp
    ui/commands.cpp 
    ui/mainwindow.cpp 
    ui/vim.cpp
    ui/objects/gcomponent.cpp
    ui/objects/gcompositeobject.cpp 
    ui/objects/gheader.cpp 
    ui/objects/gline.cpp 
    ui/objects/gobject.cpp 
    ui/objects/grect.cpp 
    ui/objects/gtext.cpp 
    ui/objects/gterminal.cpp
    ui/objects/special/glinearobjectarray.cpp 
    ui/operations.cpp 
    ui/sheetscene.cpp 
    ui/sheetview.cpp 
    ui/projectmanager.cpp
    ui/widgets/completer.cpp
    ui/widgets/insertcompleter.cpp
    ui/widgets/symbolbrowser.cpp
    ui/widgets/projectbrowser.cpp
    ui/widgets/variableeditor.cpp 
    ui/windows/componentsettings.cpp 
    ui/windows/abstractsettingsdialog.cpp
    ui/windows/sheetsettings.cpp
    ui/windows/projectsettings.cpp
    ui/windows/preferences.cpp
    ui/windows/textsettings.cpp
    ui/windows/about.cpp
    cli/cli_common.cpp
    cli/cli_export.cpp
    cli/cli_editor.cpp
### FORMS ###
    ui/mainwindow.ui 
    ui/widgets/insertcompleter.ui
    ui/widgets/variableeditor.ui
    ui/windows/componentsettings.ui 
    ui/windows/abstractsettingsdialog.ui
    ui/windows/sheetsettings.ui
    ui/windows/projectsettings.ui
    ui/windows/preferences.ui
    ui/windows/textsettings.ui
### RESOURCES ###
    ../res/resources.qrc
)
if (WIN32)
    target_link_options(schim PRIVATE -mwindows)
endif()

add_dependencies(schim dxf)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/dxflib)
add_library(dxflib STATIC IMPORTED)
set_target_properties(dxflib PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/dxflib/libdxflib.a"
)
target_link_libraries(schim
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Xml
    Qt5::Svg
    Qt5::PrintSupport
    Qt5::Help
    dxflib
)
if (NOT WIN32)
    target_link_libraries(schim -lX11)
endif()
