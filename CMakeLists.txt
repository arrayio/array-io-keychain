project( KeychainApp )

#add_subdirectory(auxiliary)

cmake_minimum_required( VERSION 3.0.2 )

set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_EXE_LINKER_FLAGS
        "-L/home/roman/blockchain/array-io-keychain/3rd-party-components/lib \
        -L/opt/openssllib/lib\
        -L/opt/boostlib/lib\
        -lpthread \
        -ldl")

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


include_directories("./include" "/opt/openssllib/include" "./3rd-party-components/include" ${Boost_INCLUDE_DIR})

find_library(FCLIB name fc HINTS "~/blockchain/3rd-party-components/lib")
find_library(FCLIB name secp256k1 HINTS "~/blockchain/3rd-party-components/lib")
find_library(FCLIB name graphene_utilities HINTS "~/blockchain/3rd-party-components/lib")
find_library(FCLIB name crypto HINTS "/opt/openssllib/lib")
find_library(FCLIB name boost_program_options HINTS "/opt/boostlib/lib")
file(GLOB KEYCHAIN_SOURCES "./src/*.cpp")
file(GLOB KEYCHAIN_INCLUDE "./include/*.hpp")

add_executable(keychain ${KEYCHAIN_SOURCES})

target_link_libraries(keychain fc secp256k1 graphene_utilities crypto ${Boost_LIBRARIES})

