project( KeychainApp )
cmake_minimum_required( VERSION 3.0.2 )

if (NOT MSVC)
  if ( LIBCXX_BUILD )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -stdlib=libc++" )
  endif()
endif()

add_subdirectory(./keychain_lib)
add_subdirectory(./libraries/secp256k1_ext)
add_subdirectory(./libraries/eth-crypto)
add_subdirectory(./libraries/fc_light)
add_subdirectory(./keychain_cmd_app/secmodlib)

if( MSVC )
  add_subdirectory(./keychain_service_win)
endif ( MSVC )
add_subdirectory(./keychain_cmd_app)

if ( ${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
  add_subdirectory(./keychain_linux/passentry_gui)
endif()
