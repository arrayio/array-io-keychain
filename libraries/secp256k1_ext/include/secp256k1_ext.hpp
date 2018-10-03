#ifndef SECP256K1_EXT_H
#define SECP256K1_EXT_H

#include "secp256k1.h"
#include <array>

void sign_bitshares(
        std::array<unsigned char, 65> &signature,
        const unsigned char *msg32,
        const unsigned char *seckey
        );

#endif //SECP256K1_EXT_H