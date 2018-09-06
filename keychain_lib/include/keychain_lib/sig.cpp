//
// Created by user on 05.09.18.
//
#include "sig.hpp"

namespace keychain_app
{
/*
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

        SignatureStruct& ss = *reinterpret_cast<SignatureStruct*>(&s);
        ss.v = static_cast<byte>(v);
        if (ss.s > c_secp256k1n / 2)
        {
            ss.v = static_cast<byte>(ss.v ^ 1);
            ss.s = h256(c_secp256k1n - u256(ss.s));
        }
        assert(ss.s <= c_secp256k1n / 2);

        return s;
    }
*/
}