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
/** \addtogroup SLH_DSA_SHAKE_256sSimple
*    includes the modules for SLH_DSA_SHAKE_256sSimple
 ** @{ */
/** \addtogroup SLH_DSA_SHAKE_256sSimple_context
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHAKE_256sSimple_context_shake.c
* \brief  description of SLH_DSA_SHAKE_256sSimple_context_shake.c
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
#include "SLH_DSA_SHAKE_256sSimple_context.h"
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
* \brief For SHAKE256, there is no immediate reason to initialize at the start, so this function is an empty
*              operation.
*
* \param[out] slh_dsa_shake_256s_ctx *ctx : t.b.d.
*
*/
/* polyspace +4 CERT-C:DCL13-C [Justified:]"For SHAKE256, there is no immediate reason to initialize at the start, 
so this function is an empty operation." */
/* polyspace +2 MISRA2012:8.13 [Justified:]"For SHAKE256, there is no immediate reason to initialize at the start, 
so this function is an empty operation." */
void SLH_DSA_SHAKE_256sSimple_InitializeHashFunction(slh_dsa_shake_256s_ctx *const ctx)
{
  (void)ctx; /* Suppress an 'unused parameter' warning. */
} // end: SLH_DSA_SHAKE_256sSimple_InitializeHashFunction

/*====================================================================================================================*/
/**
* \brief in case the hash function api is heap-based.
*
* \param[out] slh_dsa_shake_256s_ctx *ctx : t.b.d.
*
*/
/* polyspace +2 CERT-C:DCL13-C [Justified:]"in case the hash function api is heap-based." */
/* polyspace +1 MISRA2012:8.13 [Justified:]"in case the hash function api is heap-based." */
void SLH_DSA_SHAKE_256sSimple_1FreeHashFunction(slh_dsa_shake_256s_ctx *const ctx)
{
  (void)ctx;
} // end: SLH_DSA_SHAKE_256sSimple_1FreeHashFunction

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */