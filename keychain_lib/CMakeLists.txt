project( KeychainLib  )

cmake_minimum_required( VERSION 3.0.2 )

list( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/git_version_gen" )
include( GetGitRevisionDescription )
get_git_head_revision( GIT_REFSPEC KEYCHAIN_COMMON_GIT_REVISION )
get_git_unix_timestamp(KEYCHAIN_COMMON_GIT_REVISION_UNIX_TIMESTAMP)
git_describe(KEYCHAIN_COMMON_GIT_REVISION_DESCRIPTION --tags)

configure_file("${CMAKE_SOURCE_DIR}/keychain_lib/src/git_revision.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/git_revision.cpp" @ONLY)

set(CMAKE_CXX_FLAGS "-std=c++14")

if (NOT MSVC)
    if ( LIBCXX_BUILD )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -stdlib=libc++" )
    endif()
else()
#    set( CMAKE_VS_PLATFORM_TOOLSET "LLVM-vs2017")
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  /EHsc") 
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
            "${CMAKE_SOURCE_DIR}/libraries/kaitai_struct_cpp_stl_runtime"
            "${OPENSSL_ROOT_DIR}/include"
            ${Boost_INCLUDE_DIR}
            "../libraries/sqlite"
            "${CMAKE_SOURCE_DIR}/libraries/mSIGNA/include"
            )
else ( MSVC )
    include_directories(
            "${CMAKE_SOURCE_DIR}/libraries/kaitai_struct_cpp_stl_runtime"
            "./include/keychain_lib"
            "../libraries/fc_light/include"
            "../libraries/secp256k1/include"
            "../libraries/secp256k1_ext/include"
            "../libraries/eth-crypto/include"
#            "../libraries/eth-crypto/ethash/src/libethash"
            "${OPENSSL_ROOT_DIR}/include"
            "/usr/include/libcxxabi"
            ${Boost_INCLUDE_DIR}
            "../libraries/sqlite"
            "${CMAKE_SOURCE_DIR}/libraries/mSIGNA/include"
            )
endif( MSVC )


file(GLOB KEYCHAIN_LIB_SOURCES "./src/*.cpp" )
file(GLOB KEYCHAIN_LIB_INCLUDE "./include/keychain_lib/*.hpp" )
list(APPEND KEYCHAIN_LIB_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/git_revision.cpp")

add_library(keychain_common STATIC ${KEYCHAIN_LIB_SOURCES} ${KEYCHAIN_LIB_INCLUDE})
target_link_libraries(keychain_common  kaitai_struct_cpp_stl_runtime sqlite3 cryptopp msigna)


