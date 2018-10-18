cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "-std=c++14 --verbose -fno-limit-debug-info")
set(CMAKE_CXX_FLAGS "-std=c++14 ")

set (CMAKE_PREFIX_PATH "${QT_ROOT}/5.11.1/gcc_64/lib/cmake/Qt5Widgets/")

project(passentrygui)
# Find includes in corresponding build directories

set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
#set(CMAKE_AUTOUIC ON)


find_library(LIB_PTHREAD NAME pthread HINTS "/usr/lib/x86_64-linux-gnu")

SET(BOOST_COMPONENTS)
LIST(APPEND BOOST_COMPONENTS thread date_time system filesystem program_options signals serialization chrono unit_test_framework context locale iostreams)
SET( Boost_USE_STATIC_LIBS ON CACHE STRING "ON or OFF" )

set(Boost_USE_MULTITHREADED ON)
set(BOOST_ALL_DYN_LINK OFF)

FIND_PACKAGE(Boost 1.65 REQUIRED COMPONENTS ${BOOST_COMPONENTS})
SET(BOOST_LIBRARIES_TEMP ${Boost_LIBRARIES})
FIND_PACKAGE(Boost 1.65 REQUIRED COMPONENTS coroutine)
LIST(APPEND BOOST_COMPONENTS coroutine)
SET(Boost_LIBRARIES ${BOOST_LIBRARIES_TEMP} ${Boost_LIBRARIES})

include_directories(
        "../../libraries/fc_light/include"
        "/usr/include/libcxxabi"
        "${OPENSSL_ROOT_DIR}/include"
        ${Boost_INCLUDE_DIR})

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)
find_library(LIB_OPENSSL NAME crypto  HINTS "${OPENSSL_ROOT_DIR}/lib")

file(GLOB GUI_SOURCES "./*.cpp")
file(GLOB GUI_INCLUDE "./*.hpp")

add_executable(passentry_gui ${GUI_SOURCES} ${GUI_INCLUDE})


# Use the Widgets module from Qt 5
target_link_libraries(passentry_gui Qt5::Widgets fc_light ${LIB_PTHREAD} ${LIB_OPENSSL} ${Boost_LIBRARIES} )


set(GUI_BINARY ${CMAKE_BINARY_DIR}/keychain_linux/passentry_gui/passentry_gui)
set(KEYCHAIN_BINARY_DIR ${CMAKE_BINARY_DIR}/keychain_cmd_app)
    ADD_CUSTOM_COMMAND(TARGET passentry_gui
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${GUI_BINARY} ${KEYCHAIN_BINARY_DIR}
            )

