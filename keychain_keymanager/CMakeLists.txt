project( keychain_keymanager )

cmake_minimum_required( VERSION 3.0.2 )

if (MSVC)
	set(CMAKE_CXX_FLAGS "-std=c++14 /EHsc")
	set(CMAKE_EXE_LINKER_FLAGS "/SUBSYSTEM:WINDOWS")
	set(CMAKE_WIN32_EXECUTABLE ON)
else()
	set(CMAKE_CXX_FLAGS "-std=c++14")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/keychain_keymanager/src")
set(AUTOGEN_BUILD_DIR "${CMAKE_SOURCE_DIR}/keychain_keymanager/GeneratedFiles")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core)
find_package(Qt5Qml)
find_package(Qt5Svg)

get_target_property(QtCore_location Qt5::Core LOCATION)

add_definitions(-DUNICODE -D_UNICODE)

QT5_ADD_RESOURCES(RESOURCES "${CMAKE_SOURCE_DIR}/keychain_keymanager/Resources/*.qrc")
QT5_WRAP_UI(MOC_FILES "${CMAKE_SOURCE_DIR}/keychain_keymanager/src/*.ui")
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Core_INCLUDES})
include_directories(${Qt5Qml_INCLUDES})
include_directories(${Qt5Svg_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})

SET(BOOST_COMPONENTS)
LIST(APPEND BOOST_COMPONENTS log date_time system filesystem program_options signals serialization chrono unit_test_framework locale thread exception iostreams)
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

include_directories("${CMAKE_SOURCE_DIR}/keychain_keymanager/include"
			"${AUTOGEN_BUILD_DIR}"
            "${CMAKE_SOURCE_DIR}/libraries/fc_light/include"
			"${CMAKE_SOURCE_DIR}/keychain_lib/include"
			"${CMAKE_SOURCE_DIR}/libraries/kaitai_struct_cpp_stl_runtime"
			${Boost_INCLUDE_DIR})
file(GLOB KEYCHAIN_KEYMANAGER_SOURCES "${CMAKE_SOURCE_DIR}/keychain_keymanager/src/*.cpp")
file(GLOB KEYCHAIN_KEYMANAGER_INCLUDES "${CMAKE_SOURCE_DIR}/keychain_keymanager/include/*.hpp")
file(GLOB KEYCHAIN_KEYMANAGER_RESOURCES "${CMAKE_SOURCE_DIR}/keychain_keymanager/Resources/*.qrc")

add_executable(keychain_keymanager ${KEYCHAIN_KEYMANAGER_SOURCES} ${KEYCHAIN_KEYMANAGER_INCLUDES} ${KEYCHAIN_KEYMANAGER_RESOURCES})

if(MSVC)
	target_link_libraries(keychain_keymanager Crypt32.lib Qt5::Widgets Qt5::Core Qt5::Qml Qt5::Svg keychain_common secmodlib kaitai_struct_cpp_stl_runtime fc_light ${Boost_LIBRARIES} ${LIB_OPENSSL})
else()
	target_link_libraries(keychain_keymanager Qt5::Widgets Qt5::Core Qt5::Qml Qt5::Svg keychain_common secmodlib kaitai_struct_cpp_stl_runtime fc_light ${Boost_LIBRARIES} ${LIB_OPENSSL})
endif()

if(MSVC)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(BUILD_TYPE_PATH Debug )
		set(CMAKE_CXX_FLAGS "/EHsc")
	else(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(BUILD_TYPE_PATH Release )
	endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
endif()