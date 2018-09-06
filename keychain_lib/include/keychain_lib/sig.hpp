//
// Created by user on 04.09.18.
//

#pragma once

//#include "../../../libraries/secp256k1/include/secp256k1.h"
//#include "../../../libraries/secp256k1/include/secp256k1_recovery.h"
//#include <boost/multiprecision/cpp_int.hpp>
//#include "../../../libraries/ethereum/include/ethereum/crypto/Common.h"

namespace keychain_app
{

/*    using Signature = std::array<uint8_t, 65>;
//    using h2048     = std::array<uint8_t, 256>;
//    using h1024     = std::array<uint8_t, 128>;
    using h520      = std::array<uint8_t, 65>;
//    using h512      = std::array<uint8_t, 64>;
    using h256      = std::array<uint8_t, 32>;
    using byte      = uint8_t;
    using u256 =  boost::multiprecision::number<boost::multiprecision::cpp_int_backend<256, 256, boost::multiprecision::unsigned_magnitude, boost::multiprecision::unchecked, void>>;
    static const u256 c_secp256k1n("115792089237316195423570985008687907852837564279074904382605163141518161494337");

    struct SignatureStruct
    {
        SignatureStruct() = default;
        SignatureStruct(Signature const& _s) { *(h520*)this = _s; }
        SignatureStruct(h256 const& _r, h256 const& _s, byte _v): r(_r), s(_s), v(_v) {}
        operator Signature() const { return *(h520 const*)this; }

        /// @returns true if r,s,v values are valid, otherwise false
        bool isValid() const noexcept;

        h256 r;
        h256 s;
        byte v = 0;
    };

    secp256k1_context const* getCtx_();
    Signature set_sign(char* _k, char* _hash);
    */
}





