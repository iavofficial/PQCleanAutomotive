/***********************************************************************************************************************
 *
 * Original implementation: PQClean, ML-KEM (formerly CRYSTALS-Kyber)
 *
 * Copyright 2026 IAV GmbH
 *
 * Original portions are marked as Public Domain by PQClean.
 * See the NOTICE file in the repository root for the upstream
 * license reference and attribution information.
 * IAV modifications are licensed under the Apache License, Version 2.0.
 *
 * SPDX-License-Identifier: LicenseRef-PQClean-Public-Domain AND Apache-2.0
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup common
*    includes the modules for common
 ** @{ */
/** \addtogroup ML_KEM_verify
 ** @{ */

/*====================================================================================================================*/
/** \file ML_KEM_verify.h
* \brief  Declarations for the modul ML_KEM_symmetric_verify.c
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
#ifndef ML_KEM_VERIFY_H
#define ML_KEM_VERIFY_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
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

/**
* \def PQCLEAN_PREVENT_BRANCH_HACK(b)
* \brief Prevents inferring that b is 0/1-valued, and handling the two cases with a branch.
*/

/* polyspace +6 CERT-C:PRE00-C [Justified:] "This macro affects compiler optimization and its inlining must be ensured" */
/* polyspace +5 MISRA-C3:D4.9 [Justified:] "This macro affects compiler optimization and its inlining must be ensured" */
#if defined(__GNUC__) || defined(__clang__)
#define PQCLEAN_PREVENT_BRANCH_HACK(b) __asm__("" : "+r"(b) : /* no inputs */);
#else
#define PQCLEAN_PREVENT_BRANCH_HACK(b)
#endif

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
uint8 ML_KEM_Verify(const uint8 *const a, const uint8 *const b, uint32 len);
void ML_KEM_Cmov(uint8 *const r, const uint8 *const x, uint32 len, uint8 b);
void ML_KEM_Cmov_int16(sint16 *r, sint16 v, uint16 b);

#endif /* ML_KEM_VERIFY_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */