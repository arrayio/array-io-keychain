project( KeychainCmdApp )

add_subdirectory(./keychain_lib)
add_subdirectory(./array-io-core)

cmake_minimum_required( VERSION 3.0.2 )

set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_EXE_LINKER_FLAGS "-ldl")

SET(BOOST_COMPONENTS)
LIST(APPEND BOOST_COMPONENTS thread date_time system filesystem program_options signals serialization chrono unit_test_framework context locale iostreams)
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
            "./keychain_lib/include"
            "./array-io-core/libraries/utilities/include"
            "./array-io-core/libraries/db/include"
            "./array-io-core/libraries/chain/include"
            "./array-io-core/libraries/fc/include"
            "${OPENSSL_ROOT_DIR}/include"
            ${Boost_INCLUDE_DIR})
else ( MSVC )
    include_directories(
            "./keychain_lib/include"
            "./array-io-core/libraries/utilities/include"
            "./array-io-core/libraries/db/include"
            "./array-io-core/libraries/chain/include"
            "./array-io-core/libraries/fc/include"
            "${OPENSSL_ROOT_DIR}/include"
            "/usr/include/libcxxabi"
            ${Boost_INCLUDE_DIR})
endif( MSVC )

find_library(LIB_OPENSSL NAME crypto  HINTS "${OPENSSL_ROOT_DIR}/lib")
if( NOT MSVC )
  find_library(LIB_PTHREAD NAME pthread HINTS "/usr/lib/x86_64-linux-gnu")
endif( NOT MSVC )

file(GLOB KEYCHAIN_SOURCES "./keychain_cmd_app/*.cpp")
file(GLOB KEYCHAIN_INCLUDE "./keychain_cmd_app/*.hpp")

add_executable(keychain ${KEYCHAIN_SOURCES} ${KEYCHAIN_INCLUDE})

target_link_libraries(keychain keychain_common ${LIB_PTHREAD} fc secp256k1 graphene_utilities ${LIB_OPENSSL} ${Boost_LIBRARIES})

