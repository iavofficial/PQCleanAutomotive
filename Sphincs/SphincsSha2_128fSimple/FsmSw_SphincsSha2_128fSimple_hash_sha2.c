/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsSha2_128fSimple
*    includes the modules for SphincsSha2_128fSimple
 ** @{ */
/** \addtogroup SphincsSha2_128fSimple_hash
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsSha2_128fSimple_hash_sha2.c
* \brief  description of FsmSw_SphincsSha2_128fSimple_hash_sha2.c
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
#include "FsmSw_SphincsSha2_128fSimple_params.h"
#include "FsmSw_Sphincs_sha2_address.h"
#include "FsmSw_Sphincs_utils.h"
#include "FsmSw_sha2.h"

#include "FsmSw_SphincsSha2_128fSimple_hash.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define SPX_SHAX_OUTPUT_BYTES FSMSW_SPHINCS_SHA256_OUTPUT_BYTES
#define SPX_SHAX_BLOCK_BYTES  FSMSW_SPHINCS_SHA256_BLOCK_BYTES
#define shaX_inc_init         FsmSw_Sha256_IncInit
#define shaX_inc_blocks       FsmSw_Sha256_IncBlocks
#define shaX_inc_finalize     FsmSw_Sha256_IncFinalize
#define shaX                  FsmSw_Sha256
#define mgf1_X                FsmSw_SphincsSha2_128fSimple_MgF1_256
#define shaXstate             sha256ctx

#define SPX_TREE_BITS  (FSMSW_SPHINCSSHA2_128FSIMPLE_TREE_HEIGHT * (FSMSW_SPHINCSSHA2_128FSIMPLE_D - 1u))
#define SPX_TREE_BYTES ((SPX_TREE_BITS + 7u) / 8u)
#define SPX_LEAF_BITS  FSMSW_SPHINCSSHA2_128FSIMPLE_TREE_HEIGHT
#define SPX_LEAF_BYTES ((SPX_LEAF_BITS + 7u) / 8u)
#define SPX_DGST_BYTES (FSMSW_SPHINCSSHA2_128FSIMPLE_FORS_MSG_BYTES + SPX_TREE_BYTES + SPX_LEAF_BYTES)
/* Round to nearest multiple of SPX_SHAX_BLOCK_BYTES */
/* polyspace +1 MISRA2012:12.4 [Justified:]"Necessary controlled type conversions for block calculation." */
#define SPX_INBLOCKS                                                                                                   \
  (uint32)((uint32)((uint32)(((FSMSW_SPHINCSSHA2_128FSIMPLE_N + FSMSW_SPHINCSSHA2_128FSIMPLE_PK_BYTES +                \
                               SPX_SHAX_BLOCK_BYTES - 1u) &                                                            \
                              (uint32)((sint32)((-1) * (sint32)SPX_SHAX_BLOCK_BYTES)))) /                              \
                    SPX_SHAX_BLOCK_BYTES))
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
static void FsmSw_SphincsSha2_128fSimple_MgF1_256(uint8 *out, uint32 outlen, const uint8 *in, uint32 inlen);
/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
 * \brief FsmSw_SphincsSha2_128fSimple_MgF1_256 function based on the SHA-256 hash function. Note that inlen
 *        should be sufficiently small that it still allows for an array to be allocated on the stack.
 *        Typically 'in' is merely a seed. Outputs outlen number of bytes.
 *
 * \param[out] uint8      *out : t.b.d.
 * \param[out] uint32   outlen : t.b.d.
 * \param[in]  const uint8 *in : t.b.d.
 * \param[in]  uint32    inlen : t.b.d.
 */
static void FsmSw_SphincsSha2_128fSimple_MgF1_256(uint8 *out, uint32 outlen, const uint8 *in, uint32 inlen)
{
  uint8 inbuf[FSMSW_SPHINCSSHA2_128FSIMPLE_MGF1_256_BUF_LEN + 4] = {0};
  uint8 outbuf[FSMSW_SPHINCS_SHA256_OUTPUT_BYTES]                = {0};
  uint32 i                                                       = 0;

  /* out_temp is used to avoid modifying the input. */
  uint8 *out_temp = out;

  FsmSw_CommonLib_MemCpy(inbuf, in, inlen);

  /* While we can fit in at least another full block of SHA256 output.. */
  /* polyspace +2 MISRA2012:14.2 [Justified:]"The calculation involving the loop counter directly affects loop
  continuation, addressing a MISRA 14.2 warning by following its rules for how loops should work." */
  for (i = 0; ((i + 1u) * FSMSW_SPHINCS_SHA256_OUTPUT_BYTES) <= outlen; i++)
  {
    FsmSw_Sphincs_U32ToBytes(&inbuf[inlen], i);
    FsmSw_Sha256(out_temp, inbuf, inlen + 4u);
    out_temp = &out_temp[FSMSW_SPHINCS_SHA256_OUTPUT_BYTES];
  }
  /* Until we cannot anymore, and we fill the remainder. */
  if (outlen > (i * FSMSW_SPHINCS_SHA256_OUTPUT_BYTES))
  {
    FsmSw_Sphincs_U32ToBytes(&inbuf[inlen], i);
    FsmSw_Sha256(outbuf, inbuf, inlen + 4u);
    FsmSw_CommonLib_MemCpy(out_temp, outbuf, outlen - (i * FSMSW_SPHINCS_SHA256_OUTPUT_BYTES));
  }
} // end: FsmSw_SphincsSha2_128fSimple_MgF1_256

/*====================================================================================================================*/
/**
 * \brief mgf1 function based on the SHA-512 hash function
 *
 * \param[out] uint8      *out : t.b.d.
 * \param[out] uint32   outlen : t.b.d.
 * \param[in]  const uint8 *in : t.b.d.
 * \param[in]  uint32    inlen : t.b.d.
 *
 * Note: This function is currently not used.
 */
void FsmSw_SphincsSha2_128fSimple_Mgf1512(uint8 *out, uint32 outlen, const uint8 *in, uint32 inlen)
{
  uint8 inbuf[FSMSW_SPHINCSSHA2_128FSIMPLE_MGF1_512_BUF_LEN + 4] = {0};
  uint8 outbuf[FSMSW_SPHINCS_SHA512_OUTPUT_BYTES]                = {0};
  uint32 i                                                       = 0;

  /* out_temp is used to avoid modifying the input. */
  uint8 *out_temp = out;

  FsmSw_CommonLib_MemCpy(inbuf, in, inlen);

  /* While we can fit in at least another full block of SHA512 output.. */
  /* polyspace +2 MISRA2012:14.2 [Justified:]"The calculation involving the loop counter directly affects loop
  continuation, addressing a MISRA 14.2 warning by following its rules for how loops should work." */
  for (i = 0; ((i + 1u) * FSMSW_SPHINCS_SHA512_OUTPUT_BYTES) <= outlen; i++)
  {
    FsmSw_Sphincs_U32ToBytes(&inbuf[inlen], i);
    FsmSw_Sha512(out_temp, inbuf, inlen + 4u);
    out_temp = &out_temp[FSMSW_SPHINCS_SHA512_OUTPUT_BYTES];
  }
  /* Until we cannot anymore, and we fill the remainder. */
  if (outlen > (i * FSMSW_SPHINCS_SHA512_OUTPUT_BYTES))
  {
    FsmSw_Sphincs_U32ToBytes(&inbuf[inlen], i);
    FsmSw_Sha512(outbuf, inbuf, inlen + 4u);
    FsmSw_CommonLib_MemCpy(out_temp, outbuf, outlen - (i * FSMSW_SPHINCS_SHA512_OUTPUT_BYTES));
  }
} // end: FsmSw_SphincsSha2_128fSimple_Mgf1512
/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
 * \brief Computes PRF(pk_seed, sk_seed, addr).
 *
 * \param[out] uint8                       *out : t.b.d.
 * \param[in]  const sphincs_sha2_128f_ctx *ctx : t.b.d.
 * \param[in]  const uint32             addr[8] : t.b.d.
 * 
 */
void FsmSw_SphincsSha2_128fSimple_PrfAddr(uint8 *out, const sphincs_sha2_128f_ctx *ctx, const uint32 addr[8])
{
  sha256ctx sha2_state                                              = {{0}};
  uint8 buf[SPX_SHA256_ADDR_BYTES + FSMSW_SPHINCSSHA2_128FSIMPLE_N] = {0};
  uint8 outbuf[FSMSW_SPHINCS_SHA256_OUTPUT_BYTES]                   = {0};

  /* Retrieve precomputed state containing pub_seed */
  FsmSw_Sha256_IncCtxClone(&sha2_state, &ctx->state_seeded);

  /* Remainder: ADDR^c ‖ SK.seed */
  FsmSw_CommonLib_MemCpy(buf, addr, SPX_SHA256_ADDR_BYTES);
  FsmSw_CommonLib_MemCpy(&buf[SPX_SHA256_ADDR_BYTES], ctx->sk_seed, FSMSW_SPHINCSSHA2_128FSIMPLE_N);

  FsmSw_Sha256_IncFinalize(outbuf, &sha2_state, buf, SPX_SHA256_ADDR_BYTES + FSMSW_SPHINCSSHA2_128FSIMPLE_N);

  FsmSw_CommonLib_MemCpy(out, outbuf, FSMSW_SPHINCSSHA2_128FSIMPLE_N);
} // end: FsmSw_SphincsSha2_128fSimple_PrfAddr

/*====================================================================================================================*/
/**
 * \brief Computes the message-dependent randomness R, using a secret seed as a key for HMAC, and an optional
 *        randomization value prefixed to the message. This requires m to have at least SPX_SHAX_BLOCK_BYTES +
 *        FSMSW_SPHINCSSHA2_128FSIMPLE_N space available in front of the pointer, i.e. before the message to use
 *        for the prefix.
 *        This is necessary to prevent having to move the message around (and allocate memory for it).
 *
 * \param[out] uint8                         *R : t.b.d.
 * \param[in]  const uint8              *sk_prf : t.b.d.
 * \param[in]  const uint8             *optrand : t.b.d.
 * \param[in]  const uint8                   *m : t.b.d.
 * \param[in]  uint32                      mlen : t.b.d.
 * \param[in]  const sphincs_sha2_128f_ctx *ctx : t.b.d.
 *
 */
void FsmSw_SphincsSha2_128fSimple_GenMessageRandom(uint8 *R, const uint8 *sk_prf, const uint8 *optrand, const uint8 *m,
                                                   uint32 mlen, const sphincs_sha2_128f_ctx *ctx)
{
  (void)ctx;

  uint8 buf[SPX_SHAX_BLOCK_BYTES + SPX_SHAX_OUTPUT_BYTES] = {0};
  shaXstate state                                         = {{0}};
  uint8 i                                                 = 0;

  /* m_temp and mlen_temp are used to avoid modifying the input. */
  const uint8 *m_temp = m;
  uint32 mlen_temp    = mlen;

  /* This implements HMAC-SHA */
  for (i = 0; i < FSMSW_SPHINCSSHA2_128FSIMPLE_N; i++)
  {
    buf[i] = 0x36u ^ sk_prf[i];
  }
  FsmSw_CommonLib_MemSet(&buf[FSMSW_SPHINCSSHA2_128FSIMPLE_N], 0x36,
                         SPX_SHAX_BLOCK_BYTES - FSMSW_SPHINCSSHA2_128FSIMPLE_N);

  shaX_inc_init(&state);
  shaX_inc_blocks(&state, buf, 1);

  FsmSw_CommonLib_MemCpy(buf, optrand, FSMSW_SPHINCSSHA2_128FSIMPLE_N);

  /* If optrand + message cannot fill up an entire block */
  if ((FSMSW_SPHINCSSHA2_128FSIMPLE_N + mlen_temp) < SPX_SHAX_BLOCK_BYTES)
  {
    FsmSw_CommonLib_MemCpy(&buf[FSMSW_SPHINCSSHA2_128FSIMPLE_N], m_temp, mlen_temp);
    shaX_inc_finalize(&buf[SPX_SHAX_BLOCK_BYTES], &state, buf, mlen_temp + FSMSW_SPHINCSSHA2_128FSIMPLE_N);
  }
  /* Otherwise first fill a block, so that finalize only uses the message */
  else
  {
    FsmSw_CommonLib_MemCpy(&buf[FSMSW_SPHINCSSHA2_128FSIMPLE_N], m_temp,
                           SPX_SHAX_BLOCK_BYTES - FSMSW_SPHINCSSHA2_128FSIMPLE_N);
    shaX_inc_blocks(&state, buf, 1);

    m_temp = &m_temp[SPX_SHAX_BLOCK_BYTES - FSMSW_SPHINCSSHA2_128FSIMPLE_N];
    mlen_temp -= SPX_SHAX_BLOCK_BYTES - FSMSW_SPHINCSSHA2_128FSIMPLE_N;
    shaX_inc_finalize(&buf[SPX_SHAX_BLOCK_BYTES], &state, m_temp, mlen_temp);
  }

  for (i = 0; i < FSMSW_SPHINCSSHA2_128FSIMPLE_N; i++)
  {
    buf[i] = 0x5cu ^ sk_prf[i];
  }
  FsmSw_CommonLib_MemSet(&buf[FSMSW_SPHINCSSHA2_128FSIMPLE_N], 0x5c,
                         SPX_SHAX_BLOCK_BYTES - FSMSW_SPHINCSSHA2_128FSIMPLE_N);

  shaX(buf, buf, SPX_SHAX_BLOCK_BYTES + SPX_SHAX_OUTPUT_BYTES);
  FsmSw_CommonLib_MemCpy(R, buf, FSMSW_SPHINCSSHA2_128FSIMPLE_N);
} // end: FsmSw_SphincsSha2_128fSimple_GenMessageRandom

/*====================================================================================================================*/
/**
 * \brief Computes the message hash using R, the public key, and the message. Outputs the message digest and the
 *        index of the leaf. The index is split in the tree index and the leaf index, for convenient copying to
 *        an address.
 *
 * \param[out] uint8                    *digest : t.b.d.
 * \param[out] uint64                     *tree : t.b.d.
 * \param[out] uint32                 *leaf_idx : t.b.d.
 * \param[in]  const uint8                   *R : t.b.d.
 * \param[in]  const uint8                  *pk : t.b.d.
 * \param[in]  const uint8                   *m : t.b.d.
 * \param[in]  uint32                      mlen : t.b.d.
 * \param[in]  const sphincs_sha2_128f_ctx *ctx : t.b.d.
 *
 */
void FsmSw_SphincsSha2_128fSimple_HashMessage(uint8 *digest, uint64 *tree, uint32 *leaf_idx, const uint8 *R,
                                              const uint8 *pk, const uint8 *m, uint32 mlen,
                                              const sphincs_sha2_128f_ctx *ctx)
{
  (void)ctx;
  uint8 seed[(2u * FSMSW_SPHINCSSHA2_128FSIMPLE_N) + SPX_SHAX_OUTPUT_BYTES] = {0};
  /* polyspace +2 MISRA2012:2.2 [Justified:]"Calculation is important if defines should change
  and therefore not dead code" */
  uint8 inbuf[SPX_INBLOCKS * SPX_SHAX_BLOCK_BYTES] = {0};
  uint8 buf[SPX_DGST_BYTES]                        = {0};
  uint8 *bufp                                      = buf;
  shaXstate state                                  = {{0}};

  shaX_inc_init(&state);

  /* m_temp and mlen_temp are used to avoid modifying the input. */
  const uint8 *m_temp = m;
  uint32 mlen_temp    = mlen;

  /* seed: SHA-X(R ‖ PK.seed ‖ PK.root ‖ M) */
  FsmSw_CommonLib_MemCpy(inbuf, R, FSMSW_SPHINCSSHA2_128FSIMPLE_N);
  FsmSw_CommonLib_MemCpy(&inbuf[FSMSW_SPHINCSSHA2_128FSIMPLE_N], pk, FSMSW_SPHINCSSHA2_128FSIMPLE_PK_BYTES);

  /* If R + pk + message cannot fill up an entire block */
  /* polyspace +3 MISRA2012:2.2 [Justified:]"Calculation is important if defines should change
  and therefore not dead code" */
  if ((FSMSW_SPHINCSSHA2_128FSIMPLE_N + FSMSW_SPHINCSSHA2_128FSIMPLE_PK_BYTES + mlen_temp) <
      (SPX_INBLOCKS * SPX_SHAX_BLOCK_BYTES))
  {
    FsmSw_CommonLib_MemCpy(&inbuf[FSMSW_SPHINCSSHA2_128FSIMPLE_N + FSMSW_SPHINCSSHA2_128FSIMPLE_PK_BYTES], m_temp,
                           mlen_temp);
    shaX_inc_finalize(&seed[2u * FSMSW_SPHINCSSHA2_128FSIMPLE_N], &state, inbuf,
                      FSMSW_SPHINCSSHA2_128FSIMPLE_N + FSMSW_SPHINCSSHA2_128FSIMPLE_PK_BYTES + mlen_temp);
  }
  /* Otherwise first fill a block, so that finalize only uses the message */
  else
  {
    /* polyspace +3 MISRA2012:2.2 [Justified:]"Calculation is important if defines should change
    and therefore not dead code" */
    FsmSw_CommonLib_MemCpy(&inbuf[FSMSW_SPHINCSSHA2_128FSIMPLE_N + FSMSW_SPHINCSSHA2_128FSIMPLE_PK_BYTES], m_temp,
                           (SPX_INBLOCKS * SPX_SHAX_BLOCK_BYTES) - FSMSW_SPHINCSSHA2_128FSIMPLE_N -
                               FSMSW_SPHINCSSHA2_128FSIMPLE_PK_BYTES);
    shaX_inc_blocks(&state, inbuf, SPX_INBLOCKS);

    /* polyspace +2 MISRA2012:2.2 [Justified:]"Calculation is important if defines should change
    and therefore not dead code" */
    m_temp = &m_temp[(SPX_INBLOCKS * SPX_SHAX_BLOCK_BYTES) - FSMSW_SPHINCSSHA2_128FSIMPLE_N -
                     FSMSW_SPHINCSSHA2_128FSIMPLE_PK_BYTES];
    /* polyspace +3 MISRA2012:2.2 [Justified:]"Calculation is important if defines should change
    and therefore not dead code" */
    mlen_temp = mlen_temp - ((SPX_INBLOCKS * SPX_SHAX_BLOCK_BYTES) - FSMSW_SPHINCSSHA2_128FSIMPLE_N -
                             FSMSW_SPHINCSSHA2_128FSIMPLE_PK_BYTES);
    shaX_inc_finalize(&seed[2u * FSMSW_SPHINCSSHA2_128FSIMPLE_N], &state, m_temp, (uint32)mlen_temp);
  }

  /* H_msg: MGF1-SHA-X(R ‖ PK.seed ‖ seed) */
  FsmSw_CommonLib_MemCpy(seed, R, FSMSW_SPHINCSSHA2_128FSIMPLE_N);
  FsmSw_CommonLib_MemCpy(&seed[FSMSW_SPHINCSSHA2_128FSIMPLE_N], pk, FSMSW_SPHINCSSHA2_128FSIMPLE_N);

  /* By doing this in two steps, we prevent hashing the message twice;
     otherwise each iteration in MGF1 would hash the message again. */
  mgf1_X(bufp, SPX_DGST_BYTES, seed, (2u * FSMSW_SPHINCSSHA2_128FSIMPLE_N) + SPX_SHAX_OUTPUT_BYTES);

  FsmSw_CommonLib_MemCpy(digest, bufp, FSMSW_SPHINCSSHA2_128FSIMPLE_FORS_MSG_BYTES);
  bufp = &bufp[FSMSW_SPHINCSSHA2_128FSIMPLE_FORS_MSG_BYTES];

  *tree = FsmSw_Sphincs_BytesToUll(bufp, SPX_TREE_BYTES);
  *tree &= (~(uint64)0) >> (64u - SPX_TREE_BITS);
  bufp = &bufp[SPX_TREE_BYTES];

  *leaf_idx = (uint32)FsmSw_Sphincs_BytesToUll(bufp, SPX_LEAF_BYTES);
  *leaf_idx &= (~(uint32)0) >> (32u - SPX_LEAF_BITS);
} // end: FsmSw_SphincsSha2_128fSimple_HashMessage

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */