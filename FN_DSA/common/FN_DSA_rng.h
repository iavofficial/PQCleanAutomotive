/***********************************************************************************************************************
 *
 * Original implementation: PQClean, FN_DSA
 *
 * Copyright (c) 2017-2019 FN_DSA Project
 * Copyright 2026 IAV GmbH
 *
 * Original portions are licensed under the MIT License.
 * IAV modifications are licensed under the Apache License, Version 2.0.
 *
 * SPDX-License-Identifier: MIT AND Apache-2.0
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FN_DSA
*    includes the modules for SwC FN_DSA
 ** @{ */
/** \addtogroup common
*    includes the modules for common
 ** @{ */
/** \addtogroup FN_DSA_rng
 ** @{ */

/*====================================================================================================================*/
/** \file FN_DSA_rng.h
* \brief  description of FN_DSA_rng.h
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
#ifndef FN_DSA_RNG_H
#define FN_DSA_RNG_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define FN_DSA_STATE_SIZE 256
#define FN_DSA_BUF_SIZE   512
/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
/* Structure for a PRNG. This includes a large buffer so that values get generated in advance. The 'state' is used to
 * keep the current PRNG algorithm state (contents depend on the selected algorithm).*
 * The unions with 'dummy_u64' are there to ensure proper alignment for 64-bit direct access. */
typedef struct
{
  /* polyspace +2 MISRA2012:19.2 [Justified:]"The buffer and the remainder of the code require 
    the use of the union keyword." */
  union
  {
    uint8 d[FN_DSA_BUF_SIZE]; /* MUST be 512, exactly */
    uint64 dummy_u64;
  } buf;

  uint32 ptr;

  /* polyspace +2 MISRA2012:19.2 [Justified:]"The buffer and the remainder of the code require 
    the use of the union keyword." */
  union
  {
    uint8 d[FN_DSA_STATE_SIZE];
    uint64 dummy_u64;
  } state;

  sint32 type;
} prng;
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
void FN_DSA_Prng_Init(prng *p, inner_shake256_context *const src);
void FN_DSA_Prng_GetBytes(prng *const p, void *const dst, uint32 len);
uint64 FN_DSA_Prng_GetU64(prng *const p);
uint32 FN_DSA_Prng_GetU8(prng *const p);

#endif /* FN_DSA_RNG_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */