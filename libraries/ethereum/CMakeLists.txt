
project( Ether  )



#if( MSVC )
#    include_directories(
#            "./include"
#            "../secp256k1/include"
#            "${OPENSSL_ROOT_DIR}/include"
#            ${Boost_INCLUDE_DIR})
#else ( MSVC )
#    include_directories(
#            "./include"
#            "../secp256k1/include"
#            "${OPENSSL_ROOT_DIR}/include"
#            "/usr/include/libcxxabi"
#            ${Boost_INCLUDE_DIR})
#endif( MSVC )

#file(GLOB ETHER_SOURCES "./src/*/*.cpp")
#file(GLOB ETHER_INCLUDE "./include/ethereum/*/*.hpp"  "../secp256k1/include/.h")

#add_library(ether STATIC ${ETHER_SOURCES} ${ETHER_INCLUDE})
#target_link_libraries( ethereum_mod secp256k1_btc)

file(GLOB ETHER_HEADERS "include/ethereum/core/*.h" "include/ethereum/crypto/*.h")
file(GLOB ETHER_SOURCES "src/core/*.cpp" "src/crypto/*.cpp")


IF(NOT "$ENV{OPENSSL_ROOT_DIR}" STREQUAL "")
    set(OPENSSL_ROOT_DIR $ENV{OPENSSL_ROOT_DIR} )
    set(OPENSSL_INCLUDE_DIR ${OPENSSL_ROOT_DIR}/include)
    message(STATUS "Setting up OpenSSL root and include vars to ${OPENSSL_ROOT_DIR}, ${OPENSSL_INCLUDE_DIR}")
ENDIF()

find_package(OpenSSL REQUIRED)

add_library( ether ${ETHER_SOURCES} ${ETHER_HEADERS} )

target_link_libraries( ether fc_light secp256k1_btc ${OPENSSL_LIBRARIES}  ${CMAKE_USE_PTHREADS_INIT})

target_include_directories( ether
        PUBLIC "${CMAKE_SOURCE_DIR}/libraries/ethereum/include"
        PUBLIC "${CMAKE_SOURCE_DIR}/libraries/secp256k1/include"
        PUBLIC "${CMAKE_SOURCE_DIR}/libraries/fc_light/include"
        PRIVATE "${OPENSSL_ROOT_DIR}/include"
#        PUBLIC "${CMAKE_SOURCE_DIR}/libraries/secp256k1/include"
)
#target_include_directories( ether
#        PUBLIC "${CMAKE_SOURCE_DIR}/libraries/fc/include"
#        PUBLIC "${CMAKE_SOURCE_DIR}/libraries/ethereum/include"
#        PUBLIC "${CMAKE_SOURCE_DIR}/libraries/fc/vendor/secp256k1-zkp/include"
#)
