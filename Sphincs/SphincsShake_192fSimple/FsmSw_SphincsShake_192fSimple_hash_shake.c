/***********************************************************************************************************************
*
*                                          IAV GmbH
*
***********************************************************************************************************************/
/*
*
*  $File$
*
*  $Author$
*
*  $Date$
*
*  $Rev$
*
***********************************************************************************************************************/

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Fips202.h"
#include "FsmSw_CommonLib.h"
#include "FsmSw_Sphincs_shake_address.h"
#include "FsmSw_Sphincs_utils.h"
#include "FsmSw_SphincsShake_192fSimple_hash.h"
#include "FsmSw_SphincsShake_192fSimple_params.h"
#include "FsmSw_SphincsShake_192fSimple_utils.h"
#include "FsmSw_SphincsShake_192fSimple_hash.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define SPX_TREE_BITS    (FSMSW_SPHINCSSHAKE_192FSIMPLE_TREE_HEIGHT * (FSMSW_SPHINCSSHAKE_192FSIMPLE_D - 1u))
#define SPX_TREE_BYTES  ((SPX_TREE_BITS + 7u) / 8u)
#define SPX_LEAF_BITS     FSMSW_SPHINCSSHAKE_192FSIMPLE_TREE_HEIGHT
#define SPX_LEAF_BYTES  ((SPX_LEAF_BITS + 7u) / 8u)
#define SPX_DGST_BYTES   (FSMSW_SPHINCSSHAKE_192FSIMPLE_FORS_MSG_BYTES + SPX_TREE_BYTES + SPX_LEAF_BYTES)

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        FsmSw_SphincsShake_192fSimple_prf_addr
*
* Description: Computes PRF(pk_seed, sk_seed, addr).
*
* Arguments:   -       uint8                  *out:     t.b.d.
*              - const sphincs_shake_192f_ctx *ctx:     t.b.d.
*              - const uint32                  addr[8]: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsShake_192fSimple_prf_addr(uint8 *out, const sphincs_shake_192f_ctx *ctx, const uint32 addr[8])
{
    uint8 buf[2u * FSMSW_SPHINCSSHAKE_192FSIMPLE_N + FSMSW_SPHINCSSHAKE_192FSIMPLE_ADDR_BYTES];

    FsmSw_CommonLib_memcpy(buf, ctx->pub_seed, FSMSW_SPHINCSSHAKE_192FSIMPLE_N);
    FsmSw_CommonLib_memcpy(&buf[FSMSW_SPHINCSSHAKE_192FSIMPLE_N], addr, FSMSW_SPHINCSSHAKE_192FSIMPLE_ADDR_BYTES);
    FsmSw_CommonLib_memcpy(&buf[FSMSW_SPHINCSSHAKE_192FSIMPLE_N + FSMSW_SPHINCSSHAKE_192FSIMPLE_ADDR_BYTES],
                           ctx->sk_seed, FSMSW_SPHINCSSHAKE_192FSIMPLE_N);

    FsmSw_Fips202_shake256(out, FSMSW_SPHINCSSHAKE_192FSIMPLE_N, buf,
                           2u * FSMSW_SPHINCSSHAKE_192FSIMPLE_N + FSMSW_SPHINCSSHAKE_192FSIMPLE_ADDR_BYTES);
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsShake_192fSimple_gen_message_random
*
* Description: Computes the message-dependent randomness R, using a secret seed and an optional randomization value
*              as well as the message.
*
* Arguments:   -       uint8                  *R:       t.b.d.
*              - const uint8                  *sk_prf:  t.b.d.
*              - const uint8                  *optrand: t.b.d.
*              - const uint8                  *m:       t.b.d.
*              -       uint32                  mlen:    t.b.d.
*              - const sphincs_shake_192f_ctx *ctx:     t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsShake_192fSimple_gen_message_random(uint8 *R, const uint8 *sk_prf, const uint8 *optrand,
                                                      const uint8 *m, uint32 mlen, const sphincs_shake_192f_ctx *ctx)
{
    (void)ctx;
    shake256incctx s_inc;

    FsmSw_Fips202_shake256_inc_init(&s_inc);
    FsmSw_Fips202_shake256_inc_absorb(&s_inc, sk_prf, FSMSW_SPHINCSSHAKE_192FSIMPLE_N);
    FsmSw_Fips202_shake256_inc_absorb(&s_inc, optrand, FSMSW_SPHINCSSHAKE_192FSIMPLE_N);
    FsmSw_Fips202_shake256_inc_absorb(&s_inc, m, mlen);
    FsmSw_Fips202_shake256_inc_finalize(&s_inc);
    FsmSw_Fips202_shake256_inc_squeeze(R, FSMSW_SPHINCSSHAKE_192FSIMPLE_N, &s_inc);
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsShake_192fSimple_hash_message
*
* Description: Computes the message hash using R, the public key, and the message. Outputs the message digest and the
*              index of the leaf. The index is split in the tree index and the leaf index, for convenient copying to an
*              address.
*
* Arguments:   -       uint8                  *digest:   t.b.d.
*              -       uint64                 *tree:     t.b.d.
*              -       uint32                 *leaf_idx: t.b.d.
*              - const uint8                  *R:        t.b.d.
*              - const uint8                  *pk:       t.b.d.
*              - const uint8                  *m:        t.b.d.
*              -       uint32                  mlen:     t.b.d.
*              - const sphincs_shake_192f_ctx *ctx:      t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsShake_192fSimple_hash_message(uint8 *digest, uint64 *tree, uint32 *leaf_idx, const uint8 *R,
                                                const uint8 *pk, const uint8 *m, uint32 mlen,
                                                const sphincs_shake_192f_ctx *ctx)
{
    (void)ctx;

    uint8 buf[SPX_DGST_BYTES];
    uint8 *bufp = buf;
    shake256incctx s_inc;

    FsmSw_Fips202_shake256_inc_init(&s_inc);
    FsmSw_Fips202_shake256_inc_absorb(&s_inc, R, FSMSW_SPHINCSSHAKE_192FSIMPLE_N);
    FsmSw_Fips202_shake256_inc_absorb(&s_inc, pk, FSMSW_SPHINCSSHAKE_192FSIMPLE_PK_BYTES);
    FsmSw_Fips202_shake256_inc_absorb(&s_inc, m, mlen);
    FsmSw_Fips202_shake256_inc_finalize(&s_inc);
    FsmSw_Fips202_shake256_inc_squeeze(buf, SPX_DGST_BYTES, &s_inc);

    FsmSw_CommonLib_memcpy(digest, bufp, FSMSW_SPHINCSSHAKE_192FSIMPLE_FORS_MSG_BYTES);
    bufp = &bufp[FSMSW_SPHINCSSHAKE_192FSIMPLE_FORS_MSG_BYTES];

    *tree = FsmSw_Sphincs_bytes_to_ull(bufp, SPX_TREE_BYTES);
    *tree &= (~(uint64)0) >> (64u - SPX_TREE_BITS);
    bufp = &bufp[SPX_TREE_BYTES];

    *leaf_idx = (uint32)FsmSw_Sphincs_bytes_to_ull(bufp, SPX_LEAF_BYTES);
    *leaf_idx &= (~(uint32)0) >> (32u - SPX_LEAF_BITS);
}
