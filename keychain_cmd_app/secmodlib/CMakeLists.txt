project( secmodlib  )

cmake_minimum_required( VERSION 3.0.2 )

set(CMAKE_CXX_FLAGS "-std=c++14")

add_definitions(-DUNICODE -D_UNICODE)

if (NOT MSVC)
    if ( LIBCXX_BUILD )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -stdlib=libc++" )
    endif()
else()
    set( CMAKE_VS_PLATFORM_TOOLSET "LLVM-vs2017")
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Z7 /EHsc") 
endif()

if( NOT MSVC )
  find_library(LIB_OPENSSL NAME crypto HINTS "${OPENSSL_ROOT_DIR}/lib")
  find_library(LIB_PTHREAD NAME pthread HINTS "/usr/lib/x86_64-linux-gnu")
else()
  find_library(LIB_OPENSSL NAME libcrypto.lib HINTS "${OPENSSL_ROOT_DIR}/lib")
endif( NOT MSVC )

include_directories("./include"
					"../../keychain_lib/include"
				    "../../libraries/fc_light/include"
					"../../libraries/eth-crypto/include"
					"../../libraries/secp256k1/include"
					"../../libraries/secp256k1_ext/include"
					"${OPENSSL_ROOT_DIR}/include"
					)

file(GLOB SECMODLIB_SOURCES "./src/*.cpp" )
file(GLOB SECMODLIB_INCLUDE "./include/secmodlib/*.hpp" "./include/secmodlib/*/*.hpp")

#target_link_libraries(keychain_common)
add_library(secmodlib STATIC ${SECMODLIB_SOURCES} ${SECMODLIB_INCLUDE})