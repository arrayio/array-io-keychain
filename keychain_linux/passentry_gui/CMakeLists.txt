cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "-std=c++14 --verbose -fno-limit-debug-info")
set(CMAKE_CXX_FLAGS "-std=c++14 ")

project(testproject)

set(CMAKE_PREFIX_PATH ${QT_ROOT}/gcc_64)
message(${CMAKE_PREFIX_PATH} )

if (NOT MSVC)
    if ( LIBCXX_BUILD )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -stdlib=libc++" )
    endif()
endif()

project(passentrygui)
# Find includes in corresponding build directories

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/keychain_linux/passentry_gui/src")
#set(AUTOGEN_BUILD_DIR "${CMAKE_SOURCE_DIR}/keychain_linux/passentry_gui/GeneratedFiles")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core)
#find_package(Qt5 5.12.0 REQUIRED Gui Xml)

#find_package(Qt5Qml  )
#find_package(Qt5Svg)

#get_target_property(QtCore_location Qt5::Core LOCATION)

#add_definitions(-DUNICODE -D_UNICODE)

#qt5_add_resources(RESOURCES "${CMAKE_SOURCE_DIR}/keychain_linux/passentry_gui/Resources/*.qrc")
#QT5_WRAP_UI(MOC_FILES "${CMAKE_SOURCE_DIR}/keychain_linux/passentry_gui/src/*.ui")
#include_directories(${Qt5Widgets_INCLUDES})
#include_directories(${Qt5Core_INCLUDES})
#include_directories(${Qt5Qml_INCLUDES})
#include_directories(${Qt5Svg_INCLUDES})
#add_definitions(${Qt5Widgets_DEFINITIONS})

find_library(LIB_PTHREAD NAME pthread HINTS "/usr/lib/x86_64-linux-gnu")

SET(BOOST_COMPONENTS)
LIST(APPEND BOOST_COMPONENTS log date_time system filesystem program_options  serialization chrono  locale thread exception iostreams)
SET( Boost_USE_STATIC_LIBS ON CACHE STRING "ON or OFF" )

#SET(BOOST_ROOT $ENV{BOOST_ROOT})
set(Boost_USE_MULTITHREADED ON)
set(BOOST_ALL_DYN_LINK OFF)

FIND_PACKAGE(Boost 1.65 REQUIRED COMPONENTS ${BOOST_COMPONENTS})
SET(Boost_LIBRARIES ${Boost_LIBRARIES})


include_directories("${CMAKE_SOURCE_DIR}/keychain_linux/passentry_gui/include"
#        "${AUTOGEN_BUILD_DIR}"
        "${CMAKE_SOURCE_DIR}/libraries/fc_light/include"
        "${CMAKE_SOURCE_DIR}/keychain_lib/include"
        "${CMAKE_SOURCE_DIR}/libraries/kaitai_struct_cpp_stl_runtime"
        ${Boost_INCLUDE_DIR}
        "/usr/include/libcxxabi"
        "${OPENSSL_ROOT_DIR}/include"
        ${Boost_INCLUDE_DIR})

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)
find_library(LIB_OPENSSL NAME crypto  HINTS "${OPENSSL_ROOT_DIR}/lib")

file(GLOB GUI_SOURCES "${CMAKE_SOURCE_DIR}/keychain_linux/passentry_gui/src/*.cpp")
file(GLOB GUI_INCLUDE "${CMAKE_SOURCE_DIR}/keychain_linux/passentry_gui/include/*.h*")
file(GLOB GUI_RESOURCES "./Resources/*.qrc")
file(GLOB GUI_UIS "./src/*.ui")

add_executable(passentry_gui ${GUI_SOURCES} ${GUI_INCLUDE} ${GUI_RESOURCES} ${GUI_UIS} )

# Use the Widgets module from Qt 5
#target_link_libraries(passentry_gui Qt5::Widgets Qt5::Core Qt5::Qml Qt5::Svg keychain_common secmodlib kaitai_struct_cpp_stl_runtime fc_light ${LIB_PTHREAD} ${LIB_OPENSSL} ${Boost_LIBRARIES} )
target_link_libraries(passentry_gui Qt5::Widgets Qt5::Core   keychain_common  kaitai_struct_cpp_stl_runtime fc_light ${LIB_PTHREAD} ${LIB_OPENSSL} ${Boost_LIBRARIES} -ldl )
