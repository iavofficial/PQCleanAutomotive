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
/** \addtogroup SLH_DSA_SHA2_192fSimple
*    includes the modules for SLH_DSA_SHA2_192fSimple
 ** @{ */
/** \addtogroup SLH_DSA_SHA2_192fSimple_hash 
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHA2_192fSimple_hash.h
* \brief  Description of SLH_DSA_SHA2_192fSimple_hash.h
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
#ifndef SLH_DSA_SHA2_192FSIMPLE_HASH_H
#define SLH_DSA_SHA2_192FSIMPLE_HASH_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "SLH_DSA_SHA2_192fSimple_context.h"
#include "SLH_DSA_SHA2_192fSimple_params.h"
#include "Std_Types.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define SPX_SHA256_ADDR_BYTES 22u
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
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void SLH_DSA_SHA2_192fSimple_PrfAddr(uint8 *const out, const slh_dsa_sha2_192f_ctx *const ctx,
                                          const uint32 addr[8]);

void SLH_DSA_SHA2_192fSimple_GenMessageRandom(uint8 *const R, const uint8 *const sk_prf,
                                                   const uint8 *const optrand, const uint8 *const m, uint32 mlen,
                                                   const slh_dsa_sha2_192f_ctx *const ctx);

void SLH_DSA_SHA2_192fSimple_HashMessage(uint8 *const digest, uint64 *const tree, uint32 *const leaf_idx,
                                              const uint8 *const R, const uint8 *const pk, const uint8 *const m,
                                              uint32 mlen, const slh_dsa_sha2_192f_ctx *const ctx);

void SLH_DSA_SHA2_192fSimple_MgF1_512(uint8 *const out, uint32 outlen, const uint8 *const in, uint32 inlen);

#endif /* SLH_DSA_SHA2_192FSIMPLE_HASH_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */