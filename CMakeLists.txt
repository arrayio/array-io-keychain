project( KeychainApp )
cmake_minimum_required( VERSION 3.0.2 )

add_subdirectory(./keychain_lib)
add_subdirectory(./libraries/secp256k1_ext)
add_subdirectory(./libraries/ethereum)
add_subdirectory(./libraries/fc_light)

if( MSVC )
  add_subdirectory(./keychain_service_win)
endif ( MSVC )
add_subdirectory(./keychain_cmd_app)

if ( ${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
  add_subdirectory(./keychain_linux/passentry_gui)
endif()
