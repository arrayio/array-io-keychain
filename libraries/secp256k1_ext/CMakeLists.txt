
# Configure secp256k1-zkp
project( Secp256k1_bitcon  )

add_definitions(-DENABLE_MODULE_RECOVERY -DENABLE_MODULE_ECDH)

set( SECP256K1_DIR "../secp256k1" )

file( GLOB SECP256K1_SOURCES
        "${SECP256K1_DIR}/src/ec*.c"
        "${SECP256K1_DIR}/src/gen*.c"
        "./secp256k1_ext.cpp"
        )

file( GLOB SECP256K1_HEADERS
        "${SECP256K1_DIR}/src/basic*.h"
        "${SECP256K1_DIR}/src/ec*.h"
        "${SECP256K1_DIR}/src/field*.h"
        "${SECP256K1_DIR}/src/g*.h"
        "${SECP256K1_DIR}/src/h*.h"
        "${SECP256K1_DIR}/src/n*.h"
        "${SECP256K1_DIR}/src/s*.h"
        "${SECP256K1_DIR}/src/u*.h"
        "./include/secp256k1_ext.hpp"
        )
add_library( secp256k1_btc ${SECP256K1_SOURCES} ${SECP256K1_HEADERS})

add_definitions( -DVERIFY )

target_include_directories( secp256k1_btc
        PRIVATE "${SECP256K1_DIR}"
        PUBLIC "${SECP256K1_DIR}/include"
        PUBLIC "./include"
        PUBLIC "${SECP256K1_DIR}/src"
        )

set( SECP256K1_BUILD_DEFINES
        USE_FIELD_10X26
        USE_FIELD_INV_BUILTIN
        USE_NUM_NONE
        USE_SCALAR_8X32
        USE_SCALAR_INV_BUILTIN )
set_target_properties( secp256k1_btc PROPERTIES COMPILE_DEFINITIONS "${SECP256K1_BUILD_DEFINES}" LINKER_LANGUAGE C )
# End configure secp256k1-zkp

