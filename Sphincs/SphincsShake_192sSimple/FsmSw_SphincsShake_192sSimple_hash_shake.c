/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsShake_192sSimple
*    includes the modules for SphincsShake_192sSimple
 ** @{ */
/** \addtogroup SphincsShake_192sSimple_hash
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsShake_192sSimple_hash_shake.c
* \brief  description of FsmSw_SphincsShake_192sSimple_hash_shake.c
*
* \details
*
*
*/
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
 **********************************************************************************************************************/

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_CommonLib.h"
#include "FsmSw_Fips202.h"
#include "FsmSw_SphincsShake_192sSimple_params.h"
#include "FsmSw_SphincsShake_192sSimple_utils.h"
#include "FsmSw_Sphincs_shake_address.h"
#include "FsmSw_Sphincs_utils.h"

#include "FsmSw_SphincsShake_192sSimple_hash.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define SPX_TREE_BITS  (FSMSW_SPHINCSSHAKE_192SSIMPLE_TREE_HEIGHT * (FSMSW_SPHINCSSHAKE_192SSIMPLE_D - 1u))
#define SPX_TREE_BYTES ((SPX_TREE_BITS + 7u) / 8u)
#define SPX_LEAF_BITS  FSMSW_SPHINCSSHAKE_192SSIMPLE_TREE_HEIGHT
#define SPX_LEAF_BYTES ((SPX_LEAF_BITS + 7u) / 8u)
#define SPX_DGST_BYTES (FSMSW_SPHINCSSHAKE_192SSIMPLE_FORS_MSG_BYTES + SPX_TREE_BYTES + SPX_LEAF_BYTES)
/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL CONSTANTS                                                                                                   */
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

/*====================================================================================================================*/
/**
 * \brief Computes PRF(pk_seed, sk_seed, addr).
 *
 * \param[out] uint8                        *out : t.b.d.
 * \param[in]  const sphincs_shake_192s_ctx *ctx : t.b.d.
 * \param[in]  const uint32              addr[8] : t.b.d.
 *
 */
void FsmSw_SphincsShake_192sSimple_PrfAddr(uint8 *out, const sphincs_shake_192s_ctx *ctx, const uint32 addr[8])
{
  uint8 buf[(2u * FSMSW_SPHINCSSHAKE_192SSIMPLE_N) + FSMSW_SPHINCSSHAKE_192SSIMPLE_ADDR_BYTES] = {0};

  FsmSw_CommonLib_MemCpy(buf, ctx->pub_seed, FSMSW_SPHINCSSHAKE_192SSIMPLE_N);
  FsmSw_CommonLib_MemCpy(&buf[FSMSW_SPHINCSSHAKE_192SSIMPLE_N], addr, FSMSW_SPHINCSSHAKE_192SSIMPLE_ADDR_BYTES);
  FsmSw_CommonLib_MemCpy(&buf[FSMSW_SPHINCSSHAKE_192SSIMPLE_N + FSMSW_SPHINCSSHAKE_192SSIMPLE_ADDR_BYTES], ctx->sk_seed,
                         FSMSW_SPHINCSSHAKE_192SSIMPLE_N);

  FsmSw_Fips202_Shake256(out, FSMSW_SPHINCSSHAKE_192SSIMPLE_N, buf,
                         (2u * FSMSW_SPHINCSSHAKE_192SSIMPLE_N) + FSMSW_SPHINCSSHAKE_192SSIMPLE_ADDR_BYTES);
} // end: FsmSw_SphincsShake_192sSimple_PrfAddr

/*====================================================================================================================*/
/**
 * \brief Computes the message-dependent randomness R, using a secret seed and an optional randomization value
 *        as well as the message.
 *
 * \param[out] uint8                          *R : t.b.d.
 * \param[in]  const uint8               *sk_prf : t.b.d.
 * \param[in]  const uint8              *optrand : t.b.d.
 * \param[in]  const uint8                    *m : t.b.d.
 * \param[in]  uint32                       mlen : t.b.d.
 * \param[in]  const sphincs_shake_192s_ctx *ctx : t.b.d.
 *
 */
void FsmSw_SphincsShake_192sSimple_GenMessageRandom(uint8 *R, const uint8 *sk_prf, const uint8 *optrand, const uint8 *m,
                                                    uint32 mlen, const sphincs_shake_192s_ctx *ctx)
{
  (void)ctx;
  shake256incctx s_inc = {{0}};

  FsmSw_Fips202_Shake256_IncInit(&s_inc);
  FsmSw_Fips202_Shake256_IncAbsorb(&s_inc, sk_prf, FSMSW_SPHINCSSHAKE_192SSIMPLE_N);
  FsmSw_Fips202_Shake256_IncAbsorb(&s_inc, optrand, FSMSW_SPHINCSSHAKE_192SSIMPLE_N);
  FsmSw_Fips202_Shake256_IncAbsorb(&s_inc, m, mlen);
  FsmSw_Fips202_Shake256_IncFinalize(&s_inc);
  FsmSw_Fips202_Shake256_IncSqueeze(R, FSMSW_SPHINCSSHAKE_192SSIMPLE_N, &s_inc);
} // end: FsmSw_SphincsShake_192sSimple_GenMessageRandom

/*====================================================================================================================*/
/**
 * \brief Computes the message hash using R, the public key, and the message. Outputs the message digest and the
 *        index of the leaf. The index is split in the tree index and the leaf index, for convenient copying to an
 *        address.
 *
 * \param[out] uint8                     *digest : t.b.d.
 * \param[out] uint64                      *tree : t.b.d.
 * \param[out] uint32                  *leaf_idx : t.b.d.
 * \param[in]  const uint8                    *R : t.b.d.
 * \param[in]  const uint8                   *pk : t.b.d.
 * \param[in]  const uint8                    *m : t.b.d.
 * \param[in]  uint32                       mlen : t.b.d.
 * \param[in]  const sphincs_shake_192s_ctx *ctx : t.b.d.
 *
 */
void FsmSw_SphincsShake_192sSimple_HashMessage(uint8 *digest, uint64 *tree, uint32 *leaf_idx, const uint8 *R,
                                               const uint8 *pk, const uint8 *m, uint32 mlen,
                                               const sphincs_shake_192s_ctx *ctx)
{
  (void)ctx;

  uint8 buf[SPX_DGST_BYTES] = {0};
  uint8 *bufp               = buf;
  shake256incctx s_inc      = {{0}};

  FsmSw_Fips202_Shake256_IncInit(&s_inc);
  FsmSw_Fips202_Shake256_IncAbsorb(&s_inc, R, FSMSW_SPHINCSSHAKE_192SSIMPLE_N);
  FsmSw_Fips202_Shake256_IncAbsorb(&s_inc, pk, FSMSW_SPHINCSSHAKE_192SSIMPLE_PK_BYTES);
  FsmSw_Fips202_Shake256_IncAbsorb(&s_inc, m, mlen);
  FsmSw_Fips202_Shake256_IncFinalize(&s_inc);
  FsmSw_Fips202_Shake256_IncSqueeze(buf, SPX_DGST_BYTES, &s_inc);

  FsmSw_CommonLib_MemCpy(digest, bufp, FSMSW_SPHINCSSHAKE_192SSIMPLE_FORS_MSG_BYTES);
  bufp = &bufp[FSMSW_SPHINCSSHAKE_192SSIMPLE_FORS_MSG_BYTES];

  *tree = FsmSw_Sphincs_BytesToUll(bufp, SPX_TREE_BYTES);
  *tree &= (~(uint64)0) >> (64u - SPX_TREE_BITS);
  bufp = &bufp[SPX_TREE_BYTES];

  *leaf_idx = (uint32)FsmSw_Sphincs_BytesToUll(bufp, SPX_LEAF_BYTES);
  *leaf_idx &= (~(uint32)0) >> (32u - SPX_LEAF_BITS);
} // end: FsmSw_SphincsShake_192sSimple_HashMessage

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
