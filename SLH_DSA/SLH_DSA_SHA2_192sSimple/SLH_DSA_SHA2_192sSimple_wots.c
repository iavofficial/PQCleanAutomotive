/***********************************************************************************************************************
 *
 * Original implementation: PQClean, SLH-DSA (standardized as SLH-DSA)
 *
 * Copyright 2026 IAV GmbH
 *
 * Original portions are dedicated to the public domain under CC0 1.0 Universal.
 * See the NOTICE file in the repository root for attribution information.
 * IAV modifications are licensed under the Apache License, Version 2.0.
 *
 * SPDX-License-Identifier: CC0-1.0 AND Apache-2.0
 *
 **********************************************************************************************************************/

/** \addtogroup SwC SLH-DSA
*    includes the modules for SwC SLH-DSA
 ** @{ */
/** \addtogroup SLH_DSA_SHA2_192sSimple
*    includes the modules for SLH_DSA_SHA2_192sSimple
 ** @{ */
/** \addtogroup SLH_DSA_SHA2_192sSimple_wots
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHA2_192sSimple_wots.c
* \brief  description of SLH_DSA_SHA2_192sSimple_wots.c
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
// TODO clarify address expectations, and make them more uniform.
// TODO i.e. do we expect types to be set already?
// TODO and do we expect modifications or copies?
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "SLH_DSA_CommonLib.h"
#include "SLH_DSA_SHA2_192sSimple_hash.h"
#include "SLH_DSA_SHA2_192sSimple_params.h"
#include "SLH_DSA_SHA2_192sSimple_thash.h"
#include "SLH_DSA_SHA2_192sSimple_utils.h"
#include "SLH_DSA_SHA2_192sSimple_utilsx1.h"
#include "SLH_DSA_SHA2_192sSimple_wotsx1.h"
#include "SLH_DSA_SHA2_address.h"
#include "SLH_DSA_utils.h"

#include "SLH_DSA_SHA2_192sSimple_wots.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

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
/* GLOBAL CONSTANTS                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/
static void slh_dsa_sha2_192ssimple_wots_GenChain(uint8 *const out, const uint8 *const in, uint32 start,
                                                       uint32 steps, const slh_dsa_sha2_192s_ctx *const ctx,
                                                       uint32 addr[8]);
static void slh_dsa_sha2_192ssimple_wots_BaseW(uint32 *const output, sint32 out_len, const uint8 *const input);
static void slh_dsa_sha2_192ssimple_wots_Checksum(uint32 *const csum_base_w, const uint32 *const msg_base_w);

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/** 
 * \brief Computes the chaining function. out and in have to be n-byte arrays. Interprets in as start-th value of
 *        the chain. addr has to contain the address of the chain.
 *
 * \param[out] uint8                       *out : t.b.d.
 * \param[in]  const uint8                  *in : t.b.d.
 * \param[in]  uint32                     start : t.b.d.
 * \param[in]  uint32                     steps : t.b.d.
 * \param[in]  const slh_dsa_sha2_192s_ctx *ctx : t.b.d.
 * \param[in]  uint32                   addr[8] : t.b.d.
 *
 */
static void slh_dsa_sha2_192ssimple_wots_GenChain(uint8 *const out, const uint8 *const in, uint32 start,
                                                       uint32 steps, const slh_dsa_sha2_192s_ctx *const ctx,
                                                       uint32 addr[8])
{
  uint32 i = 0;

  /* Initialize out with the value at position 'start'. */
  SLH_DSA_CommonLib_MemCpy(out, in, SLH_DSA_SHA2_192SSIMPLE_N);

  /* Iterate 'steps' calls to the hash function. */
  for (i = start; (i < (start + steps)) && (i < SLH_DSA_SHA2_192SSIMPLE_WOTS_W); i++)
  {
    SLH_DSA_SHA2_SetHashAddr(addr, i);
    SLH_DSA_SHA2_192sSimple_Thash(out, out, 1, ctx, addr);
  }
} // end: slh_dsa_sha2_192ssimple_wots_GenChain

/*====================================================================================================================*/
/** 
 * \brief base_w algorithm as described in draft. Interprets an array of bytes as integers in base w. This only
 *        works when log_w is a divisor of 8.
 *
 * \param[out] uint32     *output : t.b.d.
 * \param[out] sint32    *out_len : t.b.d.
 * \param[in]  const uint8 *input : t.b.d.
 *
 */
static void slh_dsa_sha2_192ssimple_wots_BaseW(uint32 *const output, sint32 out_len, const uint8 *const input)
{
  sint32 in       = 0;
  sint32 out      = 0;
  uint8 total     = 0;
  sint32 bits     = 0;
  sint32 consumed = 0;

  for (consumed = 0; consumed < out_len; consumed++)
  {
    if (bits == 0)
    {
      total = input[in];
      in++;
      bits += 8;
    }
    bits        = bits - (sint32)SLH_DSA_SHA2_192SSIMPLE_WOTS_LOGW;
    output[out] = ((uint32)total >> (uint32)bits) & (SLH_DSA_SHA2_192SSIMPLE_WOTS_W - 1u);
    out++;
  }
} // end: slh_dsa_sha2_192ssimple_wots_BaseW

/*====================================================================================================================*/
/** 
 * \brief Computes the WOTS+ checksum over a message (in base_w).
 *
 * \param[out] uint32      *csum_base_w : t.b.d.
 * \param[in]  const uint32 *msg_base_w : t.b.d.
 *
 */
static void slh_dsa_sha2_192ssimple_wots_Checksum(uint32 *const csum_base_w, const uint32 *const msg_base_w)
{
  uint32 csum                                                                                                     = 0;
  uint8 csum_bytes[((SLH_DSA_SHA2_192SSIMPLE_WOTS_LEN2 * SLH_DSA_SHA2_192SSIMPLE_WOTS_LOGW) + 7u) / 8u] = {0};
  uint32 i                                                                                                        = 0;

  /* Compute checksum. */
  for (i = 0; i < SLH_DSA_SHA2_192SSIMPLE_WOTS_LEN1; i++)
  {
    csum += SLH_DSA_SHA2_192SSIMPLE_WOTS_W - 1u - msg_base_w[i];
  }

  /* Convert checksum to base_w. Make sure expected empty zero bits are the least significant bits. */
  csum = csum << ((8u - ((SLH_DSA_SHA2_192SSIMPLE_WOTS_LEN2 * SLH_DSA_SHA2_192SSIMPLE_WOTS_LOGW) % 8u)) % 8u);
  SLH_DSA_UllToBytes(csum_bytes, sizeof(csum_bytes), csum);
  slh_dsa_sha2_192ssimple_wots_BaseW(csum_base_w, (sint32)SLH_DSA_SHA2_192SSIMPLE_WOTS_LEN2, csum_bytes);
} /// end: slh_dsa_sha2_192ssimple_wots_Checksum
/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/** 
 * \brief Takes a message and derives the matching chain lengths.
 *
 * \param[out] uint8   *lengths : t.b.d.
 * \param[in]  const uint8 *msg : t.b.d.
 *
 */
void SLH_DSA_SHA2_192sSimple_Wots_ChainLengths(uint32 *const lengths, const uint8 *const msg)
{
  slh_dsa_sha2_192ssimple_wots_BaseW(lengths, (sint32)SLH_DSA_SHA2_192SSIMPLE_WOTS_LEN1, msg);
  slh_dsa_sha2_192ssimple_wots_Checksum(&lengths[SLH_DSA_SHA2_192SSIMPLE_WOTS_LEN1], lengths);
} // end: SLH_DSA_SHA2_192sSimple_Wots_ChainLengths

/*====================================================================================================================*/
/** 
 * \brief Takes a WOTS signature and an n-byte message, computes a WOTS public key.
 *              Writes the computed public key to 'pk'.
 *
 * \param[out] uint8                        *pk : t.b.d.
 * \param[in]  const uint8                 *sig : t.b.d.
 * \param[in]  const uint8                 *msg : t.b.d.
 * \param[in]  const slh_dsa_sha2_192s_ctx *ctx : t.b.d.
 * \param[in]  uint32                   addr[8] : t.b.d.
 *
 */
void SLH_DSA_SHA2_192sSimple_Wots_PkFromSig(uint8 *const pk, const uint8 *const sig, const uint8 *const msg,
                                                 const slh_dsa_sha2_192s_ctx *const ctx, uint32 addr[8])
{
  uint32 lengths[SLH_DSA_SHA2_192SSIMPLE_WOTS_LEN] = {0};
  uint32 i                                              = 0;

  SLH_DSA_SHA2_192sSimple_Wots_ChainLengths(lengths, msg);

  for (i = 0; i < SLH_DSA_SHA2_192SSIMPLE_WOTS_LEN; i++)
  {
    SLH_DSA_SHA2_SetChainAddr(addr, i);
    slh_dsa_sha2_192ssimple_wots_GenChain(&pk[i * SLH_DSA_SHA2_192SSIMPLE_N],
                                               &sig[i * SLH_DSA_SHA2_192SSIMPLE_N], lengths[i],
                                               SLH_DSA_SHA2_192SSIMPLE_WOTS_W - 1u - lengths[i], ctx, addr);
  }
} // end: SLH_DSA_SHA2_192sSimple_Wots_PkFromSig

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
