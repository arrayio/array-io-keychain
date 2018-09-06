cmake_minimum_required(VERSION 3.1.0)

project(passentrygui)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(BOOST_COMPONENTS)
list(APPEND BOOST_COMPONENTS thread date_time system filesystem program_options signals serialization chrono unit_test_framework context locale iostreams)
set( Boost_USE_STATIC_LIBS ON CACHE STRING "ON or OFF" )

set(Boost_USE_MULTITHREADED ON)
set(BOOST_ALL_DYN_LINK OFF)

find_package(Boost 1.65 REQUIRED COMPONENTS ${BOOST_COMPONENTS})
set(BOOST_LIBRARIES_TEMP ${Boost_LIBRARIES})
find_package(Boost 1.65 REQUIRED COMPONENTS coroutine)
list(APPEND BOOST_COMPONENTS coroutine)
set(Boost_LIBRARIES ${BOOST_LIBRARIES_TEMP} ${Boost_LIBRARIES})

find_library(LIB_OPENSSL NAME crypto  HINTS "${OPENSSL_ROOT_DIR}/lib")

find_package(Qt5 COMPONENTS Qml Quick WebEngine REQUIRED)

find_package(Qt5QuickCompiler)

qtquick_compiler_add_resources(RESOURCES qml/qml.qrc)

add_definitions(${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}})

file(GLOB QMLSOURCES "qml/*.qml")

file(GLOB QMLRESOURCES "qml/*.qrc")

include_directories("../../libraries/fc_light/include"
    ${Boost_INCLUDE_DIR}
    "${OPENSSL_ROOT_DIR}/include")

file(GLOB GUI_SOURCES "./*.cpp")
file(GLOB GUI_INCLUDE "./*.hpp")

add_custom_target(Qml SOURCES ${QMLSOURCES} ${QMLRESOURCES})

add_executable(passentrygui ${GUI_SOURCES} ${GUI_INCLUDE} ${RESOURCES})

target_link_libraries(passentrygui Qt5::Qml Qt5::Quick Qt5::WebEngine fc_light ${LIB_PTHREAD} ${LIB_OPENSSL} ${Boost_LIBRARIES})
