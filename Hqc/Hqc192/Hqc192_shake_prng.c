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
/** \addtogroup Hqc192
*    includes the modules for Hqc192
 ** @{ */
/** \addtogroup Hqc192_shake_prng
 ** @{ */

/*====================================================================================================================*/
/** \file Hqc192_shake_prng.c
* \brief  Implementation of SHAKE-256 based seed expander
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
#include "Hqc_CommonLib.h"
#include "Hqc_Fips202.h"
#include "Hqc192_domains.h"

#include "Hqc192_shake_prng.h"

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
* \brief Initialise a SHAKE-256 based seed expander
*
* Derived from function SHAKE_256 in fips202.c
*
* \param[out]   state Keccak internal state and a counter
* \param[in]    seed A seed
* \param[in]    seedlen The seed bytes length
*
*/
void Hqc192_SeedExpander_Init(hqc192_seedexpander_state *const state, const uint8 *const seed, uint32 seedlen)
{
  const uint8 domain = HQC192_SEEDEXPANDER_DOMAIN;
  Hqc_Fips202_Shake256_IncInit(state);
  Hqc_Fips202_Shake256_IncAbsorb(state, seed, seedlen);
  Hqc_Fips202_Shake256_IncAbsorb(state, &domain, 1);
  Hqc_Fips202_Shake256_IncFinalize(state);
} // end: Hqc192_SeedExpander_Init

/*====================================================================================================================*/
/**
* \brief A SHAKE-256 based seed expander
*
* Derived from function SHAKE_256 in fips202.c
* Squeezes Keccak state by 64-bit blocks (hardware version compatibility)
*
* \param[out]   state Internal state of SHAKE
* \param[out]   output The XOF data
* \param[in]    outlen Number of bytes to return
*
*/
void Hqc192_SeedExpander(hqc192_seedexpander_state *const state, uint8 *output, uint32 outlen)
{
  const uint8 bsize     = sizeof(uint64);
  const uint8 remainder = (uint8)((outlen % Hqc_Convert_u8_to_u32(bsize)) & 0xFFU);
  uint8 tmp[sizeof(uint64)];
  Hqc_Fips202_Shake256_IncSqueeze(output, outlen - Hqc_Convert_u8_to_u32(remainder), state);
  if (remainder != 0)
  {
    Hqc_Fips202_Shake256_IncSqueeze(tmp, Hqc_Convert_u8_to_u32(bsize), state);
    uint8 *const output_tmp = &output[outlen - Hqc_Convert_u8_to_u32(remainder)];
    for (uint8 i = 0; i < remainder; ++i)
    {
      output_tmp[i] = tmp[i];
    }
  }
} // end: Hqc192_SeedExpander

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */