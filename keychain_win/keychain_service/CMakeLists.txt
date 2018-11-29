project( KeychainWinService  )

cmake_minimum_required( VERSION 3.0.2 )

set(CMAKE_CXX_FLAGS "-std=c++11 /EHsc")

SET(BOOST_COMPONENTS)
LIST(APPEND BOOST_COMPONENTS log)
SET( Boost_USE_STATIC_LIBS ON CACHE STRING "ON or OFF" )

SET(BOOST_ROOT $ENV{BOOST_ROOT})
set(Boost_USE_MULTITHREADED ON)
set(BOOST_ALL_DYN_LINK OFF)

FIND_PACKAGE(Boost 1.65 REQUIRED COMPONENTS ${BOOST_COMPONENTS})
SET(BOOST_LIBRARIES_TEMP ${Boost_LIBRARIES})
FIND_PACKAGE(Boost 1.65 REQUIRED COMPONENTS coroutine)
LIST(APPEND BOOST_COMPONENTS coroutine)
SET(Boost_LIBRARIES ${BOOST_LIBRARIES_TEMP} ${Boost_LIBRARIES})

add_definitions(-DUNICODE -D_UNICODE)

include_directories(
		"./include"
		"${CMAKE_SOURCE_DIR}/keychain_lib/include"
	    "${CMAKE_SOURCE_DIR}/libraries/fc_light/include"
		"${CMAKE_SOURCE_DIR}/libraries/eth-crypto/include"
		"${CMAKE_SOURCE_DIR}/libraries/secp256k1/include"
		"${CMAKE_SOURCE_DIR}/libraries/secp256k1_ext/include"
        "${OPENSSL_ROOT_DIR}/include"
        ${Boost_INCLUDE_DIR})


file(GLOB KEYCHAIN_WIN_SERVICE_SOURCES "./src/*.cpp")
file(GLOB KEYCHAIN_WIN_SERVICE_INCLUDES "./include/keychain_lib/*.hpp" 	"${CMAKE_SOURCE_DIR}/keychain_win/secmodlib/include/*.hpp")

add_executable(keychain_service_win ${KEYCHAIN_WIN_SERVICE_SOURCES} ${KEYCHAIN_WIN_SERVICE_INCLUDES})
target_link_libraries(keychain_service_win keychain_agent keychain_common secmodlib fc_light secp256k1 eth-crypto ${LIB_OPENSSL} ${Boost_LIBRARIES} Crypt32.lib Userenv.lib Wtsapi32.lib)

