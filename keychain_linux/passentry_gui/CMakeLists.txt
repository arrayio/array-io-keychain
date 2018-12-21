cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "-std=c++14 --verbose -fno-limit-debug-info")
set(CMAKE_CXX_FLAGS "-std=c++14 ")

if (NOT MSVC)
    if ( LIBCXX_BUILD )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -stdlib=libc++" )
    endif()
endif()

set (CMAKE_PREFIX_PATH "${QT_ROOT}/gcc_64/lib/cmake/Qt5Widgets/")

project(passentrygui)
# Find includes in corresponding build directories

set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
#set(CMAKE_AUTOUIC ON)


find_library(LIB_PTHREAD NAME pthread HINTS "/usr/lib/x86_64-linux-gnu")

SET(BOOST_COMPONENTS)
LIST(APPEND BOOST_COMPONENTS date_time system filesystem program_options serialization chrono unit_test_framework locale iostreams)
SET( Boost_USE_STATIC_LIBS ON CACHE STRING "ON or OFF" )

set(Boost_USE_MULTITHREADED ON)
set(BOOST_ALL_DYN_LINK OFF)

FIND_PACKAGE(Boost 1.65 REQUIRED COMPONENTS ${BOOST_COMPONENTS})
SET(Boost_LIBRARIES ${Boost_LIBRARIES})

include_directories(
        "include"
        "../../keychain_lib/include"
        "../../libraries/kaitai_struct_cpp_stl_runtime"
        "../../libraries/fc_light/include"
        "/usr/include/libcxxabi"
        "${OPENSSL_ROOT_DIR}/include"
        ${Boost_INCLUDE_DIR})

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)
find_library(LIB_OPENSSL NAME crypto  HINTS "${OPENSSL_ROOT_DIR}/lib")

file(GLOB GUI_SOURCES "./src/*.cpp")
file(GLOB GUI_INCLUDE "./include/*.hpp")

add_executable(passentry_gui ${GUI_SOURCES} ${GUI_INCLUDE})


# Use the Widgets module from Qt 5
target_link_libraries(passentry_gui Qt5::Widgets fc_light ${LIB_PTHREAD} ${LIB_OPENSSL} ${Boost_LIBRARIES} )

