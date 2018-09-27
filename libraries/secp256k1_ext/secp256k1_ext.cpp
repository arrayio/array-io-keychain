//
// Created by user on 27.09.18.
//

#include "secp256k1_ext.hpp"
#include "secp256k1.c"


bool is_canonical_( const std::array<unsigned char, 65>& c )
{
    return !(c[1] & 0x80)
           && !(c[1] == 0 && !(c[2] & 0x80))
           && !(c[33] & 0x80)
           && !(c[33] == 0 && !(c[34] & 0x80));
}

static int extended_nonce_function(
        unsigned char *nonce32,
        const unsigned char *msg32,
        const unsigned char *key32,
        const unsigned char *algo16,
        void *data,
        unsigned int attempt
) {
    unsigned int* extra = (unsigned int*) data;
    (*extra)++;
    return secp256k1_nonce_function_default( nonce32, msg32, key32, NULL, NULL, *extra);
}

int secp256k1_ecdsa_sign_with_recid(
        const secp256k1_context* ctx,
        secp256k1_ecdsa_signature *signature,
        const unsigned char *msg32,
        const unsigned char *seckey,
        secp256k1_nonce_function noncefp,
        const void* noncedata,
        int * recid
) {
    secp256k1_scalar r, s;
    secp256k1_scalar sec,  msg, non;
    int ret = 0;
    int overflow = 0;
    VERIFY_CHECK(ctx != NULL);
    ARG_CHECK(secp256k1_ecmult_gen_context_is_built(&ctx->ecmult_gen_ctx));
    ARG_CHECK(msg32 != NULL);
    ARG_CHECK(signature != NULL);
    ARG_CHECK(seckey != NULL);

    if (noncefp == NULL) {
        noncefp  = extended_nonce_function;
    }

    secp256k1_scalar_set_b32(&sec, seckey, &overflow);
    /* Fail if the secret key is invalid. */
    if (!overflow && !secp256k1_scalar_is_zero(&sec)) {
        unsigned char nonce32[32];
        unsigned int count = 0;
        secp256k1_scalar_set_b32(&msg, msg32, NULL);
        while (1) {
            ret = noncefp(nonce32, msg32, seckey, NULL, (void*)noncedata, count);
            if (!ret) {
                break;
            }
            secp256k1_scalar_set_b32(&non, nonce32, &overflow);

            if (!overflow && !secp256k1_scalar_is_zero(&non)) {
                if (secp256k1_ecdsa_sig_sign(&ctx->ecmult_gen_ctx, &r, &s, &sec, &msg, &non, recid)) {
                    break;
                }
            }
            count++;
        }
        memset(nonce32, 0, 32);
        secp256k1_scalar_clear(&msg);
        secp256k1_scalar_clear(&non);
        secp256k1_scalar_clear(&sec);
    }
    if (ret) {
        secp256k1_scalar_get_b32(&signature->data[0], &r);
        secp256k1_scalar_get_b32(&signature->data[32], &s);
    } else {
        memset(signature, 0, sizeof(*signature));
    }
    return ret;
}

void sign_bitshares(
        std::array<unsigned char, 65> &signature,
        const unsigned char *msg32,
        const unsigned char *seckey
)
{
    int res_sig, recid = 0, counter=0;
    do{
        secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN |SECP256K1_CONTEXT_VERIFY );
        res_sig = secp256k1_ecdsa_sign_with_recid(
                ctx,
                (secp256k1_ecdsa_signature *) (signature.data() + 1),
                msg32,
                seckey,
                NULL,
                &counter,
                &recid
        );

        if (res_sig == 0)
            throw std::runtime_error("secp256k1_ecdsa_sign_with_recid()");
        secp256k1_context_destroy(ctx);
        signature.begin()[0] = 27 + 4 + recid;
    }while (!is_canonical_(signature));
}
