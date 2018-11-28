project( KeychainWinService  )

add_subdirectory(./pass_ent_app)

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
		"../keychain_lib/include"
		"../keychain_cmd_app/secmodlib/include"
        "../libraries/fc_light/include"
		"../libraries/eth-crypto/include"
		"../libraries/secp256k1/include"
		"../libraries/secp256k1_ext/include"
        "${OPENSSL_ROOT_DIR}/include"
        ${Boost_INCLUDE_DIR})


file(GLOB KEYCHAIN_WIN_SERVICE_SOURCES "./src/*.cpp")
file(GLOB KEYCHAIN_WIN_SERVICE_INCLUDES "./include/keychain_lib/*.hpp" 	"../keychain_cmd_app/secmodlib/include/*.hpp")

add_executable(keychain_service_win ${KEYCHAIN_WIN_SERVICE_SOURCES} ${KEYCHAIN_WIN_SERVICE_INCLUDES})
target_link_libraries(keychain_service_win keychain_common secmodlib fc_light secp256k1 eth-crypto ${LIB_OPENSSL} ${Boost_LIBRARIES} Crypt32.lib Userenv.lib Wtsapi32.lib)

