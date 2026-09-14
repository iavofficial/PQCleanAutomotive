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
/** \addtogroup SLH_DSA_SHAKE_192fSimple
*    includes the modules for SLH_DSA_SHAKE_192fSimple
 ** @{ */
/** \addtogroup SLH_DSA_SHAKE_192fSimple_utilsx1
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHAKE_192fSimple_utilsx1.h
* \brief  Description of SLH_DSA_SHAKE_192fSimple_utilsx1.h
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
#ifndef SLH_DSA_SHAKE_192FSIMPLE_UTILSX4_H
#define SLH_DSA_SHAKE_192FSIMPLE_UTILSX4_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "SLH_DSA_SHAKE_192fSimple_context.h"
#include "SLH_DSA_SHAKE_192fSimple_params.h"
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
/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void SLH_DSA_SHAKE_192fSimple_TreeHashX1(uint8 *const root, uint8 *const auth_path,
                                              const slh_dsa_shake_192f_ctx *const ctx, uint32 leaf_idx,
                                              uint32 idx_offset, uint32 tree_height,
                                              void (*const gen_leaf)(uint8 *const leaf /* Where to write the leaf */,
                                                                     const slh_dsa_shake_192f_ctx *const ctx,
                                                                     uint32 addr_idx, void *const info),
                                              uint32 tree_addr[8], void *const info);

#endif /* SLH_DSA_SHAKE_192FSIMPLE_UTILSX4_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */