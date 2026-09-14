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
/** \addtogroup SLH_DSA_SHA2_256sSimple
*    includes the modules for SLH_DSA_SHA2_256sSimple
 ** @{ */
/** \addtogroup SLH_DSA_SHA2_256sSimple_thash
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHA2_256sSimple_thash_sha2_simple.c
* \brief  description of SLH_DSA_SHA2_256sSimple_thash_sha2_simple.c
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
#include "SLH_DSA_CommonLib.h"
#include "SLH_DSA_SHA2_256sSimple_hash.h"
#include "SLH_DSA_SHA2_256sSimple_params.h"
#include "SLH_DSA_SHA2_256sSimple_utils.h"
#include "SLH_DSA_SHA2_address.h"
#include "SLH_DSA_sha2.h"

#include "SLH_DSA_SHA2_256sSimple_thash.h"
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
 * \brief Takes an array of inblocks concatenated arrays of SLH_DSA_SHA2_256SSIMPLE_N bytes.
 *
 * \param[out] uint8                       *out : t.b.d.
 * \param[in]  const uint8                  *in : t.b.d.
 * \param[in]  uint32                  inblocks : t.b.d.
 * \param[in]  const slh_dsa_sha2_256s_ctx *ctx : t.b.d.
 * \param[in]  const uint32             addr[8] : t.b.d.
 *
 */
void SLH_DSA_SHA2_256sSimple_Thash(uint8 *const out, const uint8 *const in, uint32 inblocks,
                                        const slh_dsa_sha2_256s_ctx *const ctx, const uint32 addr[8])
{

  uint8 outbuf[SLH_DSA_SHA256_OUTPUT_BYTES] = {0};
  sha256ctx sha2_state                            = {{0}};
  uint8 buf[SPX_SHA256_ADDR_BYTES + (SLH_DSA_SHA2_256SSIMPLE_THASH_BUF_LEN * SLH_DSA_SHA2_256SSIMPLE_N)] = {
      0};

  /* Retrieve precomputed state containing pub_seed */
  SLH_DSA_Sha256_IncCtxClone(&sha2_state, &ctx->state_seeded);

  SLH_DSA_CommonLib_MemCpy(buf, addr, SPX_SHA256_ADDR_BYTES);
  SLH_DSA_CommonLib_MemCpy(&buf[SPX_SHA256_ADDR_BYTES], in, inblocks * SLH_DSA_SHA2_256SSIMPLE_N);

  SLH_DSA_Sha256_IncFinalize(outbuf, &sha2_state, buf,
                           SPX_SHA256_ADDR_BYTES + (inblocks * SLH_DSA_SHA2_256SSIMPLE_N));
  SLH_DSA_CommonLib_MemCpy(out, outbuf, SLH_DSA_SHA2_256SSIMPLE_N);
} // end: SLH_DSA_SHA2_256sSimple_Thash

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */