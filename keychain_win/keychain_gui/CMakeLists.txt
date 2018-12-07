project( keychain_win_gui  )

cmake_minimum_required( VERSION 3.0.2 )

set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_EXE_LINKER_FLAGS "/SUBSYSTEM:WINDOWS")

SET(BOOST_COMPONENTS)
SET( Boost_USE_STATIC_LIBS ON CACHE STRING "ON or OFF" )


FIND_PACKAGE(Boost 1.65 REQUIRED COMPONENTS ${BOOST_COMPONENTS})
SET(BOOST_LIBRARIES_TEMP ${Boost_LIBRARIES})
FIND_PACKAGE(Boost 1.65 REQUIRED COMPONENTS coroutine)
LIST(APPEND BOOST_COMPONENTS coroutine)
SET(Boost_LIBRARIES ${BOOST_LIBRARIES_TEMP} ${Boost_LIBRARIES})

add_definitions(-DUNICODE -D_UNICODE)

file(GLOB KEYCHAIN_GUI_SOURCES "./src/*.cpp")
file(GLOB KEYCHAIN_GUI_INCLUDES "./include/*.h")
file(GLOB KEYCHAIN_GUI_RESOURCES "./*.rc")

add_executable(keychain_win_gui ${KEYCHAIN_GUI_SOURCES} ${KEYCHAIN_GUI_INCLUDES} ${KEYCHAIN_GUI_RESOURCES})
target_include_directories( keychain_win_gui PRIVATE "./include/")
        
target_link_libraries(keychain_win_gui Crypt32.lib)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(BUILD_TYPE_PATH Debug )
	set(CMAKE_CXX_FLAGS "/EHsc")
else(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(BUILD_TYPE_PATH Release )
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

add_custom_command(TARGET keychain_win_gui 
				   POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
				   ${CMAKE_CURRENT_BINARY_DIR}/${BUILD_TYPE_PATH}/keychain_win_gui.exe
				   ../keychain_service/service_installer/wsserverinstaller/wsservice/keychain_win_gui.exe)

