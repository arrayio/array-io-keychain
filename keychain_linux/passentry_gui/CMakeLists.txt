cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "-std=c++14 --verbose -fno-limit-debug-info")
set(CMAKE_CXX_FLAGS "-std=c++14 --verbose ")
set(CMAKE_EXE_LINKER_FLAGS "-ldl -v")

set (CMAKE_PREFIX_PATH "/home/user/Qt5.11.1/")

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
        "../../array-io-core/libraries/utilities/include"
        "../../array-io-core/libraries/db/include"
        "../../array-io-core/libraries/chain/include"
        "../../array-io-core/libraries/fc/include"
        "/usr/include/libcxxabi"
        ${Boost_INCLUDE_DIR})

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

# Populate a CMake variable with the sources
set(SRCS
    polling.cpp
    widget.cpp
    main.cpp
    polling.hpp
    widget.hpp
        cmd.cpp cmd.hpp)

# Tell CMake to create the helloworld executable
add_executable(passentry_gui ${SRCS})

# Use the Widgets module from Qt 5
target_link_libraries(passentry_gui Qt5::Widgets ${LIB_PTHREAD} fc secp256k1 graphene_utilities graphene_chain ${Boost_LIBRARIES} )

