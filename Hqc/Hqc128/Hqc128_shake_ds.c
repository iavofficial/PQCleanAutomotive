/***********************************************************************************************************************
 *
 * Original implementation: PQClean, HQC
 *
 * Copyright 2026 IAV GmbH
 *
 * The upstream PQClean repository identifies the original HQC
 * implementation as "Public Domain". No complete upstream license text
 * or explicit CC0 reference is provided.
 * IAV modifications are licensed under the Apache License, Version 2.0.
 *
 * SPDX-License-Identifier: LicenseRef-PQClean-HQC-Public-Domain AND Apache-2.0
 *
 **********************************************************************************************************************/

/** \addtogroup SwC Hqc
*    includes the modules for SwC Hqc
 ** @{ */
/** \addtogroup Hqc128
*    includes the modules for Hqc128
 ** @{ */
/** \addtogroup Hqc128_shake_ds
 ** @{ */

/*====================================================================================================================*/
/** \file Hqc128_shake_ds.c
* \brief Implementation SHAKE-256 with incremental API and domain separation
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
#include "Hqc128_shake_ds.h"
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
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
 * \brief SHAKE-256 with incremental API and domain separation
 *
 * Derived from function SHAKE_256 in fips202.c
 *
 * \param[out] state Internal state of SHAKE
 * \param[in] output Pointer to output
 * \param[in] input Pointer to input
 * \param[in] inlen length of input in bytes
 * \param[in] domain byte for domain separation
 */
void Hqc128_Shake256_512_Ds(shake256incctx *const state, uint8 *const output, const uint8 *const input,
                                  uint16 inlen, uint8 domain)
{
  const uint8 domain_tmp = domain;
  /* Init state */
  FsmSw_Fips202_Shake256_IncInit(state);

  /* Absorb input */
  FsmSw_Fips202_Shake256_IncAbsorb(state, input, inlen);

  /* Absorb domain separation byte */
  FsmSw_Fips202_Shake256_IncAbsorb(state, &domain_tmp, 1);

  /* Finalize */
  FsmSw_Fips202_Shake256_IncFinalize(state);

  /* Squeeze output */
  FsmSw_Fips202_Shake256_IncSqueeze(output, 512 / 8, state);

  /* Release ctx */
  //shake256_inc_ctx_release(state);
} // end: Hqc128_Shake256_512_Ds

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */