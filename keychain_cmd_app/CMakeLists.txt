project( KeychainCmdApp  )

cmake_minimum_required( VERSION 3.0.2 )

set(CMAKE_CXX_FLAGS "-std=c++14")
set( CMAKE_EXE_LINKER_FLAGS )

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


find_library(LIB_OPENSSL NAME crypto  HINTS "${OPENSSL_ROOT_DIR}/lib")
if( NOT MSVC )
  find_library(LIB_PTHREAD NAME pthread HINTS "/usr/lib/x86_64-linux-gnu")
endif( NOT MSVC )


if ( ${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
    include_directories(
            "../keychain_lib/include"
            "../keychain_linux/passentry_cmd"
            "../libraries/fc_light/include"
            "../libraries/ethereum/include"
            "../libraries/secp256k1/include"
            "../libraries/secp256k1_ext/include"
            "${OPENSSL_ROOT_DIR}/include"
            "/usr/include/libcxxabi"
            ${Boost_INCLUDE_DIR})

    file(GLOB KEYCHAIN_SOURCES "./*.cpp" "../keychain_linux/passentry_cmd/*.cpp" )
    file(GLOB KEYCHAIN_INCLUDE "./*.hpp" "../keychain_linux/passentry_cmd/*.hpp" )

    find_package(X11 REQUIRED)
    if(NOT X11_FOUND)
        message(SEND_ERROR "Failed to find X11")
        return()
    else()
        include_directories(${X11_INCLUDE_DIR})
    endif()
    find_library(LIB_XI NAME Xi HINTS "/usr/lib/x86_64-linux-gnu/")
    add_executable(keychain ${KEYCHAIN_SOURCES} ${KEYCHAIN_INCLUDE} )
    target_link_libraries(keychain keychain_common ${LIB_PTHREAD} fc_light ether ${LIB_OPENSSL} ${Boost_LIBRARIES} ${X11_LIBRARIES} ${LIB_XI})

    set(GUI_BINARY ${CMAKE_BINARY_DIR}/keychain_linux/passentry_gui/passentry_gui)
    set(KEYCHAIN_BINARY_DIR ${CMAKE_BINARY_DIR}/keychain_cmd_app)
    ADD_CUSTOM_COMMAND(TARGET keychain
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${GUI_BINARY} ${KEYCHAIN_BINARY_DIR}
            )
else()
    include_directories(
            "../keychain_lib/include"
            "../libraries/fc_light/include"
            "../libraries/ethereum/include"
            "../libraries/secp256k1/include"
            "../libraries/secp256k1_ext/include"
            "${OPENSSL_ROOT_DIR}/include"
            ${Boost_INCLUDE_DIR})

    file(GLOB KEYCHAIN_SOURCES "./*.cpp"  )
    file(GLOB KEYCHAIN_INCLUDE "./*.hpp"  )
    add_executable(keychain ${KEYCHAIN_SOURCES} ${KEYCHAIN_INCLUDE} )
    target_link_libraries(keychain keychain_common fc_light ether ${LIB_OPENSSL} ${Boost_LIBRARIES})

endif()
