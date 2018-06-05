cmake_minimum_required(VERSION 3.10rr)
project(passentry)

set(CMAKE_CXX_STANDARD 11)

set (CMAKE_EXE_LINKER_FLAGS "--verbose")

find_package(X11 REQUIRED)
if(NOT X11_FOUND)
    message(SEND_ERROR "Failed to find X11")
    return()
else()
    include_directories(${X11_INCLUDE_DIR})
endif()

find_library(LIB_XI NAME Xi HINTS "/usr/lib/x86_64-linux-gnu/")

set(SOURCE_LIB "./keysym2ucs.cpp")

add_executable(passentry main.cpp ${SOURCE_LIB})

target_link_libraries(passentry    ${X11_LIBRARIES} ${LIB_XI})

