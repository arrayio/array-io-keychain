project( KeychainCmdApp  )

cmake_minimum_required( VERSION 3.0.2 )

set(CMAKE_CXX_FLAGS "-std=c++14")
if( NOT MSVC )
  set(CMAKE_EXE_LINKER_FLAGS "-ldl -v -fuse-ld=lld ")
#  SET(CMAKE_CXX_LINK_EXECUTABLE "/usr/bin/lld-6.0")
endif ( NOT MSVC )
  
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
            "../keychain_lib/include"
            "../array-io-core/libraries/utilities/include"
            "../array-io-core/libraries/db/include"
            "../array-io-core/libraries/chain/include"
            "../array-io-core/libraries/fc/include"
            "${OPENSSL_ROOT_DIR}/include"
            ${Boost_INCLUDE_DIR})
else ( MSVC )
    include_directories(
            "../keychain_lib/include"
            "../keychain_linux/passentry_cmd"
            "../array-io-core/libraries/utilities/incl/includeude"
            "../array-io-core/libraries/db/include"
            "../array-io-core/libraries/chain/include"
            "../array-io-core/libraries/fc/include"
            "${OPENSSL_ROOT_DIR}/include"
            "/usr/include/libcxxabi"
            ${Boost_INCLUDE_DIR})
endif( MSVC )

find_library(LIB_OPENSSL NAME crypto  HINTS "${OPENSSL_ROOT_DIR}/lib")
if( NOT MSVC )
  find_library(LIB_PTHREAD NAME pthread HINTS "/usr/lib/x86_64-linux-gnu")
endif( NOT MSVC )

file(GLOB KEYCHAIN_SOURCES "./*.cpp")
file(GLOB KEYCHAIN_INCLUDE "./*.hpp")

add_executable(keychain ${KEYCHAIN_SOURCES} ${KEYCHAIN_INCLUDE})

find_package(X11 REQUIRED)
if(NOT X11_FOUND)
    message(SEND_ERROR "Failed to find X11")
    return()
else()
    include_directories(${X11_INCLUDE_DIR})
endif()

find_library(LIB_XI NAME Xi HINTS "/usr/lib/x86_64-linux-gnu/")

if( MSVC )  
  target_link_libraries(keychain keychain_common fc secp256k1 graphene_utilities ${LIB_OPENSSL} ${Boost_LIBRARIES})
else ( MSVC )  
  target_link_libraries(keychain keychain_common ${LIB_PTHREAD} fc secp256k1 graphene_utilities graphene_chain ${LIB_OPENSSL} ${Boost_LIBRARIES} ${X11_LIBRARIES} ${LIB_XI})
endif ( MSVC ) 






