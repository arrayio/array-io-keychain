project( KeychainApp )
cmake_minimum_required( VERSION 3.0.2 )

add_subdirectory(./keychain_lib)
add_subdirectory(./libraries/secp256k1)
add_subdirectory(./array-io-core)
add_subdirectory(./libraries/fc_light)
add_subdirectory(./libraries/ethereum)
add_subdirectory(./keychain_linux/passentry_gui)

if( MSVC )
  add_subdirectory(./keychain_service_win)
endif ( MSVC )
add_subdirectory(./keychain_cmd_app)
