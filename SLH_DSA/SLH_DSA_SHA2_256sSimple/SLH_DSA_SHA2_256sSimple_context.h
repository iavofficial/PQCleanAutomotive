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
/** \addtogroup SLH_DSA_SHA2_256sSimple_context
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHA2_256sSimple_context.h
* \brief  Description of SLH_DSA_SHA2_256sSimple_context.h
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
#ifndef SLH_DSA_SHA2_256SSIMPLE_CONTEXT_H
#define SLH_DSA_SHA2_256SSIMPLE_CONTEXT_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "SLH_DSA_SHA2_256sSimple_params.h"
#include "FsmSw_sha2.h"
#include "Std_Types.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
/* polyspace +5 CERT-C:DCL12-C [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* polyspace +3 MISRA2012:D4.8 [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
typedef struct
{
  uint8 pub_seed[SLH_DSA_SHA2_256SSIMPLE_N];
  uint8 sk_seed[SLH_DSA_SHA2_256SSIMPLE_N];
  /* sha256 state that absorbed pub_seed */
  sha256ctx state_seeded;
  /* sha512 state that absorbed pub_seed */
  sha512ctx state_seeded_512;
} slh_dsa_sha2_256s_ctx;

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
void SLH_DSA_SHA2_256sSimple_InitializeHashFunction(slh_dsa_sha2_256s_ctx *const ctx);

#endif /* SLH_DSA_SHA2_256SSIMPLE_CONTEXT_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */