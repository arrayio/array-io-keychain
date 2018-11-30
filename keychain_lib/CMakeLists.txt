project( KeychainLib  )

cmake_minimum_required( VERSION 3.0.2 )

set(CMAKE_CXX_FLAGS "-std=c++14")

if (NOT MSVC)
    if ( LIBCXX_BUILD )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -stdlib=libc++" )
    endif()
else()
    set( CMAKE_VS_PLATFORM_TOOLSET "LLVM-vs2017")
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Z7 /EHsc") 
endif()

SET(BOOST_COMPONENTS)
LIST(APPEND BOOST_COMPONENTS date_time system filesystem program_options serialization chrono unit_test_framework locale iostreams)
SET( Boost_USE_STATIC_LIBS ON CACHE STRING "ON or OFF" )

SET(BOOST_ROOT $ENV{BOOST_ROOT})
set(Boost_USE_MULTITHREADED ON)
set(BOOST_ALL_DYN_LINK OFF)

FIND_PACKAGE(Boost 1.65 REQUIRED COMPONENTS ${BOOST_COMPONENTS})
SET(Boost_LIBRARIES ${Boost_LIBRARIES})

if( MSVC )
    include_directories(
            "./include/keychain_lib"
            "../libraries/fc_light/include"
            "../libraries/secp256k1/include"
            "../libraries/secp256k1_ext/include"
            "../libraries/eth-crypto/include"
#            "../libraries/eth-crypto/ethash/src/libethash"
            "${OPENSSL_ROOT_DIR}/include"
            ${Boost_INCLUDE_DIR})
else ( MSVC )
    include_directories(
            "./include/keychain_lib"
            "../libraries/fc_light/include"
            "../libraries/secp256k1/include"
            "../libraries/secp256k1_ext/include"
            "../libraries/eth-crypto/include"
#            "../libraries/eth-crypto/ethash/src/libethash"
            "${OPENSSL_ROOT_DIR}/include"
            "/usr/include/libcxxabi"
            ${Boost_INCLUDE_DIR})
endif( MSVC )


file(GLOB KEYCHAIN_LIB_SOURCES "./src/*.cpp" )
file(GLOB KEYCHAIN_LIB_INCLUDE "./include/keychain_lib/*.hpp" )

add_library(keychain_common STATIC ${KEYCHAIN_LIB_SOURCES} ${KEYCHAIN_LIB_INCLUDE})

