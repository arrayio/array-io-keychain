project( fc_light  )

cmake_minimum_required( VERSION 3.0.2 )

set(CMAKE_CXX_FLAGS "-std=c++14 ")

if (NOT MSVC)
    if ( LIBCXX_BUILD )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -stdlib=libc++" )
    endif()
endif()

SET(BOOST_COMPONENTS)
LIST(APPEND BOOST_COMPONENTS date_time system filesystem program_options signals serialization chrono unit_test_framework locale iostreams)
SET( Boost_USE_STATIC_LIBS ON CACHE STRING "ON or OFF" )

SET(BOOST_ROOT $ENV{BOOST_ROOT})
set(Boost_USE_MULTITHREADED ON)
set(BOOST_ALL_DYN_LINK OFF)

FIND_PACKAGE(Boost 1.65 REQUIRED COMPONENTS ${BOOST_COMPONENTS})
SET(BOOST_LIBRARIES_TEMP ${Boost_LIBRARIES})
FIND_PACKAGE(Boost 1.65 REQUIRED COMPONENTS coroutine)
LIST(APPEND BOOST_COMPONENTS coroutine)
SET(Boost_LIBRARIES ${BOOST_LIBRARIES_TEMP} ${Boost_LIBRARIES})

if( MSVC )
    include_directories(
            "./include"
            "${OPENSSL_ROOT_DIR}/include"
            ${Boost_INCLUDE_DIR})
else ( MSVC )
    include_directories(
            "./include"
            "${OPENSSL_ROOT_DIR}/include"
            "/usr/include/libcxxabi"
            ${Boost_INCLUDE_DIR})
endif( MSVC )


file(GLOB FC_LIGHT_SOURCES "./src/*.cpp" "./src/*/*.cpp")
file(GLOB FC_LIGHT_INCLUDE "./include/fc_light/*.hpp" "./include/fc_light/*/*.hpp")

add_library(fc_light STATIC ${FC_LIGHT_SOURCES} ${FC_LIGHT_INCLUDE})