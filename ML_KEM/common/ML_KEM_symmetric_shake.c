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
/** \addtogroup ML_KEM_symmetric_shake
 ** @{ */

/*====================================================================================================================*/
/** \file ML_KEM_symmetric_shake.c
* \brief  description of FsmSw_symmetric_shake.c
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
#include "FsmSw_CommonLib.h"
#include "FsmSw_Fips202.h"
#include "ML_KEM_params.h"
#include "Std_Types.h"

#include "ML_KEM_symmetric.h"
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
* \brief Absorb step of the SHAKE128 specialized for the ML_KEM context.
*
* \param[out] xof_state      *s : pointer to (uninitialized) output Keccak state
* \param[in]  const uint8 *seed : pointer to ML_KEM_SYMBYTES input to be absorbed into state
* \param[in]  uint8           i : additional byte of input
* \param[in]  uint8           j : additional byte of input
*/
void ML_KEM_Shake128_Absorb(xof_state *const s, const uint8 seed[ML_KEM_SYMBYTES], uint8 x, uint8 y)
{
  uint8 extseed[ML_KEM_SYMBYTES + 2u] = {0};

  FsmSw_CommonLib_MemCpy(extseed, seed, ML_KEM_SYMBYTES);
  extseed[ML_KEM_SYMBYTES]      = x;
  extseed[ML_KEM_SYMBYTES + 1u] = y;

  FsmSw_Fips202_Shake128_Absorb(s, extseed, sizeof(extseed));
} // end: ML_KEM_Shake128_Absorb

/*====================================================================================================================*/
/**
* \brief Usage of SHAKE256 as a PRF, concatenates secret and public input
*        and then generates outlen bytes of SHAKE256 output
*
* \param[out] uint8       *out : pointer to output
* \param[out] uint32    outlen : number of requested output bytes
* \param[in]  const uint8 *key : pointer to the key (of length ML_KEM_SYMBYTES)
* \param[in]  uint8      nonce : single-byte nonce (public PRF input)
*/
void ML_KEM_Shake256_Prf(uint8 *const out, uint32 outlen, const uint8 key[ML_KEM_SYMBYTES], uint8 nonce)
{
  uint8 extkey[ML_KEM_SYMBYTES + 1u] = {0};

  FsmSw_CommonLib_MemCpy(extkey, key, ML_KEM_SYMBYTES);
  extkey[ML_KEM_SYMBYTES] = nonce;

  FsmSw_Fips202_Shake256(out, outlen, extkey, sizeof(extkey));
} // end: ML_KEM_Shake256_Prf

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */