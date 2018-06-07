project( KeychainCmdApp )

add_subdirectory(./keychain_lib)

cmake_minimum_required( VERSION 3.0.2 )

set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_EXE_LINKER_FLAGS "-ldl -v")

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

include_directories("./keychain_lib/include" "${OPENSSL_ROOT_DIR}/include" "./3rd-party-components/include" "/usr/include/libcxxabi" ${Boost_INCLUDE_DIR})

find_library(LIB_FC NAME fc HINTS "./3rd-party-components/lib")
find_library(LIB_SECP256 NAME secp256k1 HINTS "./3rd-party-components/lib")
find_library(LIB_GRAPHENE graphene_utilities HINTS "./3rd-party-components/lib")

find_library(LIB_OPENSSL NAME crypto  HINTS "${OPENSSL_ROOT_DIR}/lib")
find_library(LIB_PTHREAD NAME pthread HINTS "/usr/lib/x86_64-linux-gnu")

file(GLOB KEYCHAIN_SOURCES "./keychain_cmd_app/*.cpp")
file(GLOB KEYCHAIN_INCLUDE "./keychain_cmd_app/*.hpp")

add_executable(keychain ${KEYCHAIN_SOURCES} ${KEYCHAIN_INCLUDE})

target_link_libraries(keychain keychain_common ${LIB_PTHREAD} ${LIB_FC} ${LIB_SECP256} ${LIB_GRAPHENE} ${LIB_OPENSSL} ${Boost_LIBRARIES})

