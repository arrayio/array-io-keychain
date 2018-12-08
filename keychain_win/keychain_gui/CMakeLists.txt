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

SET(BOOST_COMPONENTS)
LIST(APPEND BOOST_COMPONENTS log date_time system filesystem program_options signals serialization chrono unit_test_framework locale thread exception iostreams)
SET( Boost_USE_STATIC_LIBS ON CACHE STRING "ON or OFF" )

SET(BOOST_ROOT $ENV{BOOST_ROOT})
set(Boost_USE_MULTITHREADED ON)
set(BOOST_ALL_DYN_LINK OFF)

FIND_PACKAGE(Boost 1.65 REQUIRED COMPONENTS ${BOOST_COMPONENTS})
SET(Boost_LIBRARIES ${Boost_LIBRARIES})


if( NOT MSVC )
  find_library(LIB_OPENSSL NAME crypto HINTS "${OPENSSL_ROOT_DIR}/lib")
  find_library(LIB_PTHREAD NAME pthread HINTS "/usr/lib/x86_64-linux-gnu")
else()
  find_library(LIB_OPENSSL NAME libcrypto.lib HINTS "${OPENSSL_ROOT_DIR}/lib")
endif( NOT MSVC )

include_directories("${CMAKE_SOURCE_DIR}/keychain_win/keychain_gui/include" 
			"${AUTOGEN_BUILD_DIR}"
            "${CMAKE_SOURCE_DIR}/libraries/fc_light/include"
			"${CMAKE_SOURCE_DIR}/keychain_lib/include")
file(GLOB KEYCHAIN_GUI_SOURCES "${CMAKE_SOURCE_DIR}/keychain_win/keychain_gui/src/*.cpp")
file(GLOB KEYCHAIN_GUI_INCLUDES "${CMAKE_SOURCE_DIR}/keychain_win/keychain_gui/include/*.h")
file(GLOB KEYCHAIN_GUI_RESOURCES "${CMAKE_SOURCE_DIR}/keychain_win/keychain_gui/Resources/*.qrc")
file(GLOB KEYCHAIN_GUI_UIS "${CMAKE_SOURCE_DIR}/keychain_win/keychain_gui/src/*.ui")

add_executable(keychain_win_gui ${KEYCHAIN_GUI_SOURCES} ${KEYCHAIN_GUI_INCLUDES} ${KEYCHAIN_GUI_RESOURCES} ${KEYCHAIN_GUI_UIS})
        
target_link_libraries(keychain_win_gui Crypt32.lib Qt5::Widgets Qt5::Core Qt5::Qml Qt5::Svg)

