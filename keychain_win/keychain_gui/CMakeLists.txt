project( keychain_win_gui  )

cmake_minimum_required( VERSION 3.0.2 )

set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_EXE_LINKER_FLAGS "/SUBSYSTEM:WINDOWS")

set(CMAKE_WIN32_EXECUTABLE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/keychain_win/keychain_gui/src")
set(AUTOGEN_BUILD_DIR "${CMAKE_SOURCE_DIR}/keychain_win/keychain_gui/GeneratedFiles")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core)
find_package(Qt5Qml)
find_package(Qt5Svg)

get_target_property(QtCore_location Qt5::Core LOCATION)

add_definitions(-DUNICODE -D_UNICODE)


QT5_ADD_RESOURCES(RESOURCES "${CMAKE_SOURCE_DIR}/keychain_win/keychain_gui/Resources/*.qrc")
QT5_WRAP_UI(MOC_FILES "${CMAKE_SOURCE_DIR}/keychain_win/keychain_gui/src/*.ui")
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Core_INCLUDES})
include_directories(${Qt5Qml_INCLUDES})
include_directories(${Qt5Svg_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})

include_directories("${CMAKE_SOURCE_DIR}/keychain_win/keychain_gui/include" "${AUTOGEN_BUILD_DIR}")
file(GLOB KEYCHAIN_GUI_SOURCES "${CMAKE_SOURCE_DIR}/keychain_win/keychain_gui/src/*.cpp")
file(GLOB KEYCHAIN_GUI_INCLUDES "${CMAKE_SOURCE_DIR}/keychain_win/keychain_gui/include/*.h")
file(GLOB KEYCHAIN_GUI_RESOURCES "${CMAKE_SOURCE_DIR}/keychain_win/keychain_gui/Resources/*.qrc")
file(GLOB KEYCHAIN_GUI_UIS "${CMAKE_SOURCE_DIR}/keychain_win/keychain_gui/src/*.ui")

add_executable(keychain_win_gui ${KEYCHAIN_GUI_SOURCES} ${KEYCHAIN_GUI_INCLUDES} ${KEYCHAIN_GUI_RESOURCES} ${KEYCHAIN_GUI_UIS})
        
target_link_libraries(keychain_win_gui Crypt32.lib Qt5::Widgets Qt5::Core Qt5::Qml Qt5::Svg)

