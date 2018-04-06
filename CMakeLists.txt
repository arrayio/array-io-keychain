project( KeychainApp )

cmake_minimum_required( VERSION 3.0.2 )

add_compile_options(-std=c++11)

include_directories("./include" "/opt/boostlib/include" "/opt/openssllib/include" "./3rd-party-components/include")

find_library(FCLIB name fc HINTS "~/blockchain/3rd-party-components/libs")
file(GLOB KEYCHAIN_SOURCES "./src/*.cpp")
file(GLOB KEYCHAIN_INCLUDE "./include/*.hpp")

add_executable(keychain ${KEYCHAIN_SOURCES})

#target_link_libraries(keychain fc)
