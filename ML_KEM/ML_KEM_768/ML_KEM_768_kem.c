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
/** \addtogroup ML_KEM_768
*    includes the modules for ML_KEM_768
 ** @{ */
/** \addtogroup ML_KEM_768_kem
 ** @{ */

/*====================================================================================================================*/
/** \file ML_KEM_768_kem.c
* \brief  description of ML_KEM_768_kem.c
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
#include "ML_KEM_768_indcpa.h"
#include "ML_KEM_768_params.h"
#include "ML_KEM_symmetric.h"
#include "ML_KEM_verify.h"
#include "Std_Types.h"

#include "ML_KEM_768_kem.h"
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
* \brief Generates public and private key
*        for CCA-secure ML_KEM key encapsulation mechanism
*
* \param[out] uint8 *pk : pointer to output public key (of length ML_KEM_768_PUBLICKEYBYTES bytes)
* \param[out] uint8 *sk : pointer to output private key (of length ML_KEM_768_SECRETKEYBYTES bytes)
*/
void ML_KEM_768_Crypto_Kem_KeyPair(uint8 *const pk, uint8 *const sk)
{
  uint32 i = 0;

  ML_KEM_768_Indcpa_KeyPair(pk, sk);

  for (i = 0; i < ML_KEM_768_INDCPA_PUBLICKEYBYTES; i++)
  {
    sk[i + ML_KEM_768_INDCPA_SECRETKEYBYTES] = pk[i];
  }

  FsmSw_Fips202_Sha3_256(&sk[ML_KEM_768_SECRETKEYBYTES - (2u * ML_KEM_SYMBYTES)], pk, ML_KEM_768_PUBLICKEYBYTES);
  /* Value z for pseudo-random output on reject */
  (void)FsmSw_CommonLib_RandomBytes(&sk[ML_KEM_768_SECRETKEYBYTES - ML_KEM_SYMBYTES], ML_KEM_SYMBYTES);

  return;
} // end: ML_KEM_768_Crypto_Kem_KeyPair

/*====================================================================================================================*/
/**
* \brief Generates cipher text and shared
*        secret for given public key
*
* \param[out] uint8       *ct : pointer to output cipher text (of length ML_KEM_768_CIPHERTEXTBYTES bytes)
* \param[out] uint8       *ss : pointer to output shared secret (of length ML_KEM_SSBYTES bytes)
* \param[in]  const uint8 *pk : pointer to input public key (of length ML_KEM_768_PUBLICKEYBYTES bytes)
*/
void ML_KEM_768_Crypto_Kem_Enc(uint8 *const ct, uint8 *const ss, const uint8 *const pk)
{
  uint8 buf[2u * ML_KEM_SYMBYTES] = {0};
  /* Will contain key, coins */
  uint8 kr[2u * ML_KEM_SYMBYTES] = {0};

  (void)FsmSw_CommonLib_RandomBytes(buf, ML_KEM_SYMBYTES);
  /* Don't release system RNG output */

  /* Multitarget countermeasure for coins + contributory KEM */
  FsmSw_Fips202_Sha3_256(&buf[ML_KEM_SYMBYTES], pk, ML_KEM_768_PUBLICKEYBYTES);
  FsmSw_Fips202_Sha3_512(kr, buf, 2u * ML_KEM_SYMBYTES);

  /* coins are in kr+ML_KEM_SYMBYTES */
  ML_KEM_768_Indcpa_Enc(ct, buf, pk, &kr[ML_KEM_SYMBYTES]);

  /* overwrite coins in kr with H(c) */
  FsmSw_Fips202_Sha3_256(&kr[ML_KEM_SYMBYTES], ct, ML_KEM_768_CIPHERTEXTBYTES);
  /* hash concatenation of pre-k and H(c) to k */
  FsmSw_Fips202_Shake256(ss, ML_KEM_SSBYTES, kr, 2u * ML_KEM_SYMBYTES);

  return;
} // end: ML_KEM_768_Crypto_Kem_Enc

/*====================================================================================================================*/
/**
* \brief Generates shared secret for given
*        cipher text and private key
*
* \param[out] uint8       *ss : pointer to output shared secret (of length ML_KEM_SSBYTES bytes)
* \param[in]  const uint8 *ct : pointer to input cipher text (of length ML_KEM_768_CIPHERTEXTBYTES bytes)
* \param[in]  const uint8 *sk : pointer to input private key (of length ML_KEM_768_SECRETKEYBYTES bytes)

* On failure, ss will contain a pseudo-random value.
*/
void ML_KEM_768_Crypto_Kem_Dec(uint8 *const ss, const uint8 *const ct, const uint8 *const sk)
{
  uint32 i                       = 0;
  uint8 fail                     = 0;
  uint8 buf[2u * ML_KEM_SYMBYTES] = {0};
  /* Will contain key, coins */
  uint8 kr[2u * ML_KEM_SYMBYTES]       = {0};
  uint8 cmp[ML_KEM_768_CIPHERTEXTBYTES] = {0};
  const uint8 *const pk               = &sk[ML_KEM_768_INDCPA_SECRETKEYBYTES];

  ML_KEM_768_Indcpa_Dec(buf, ct, sk);

  /* Multitarget countermeasure for coins + contributory KEM */
  for (i = 0; i < ML_KEM_SYMBYTES; i++)
  {
    buf[ML_KEM_SYMBYTES + i] = sk[ML_KEM_768_SECRETKEYBYTES - (2u * ML_KEM_SYMBYTES) + i];
  }

  FsmSw_Fips202_Sha3_512(kr, buf, 2u * ML_KEM_SYMBYTES);

  /* coins are in kr+ML_KEM_SYMBYTES */
  ML_KEM_768_Indcpa_Enc(cmp, buf, pk, &kr[ML_KEM_SYMBYTES]);

  fail = ML_KEM_Verify(ct, cmp, ML_KEM_768_CIPHERTEXTBYTES);

  /* overwrite coins in kr with H(c) */
  FsmSw_Fips202_Sha3_256(&kr[ML_KEM_SYMBYTES], ct, ML_KEM_768_CIPHERTEXTBYTES);

  /* Overwrite pre-k with z on re-encryption failure */
  ML_KEM_Cmov(kr, &sk[ML_KEM_768_SECRETKEYBYTES - ML_KEM_SYMBYTES], ML_KEM_SYMBYTES, fail);

  /* hash concatenation of pre-k and H(c) to k */
  FsmSw_Fips202_Shake256(ss, ML_KEM_SSBYTES, kr, 2u * ML_KEM_SYMBYTES);

  return;
} // end: ML_KEM_768_Crypto_Kem_Dec

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */