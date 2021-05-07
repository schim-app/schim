# This file contains the rules to build schim on its own, and also as part of
# unit tests

# The following variables must be set in the cmake file that includes this file:
# QT_VERSION, QT_VERSION_MAJOR, EXECUTABLE, SRC_DIR

find_package(Qt${QT_VERSION_MAJOR} ${QT_VERSION}
    COMPONENTS
    Core Gui Widgets Xml Svg PrintSupport Help
    REQUIRED
)

# Configuration
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
configure_file(${SRC_DIR}/symbols.h.in symbols.h @ONLY)

add_executable(${EXECUTABLE}
    ${SRC_DIR}/fileio/database.cpp 
    ${SRC_DIR}/fileio/dxf.cpp 
    ${SRC_DIR}/fileio/miscfiles.cpp 
    ${SRC_DIR}/fileio/pdf.cpp 
    ${SRC_DIR}/fileio/xml.cpp 
    ${SRC_DIR}/global.cpp 
    ${SRC_DIR}/model/entity.cpp
    ${SRC_DIR}/model/component.cpp 
    ${SRC_DIR}/model/compositeobject.cpp 
    ${SRC_DIR}/model/objectarray.cpp 
    ${SRC_DIR}/model/special/corner.cpp 
    ${SRC_DIR}/model/device.cpp 
    ${SRC_DIR}/model/header.cpp 
    ${SRC_DIR}/model/line.cpp 
    ${SRC_DIR}/model/object.cpp 
    ${SRC_DIR}/model/project.cpp 
    ${SRC_DIR}/model/rect.cpp 
    ${SRC_DIR}/model/sheet.cpp 
    ${SRC_DIR}/model/special/linearobjectarray.cpp 
    ${SRC_DIR}/model/text.cpp 
    ${SRC_DIR}/model/variable.cpp 
    ${SRC_DIR}/model/terminal.cpp
    ${SRC_DIR}/ui/commands.cpp 
    ${SRC_DIR}/ui/mainwindow.cpp 
    ${SRC_DIR}/ui/vim.cpp
    ${SRC_DIR}/ui/objects/gcomponent.cpp
    ${SRC_DIR}/ui/objects/gcompositeobject.cpp 
    ${SRC_DIR}/ui/objects/gheader.cpp 
    ${SRC_DIR}/ui/objects/gline.cpp 
    ${SRC_DIR}/ui/objects/gobject.cpp 
    ${SRC_DIR}/ui/objects/grect.cpp 
    ${SRC_DIR}/ui/objects/gtext.cpp 
    ${SRC_DIR}/ui/objects/gterminal.cpp
    ${SRC_DIR}/ui/objects/special/glinearobjectarray.cpp 
    ${SRC_DIR}/ui/operations.cpp 
    ${SRC_DIR}/ui/sheetscene.cpp 
    ${SRC_DIR}/ui/sheetview.cpp 
    ${SRC_DIR}/ui/projectmanager.cpp
    ${SRC_DIR}/ui/widgets/completer.cpp
    ${SRC_DIR}/ui/widgets/insertcompleter.cpp
    ${SRC_DIR}/ui/widgets/symbolbrowser.cpp
    ${SRC_DIR}/ui/widgets/projectbrowser.cpp
    ${SRC_DIR}/ui/widgets/variableeditor.cpp 
    ${SRC_DIR}/ui/windows/componentsettings.cpp 
    ${SRC_DIR}/ui/windows/abstractsettingsdialog.cpp
    ${SRC_DIR}/ui/windows/sheetsettings.cpp
    ${SRC_DIR}/ui/windows/projectsettings.cpp
    ${SRC_DIR}/ui/windows/preferences.cpp
    ${SRC_DIR}/ui/windows/textsettings.cpp
    ${SRC_DIR}/ui/windows/about.cpp
    ${SRC_DIR}/cli/cli_common.cpp
    ${SRC_DIR}/cli/cli_export.cpp
    ${SRC_DIR}/cli/cli_editor.cpp
### FORMS ###
    ${SRC_DIR}/ui/mainwindow.ui 
    ${SRC_DIR}/ui/widgets/insertcompleter.ui
    ${SRC_DIR}/ui/widgets/variableeditor.ui
    ${SRC_DIR}/ui/windows/componentsettings.ui 
    ${SRC_DIR}/ui/windows/abstractsettingsdialog.ui
    ${SRC_DIR}/ui/windows/sheetsettings.ui
    ${SRC_DIR}/ui/windows/projectsettings.ui
    ${SRC_DIR}/ui/windows/preferences.ui
    ${SRC_DIR}/ui/windows/textsettings.ui
### RESOURCES ###
    ${SRC_DIR}/../res/resources.qrc
)

# Build dxflib from source
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/dxflib/libdxflib.a
    COMMENT "Building dxflib..."
    COMMAND mkdir -p dxflib
    COMMAND cd dxflib/ && qmake CONFIG+=warn_off ${SRC_DIR}/dxflib/ && make
    COMMAND cp ${SRC_DIR}/dxflib/src/*.h dxflib/
    # TODO I don't know why it compiles inside release/ on Windows
    COMMAND cp dxflib/release/libdxflib.a dxflib/ || true
)
add_custom_target(dxfbuild DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/dxflib/libdxflib.a)

# Create dependency on generated dxflib
add_dependencies(${EXECUTABLE} dxfbuild)
include_directories("${CMAKE_CURRENT_BINARY_DIR}/dxflib")
add_library(dxflib STATIC IMPORTED)
set_target_properties(dxflib PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/dxflib/libdxflib.a"
)

# Link dependencies

target_link_libraries(${EXECUTABLE}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Xml
    Qt5::Svg
    Qt5::PrintSupport
    Qt5::Help
    dxflib
)
if (WIN32)
    target_link_options(${EXECUTABLE} PRIVATE -mwindows)
else()
    target_link_libraries(${EXECUTABLE} -lX11)
endif()

