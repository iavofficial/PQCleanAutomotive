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
/** \addtogroup SLH_DSA_SHA2_192sSimple_context
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHA2_192sSimple_context_sha2.c
* \brief  description of SLH_DSA_SHA2_192sSimple_context_sha2.c
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
#include "SLH_DSA_SHA2_192sSimple_context.h"
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
static void slh_dsa_sha2_192ssimple_SeedState(slh_dsa_sha2_192s_ctx *ctx);
/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/** 
 * \brief Absorb the constant pub_seed using one round of the compression function. This initializes state_seeded
 *        and state_seeded_512, which can then be reused in SLH_DSA_SHA2_192sSimple_Thash.
 *
 * \param[in] slh_dsa_sha2_192s_ctx *ctx : t.b.d.
 *
 */
static void slh_dsa_sha2_192ssimple_SeedState(slh_dsa_sha2_192s_ctx *ctx)
{
  uint8 block[SLH_DSA_SHA512_BLOCK_BYTES] = {0};
  uint32 i                                      = 0;

  for (i = 0; i < SLH_DSA_SHA2_192SSIMPLE_N; ++i)
  {
    block[i] = ctx->pub_seed[i];
  }

  for (i = SLH_DSA_SHA2_192SSIMPLE_N; i < SLH_DSA_SHA512_BLOCK_BYTES; ++i)
  {
    block[i] = 0;
  }
  /* block has been properly initialized for both SHA-256 and SHA-512 */

  FsmSw_Sha256_IncInit(&ctx->state_seeded);
  FsmSw_Sha256_IncBlocks(&ctx->state_seeded, block, 1);

  FsmSw_Sha512_IncInit(&ctx->state_seeded_512);
  FsmSw_Sha512_IncBlocks(&ctx->state_seeded_512, block, 1);
} // end: slh_dsa_sha2_192ssimple_SeedState
/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/** 
 * \brief We initialize the state for the hash functions
 *
 * \param[in] slh_dsa_sha2_192s_ctx *ctx : t.b.d.
 *
 */
void SLH_DSA_SHA2_192sSimple_InitializeHashFunction(slh_dsa_sha2_192s_ctx *const ctx)
{
  slh_dsa_sha2_192ssimple_SeedState(ctx);
} // end: SLH_DSA_SHA2_192sSimple_InitializeHashFunction

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */