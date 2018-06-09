project( KeychainApp )

add_subdirectory(./keychain_lib)
add_subdirectory(./array-io-core)
if( MSVC )
  add_subdirectory(./keychain_service_win)
endif ( MSVC )
add_subdirectory(./keychain_cmd_app)
