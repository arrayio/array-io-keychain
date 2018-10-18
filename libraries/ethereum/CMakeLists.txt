file(GLOB ETHER_HEADERS "include/ethereum/core/*.h" "include/ethereum/crypto/*.h")
file(GLOB ETHER_SOURCES "src/core/*.cpp" "src/crypto/*.cpp")

IF(NOT "$ENV{OPENSSL_ROOT_DIR}" STREQUAL "")
    set(OPENSSL_ROOT_DIR $ENV{OPENSSL_ROOT_DIR} )
    set(OPENSSL_INCLUDE_DIR ${OPENSSL_ROOT_DIR}/include)
    message(STATUS "Setting up OpenSSL root and include vars to ${OPENSSL_ROOT_DIR}, ${OPENSSL_INCLUDE_DIR}")
ENDIF()

find_package(OpenSSL REQUIRED)

FIND_PACKAGE(Boost 1.65 REQUIRED COMPONENTS ${BOOST_COMPONENTS})
include_directories (../secp256k1_ext/include)
include_directories(${Boost_INCLUDE_DIR})


add_library( ether ${ETHER_SOURCES} ${ETHER_HEADERS} )

target_link_libraries( ether fc_light secp256k1_btc ${OPENSSL_LIBRARIES}  ${CMAKE_USE_PTHREADS_INIT} dl)

target_include_directories( ether
        PUBLIC "${CMAKE_SOURCE_DIR}/libraries/ethereum/include"
        PUBLIC "${CMAKE_SOURCE_DIR}/libraries/secp256k1_ext/include"
        PUBLIC "${CMAKE_SOURCE_DIR}/libraries/fc_light/include"
        PRIVATE "${OPENSSL_ROOT_DIR}/include"
        )
