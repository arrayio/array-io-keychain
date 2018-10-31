project( KeychainCmdApp  )

cmake_minimum_required( VERSION 3.0.2 )

set(CMAKE_CXX_FLAGS "-std=c++14")

if(MSVC)
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  /EHsc" )
	set( CMAKE_VS_PLATFORM_TOOLSET "LLVM-vs2017")
endif()


if (NOT MSVC)
    if ( LIBCXX_BUILD )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -stdlib=libc++" )
    endif()
endif()
if(APPLE)
	SET(CMAKE_EXE_LINKER_FLAGS "-framework Cocoa")
endif()

SET(BOOST_COMPONENTS)
LIST(APPEND BOOST_COMPONENTS date_time system filesystem program_options signals serialization chrono unit_test_framework locale thread exception iostreams)
SET( Boost_USE_STATIC_LIBS ON CACHE STRING "ON or OFF" )

SET(BOOST_ROOT $ENV{BOOST_ROOT})
set(Boost_USE_MULTITHREADED ON)
set(BOOST_ALL_DYN_LINK OFF)

FIND_PACKAGE(Boost 1.65 REQUIRED COMPONENTS ${BOOST_COMPONENTS})
SET(Boost_LIBRARIES ${Boost_LIBRARIES})


if( NOT MSVC )
  find_library(LIB_OPENSSL NAME crypto HINTS "${OPENSSL_ROOT_DIR}/lib")
  find_library(LIB_PTHREAD NAME pthread HINTS "/usr/lib/x86_64-linux-gnu")
else()
  find_library(LIB_OPENSSL NAME libcrypto.lib HINTS "${OPENSSL_ROOT_DIR}/lib")
endif( NOT MSVC )


if ( ${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
    include_directories(
            "../keychain_lib/include"
            "../keychain_linux/passentry_cmd"
            "../libraries/fc_light/include"
            "../libraries/eth-crypto/include"
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
    add_dependencies(keychain passentry_gui)
    target_link_libraries(keychain keychain_common ${LIB_PTHREAD} fc_light eth-crypto ${LIB_OPENSSL} ${Boost_LIBRARIES} ${X11_LIBRARIES} ${LIB_XI})

    set(GUI_BINARY ${CMAKE_BINARY_DIR}/keychain_linux/passentry_gui/passentry_gui)
    set(KEYCHAIN_BINARY_DIR ${CMAKE_BINARY_DIR}/keychain_cmd_app/passentry_gui)

    ADD_CUSTOM_COMMAND(TARGET keychain
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E  copy_if_different   ${GUI_BINARY} ${KEYCHAIN_BINARY_DIR}
            )
else()
    include_directories(
            "../keychain_lib/include"
            "../libraries/fc_light/include"
            "../libraries/eth-crypto/include"
            "../libraries/secp256k1/include"
            "../libraries/secp256k1_ext/include"
            "${OPENSSL_ROOT_DIR}/include"
            ${Boost_INCLUDE_DIR})

    file(GLOB KEYCHAIN_SOURCES "./*.cpp"  )
    file(GLOB KEYCHAIN_INCLUDE "./*.hpp"  )
    if(APPLE) 
	file(GLOB KEYCHAIN_INCLUDE "./*.h*"  )
	file(GLOB KEYCHAIN_SOURCES ("./*.m*" "./*.cpp" ) )
    endif()
    add_executable(keychain ${KEYCHAIN_SOURCES} ${KEYCHAIN_INCLUDE} )

	if (MSVC)
		target_link_libraries(keychain keychain_common fc_light eth-crypto ${LIB_OPENSSL} ${Boost_LIBRARIES} Userenv.lib)
	else()
		target_link_libraries(keychain keychain_common fc_light eth-crypto ${LIB_OPENSSL} ${Boost_LIBRARIES})
		if(APPLE) 
		set_property (TARGET keychain APPEND_STRING PROPERTY 
              	COMPILE_FLAGS "-fobjc-arc")
		set_target_properties(
    		${APP_NAME}
		PROPERTIES
    		MACOSX_BUNDLE YES
    		XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES  
		)
		endif()
	endif()
endif()
