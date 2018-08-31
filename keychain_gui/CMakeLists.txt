cmake_minimum_required(VERSION 3.1.0)

project(KeychainGuiApp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(CMAKE_PREFIX_PATH "/home/igor/Qt/5.11.1/gcc_64/lib/cmake/")

set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Widgets Qml Quick WebEngine REQUIRED)

find_package(Qt5QuickCompiler)

qtquick_compiler_add_resources(RESOURCES qml.qrc)

add_definitions(${Qt5Widgets_DEFINITIONS} ${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}})

file(GLOB QMLSOURCES "*.qml")

file(GLOB QMLRESOURCES "*.qrc")

set(SOURCES
    main.cpp)

add_custom_target(Qml SOURCES ${QMLSOURCES} ${QMLRESOURCES})

add_executable(KeychainGuiApp ${SOURCES} ${RESOURCES})

target_link_libraries(KeychainGuiApp Qt5::Widgets Qt5::Qml Qt5::Quick Qt5::WebEngine)
