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
/** \addtogroup SLH_DSA_SHA2_256fSimple
*    includes the modules for SLH_DSA_SHA2_256fSimple
 ** @{ */
/** \addtogroup SLH_DSA_SHA2_256fSimple_utils
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHA2_256fSimple_utils.h
* \brief  Description of SLH_DSA_SHA2_256fSimple_utils.h
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
#ifndef SLH_DSA_SHA2_256FSIMPLE_UTILS_H
#define SLH_DSA_SHA2_256FSIMPLE_UTILS_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "SLH_DSA_SHA2_256fSimple_context.h"
#include "SLH_DSA_SHA2_256fSimple_params.h"
#include "Std_Types.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
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
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void SLH_DSA_SHA2_256fSimple_ComputeRoot(uint8 *const root, const uint8 *const leaf, uint32 leaf_idx,
                                              uint32 idx_offset, const uint8 *const auth_path, uint32 tree_height,
                                              const slh_dsa_sha2_256f_ctx *const ctx, uint32 addr[8]);
void SLH_DSA_SHA2_256fSimple_TreeHash(uint8 *const root, uint8 *const auth_path,
                                           const slh_dsa_sha2_256f_ctx *const ctx, uint32 leaf_idx, uint32 idx_offset,
                                           uint32 tree_height,
                                           void (*const gen_leaf)(uint8 *leaf, const slh_dsa_sha2_256f_ctx *ctx,
                                                                  uint32 addr_idx, const uint32 tree_addr[8]),
                                           uint32 tree_addr[8]);

#endif /* SLH_DSA_SHA2_256FSIMPLE_UTILS_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */