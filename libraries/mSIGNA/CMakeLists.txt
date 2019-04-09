set(CMAKE_CXX_FLAGS "-std=c++14")

if(MSVC)
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  /EHsc" )
    #	set( CMAKE_VS_PLATFORM_TOOLSET "LLVM-vs2017")
endif()

if (NOT MSVC)
    if ( LIBCXX_BUILD )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -stdlib=libc++" )
    endif()
endif()

include_directories("${CMAKE_SOURCE_DIR}/libraries/mSIGNA/include"
        "${OPENSSL_ROOT_DIR}/include")

file(GLOB MSIGNA_HEADERS "./include/*.h")
file(GLOB MSIGNA_SOURCES "*.cpp" )


add_library( msigna ${MSIGNA_SOURCES} ${MSIGNA_HEADERS} )
