//
// Created by user on 04.09.18.
//

#ifndef KEYCHAINAPP_SIG_H
#define KEYCHAINAPP_SIG_H

#include "include/secp256k1.h"
#include "include/secp256k1_recovery.h"
#include <boost/multiprecision/cpp_int.hpp>

namespace keychain_app
{

    using Signature = std::array<uint8_t, 65>;
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

    secp256k1_context const* getCtx()
    {
        static std::unique_ptr<secp256k1_context, decltype(&secp256k1_context_destroy)> s_ctx{
                secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY),
                &secp256k1_context_destroy
        };
        return s_ctx.get();
    }

    Signature set_sign(char* _k, char* _hash)
    {
        auto* ctx = getCtx();
        secp256k1_ecdsa_recoverable_signature rawSig;
        if (!secp256k1_ecdsa_sign_recoverable(ctx, &rawSig, (const unsigned char *) _hash,
                                              (const unsigned char *) _k, nullptr, nullptr))
            return {};

        Signature s;
        int v = 0;
        secp256k1_ecdsa_recoverable_signature_serialize_compact(ctx, s.data(), &v, &rawSig);

        /*SignatureStruct& ss = *reinterpret_cast<SignatureStruct*>(&s);
        ss.v = static_cast<byte>(v);
        if (ss.s > c_secp256k1n / 2)
        {
            ss.v = static_cast<byte>(ss.v ^ 1);
            ss.s = h256(c_secp256k1n - u256(ss.s));
        }
        assert(ss.s <= c_secp256k1n / 2);
         */
        return s;
    }
}




#endif //KEYCHAINAPP_SIG_H
