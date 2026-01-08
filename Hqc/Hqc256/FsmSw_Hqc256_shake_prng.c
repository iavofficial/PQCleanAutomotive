/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Hqc256
*    includes the modules for Hqc256
 ** @{ */
/** \addtogroup FsmSw_Hqc256_shake_prng
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Hqc256_shake_prng.c
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
#include "FsmSw_CommonLib.h"
#include "FsmSw_Fips202.h"
#include "FsmSw_Hqc256_domains.h"

#include "FsmSw_Hqc256_shake_prng.h"

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
void FsmSw_Hqc256_SeedExpander_Init(hqc256_seedexpander_state *const state, const uint8 *const seed, uint32 seedlen)
{
  const uint8 domain = HQC256_SEEDEXPANDER_DOMAIN;
  FsmSw_Fips202_Shake256_IncInit(state);
  FsmSw_Fips202_Shake256_IncAbsorb(state, seed, seedlen);
  FsmSw_Fips202_Shake256_IncAbsorb(state, &domain, 1);
  FsmSw_Fips202_Shake256_IncFinalize(state);
} // end: FsmSw_Hqc256_SeedExpander_Init

/**
 * @brief A SHAKE-256 based seed expander
 *
 * Derived from function SHAKE_256 in fips202.c
 * Squeezes Keccak state by 64-bit blocks (hardware version compatibility)
 *
 * @param[out] state Internal state of SHAKE
 * @param[out] output The XOF data
 * @param[in] outlen Number of bytes to return
 */

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
void FsmSw_Hqc256_SeedExpander(hqc256_seedexpander_state *const state, uint8 *output, uint32 outlen)
{
  const uint8 bsize     = sizeof(uint64);
  const uint8 remainder = (uint8)((outlen % FsmSw_Convert_u8_to_u32(bsize)) & 0xFFU);
  uint8 tmp[sizeof(uint64)];
  FsmSw_Fips202_Shake256_IncSqueeze(output, outlen - FsmSw_Convert_u8_to_u32(remainder), state);
  if (remainder != 0)
  {
    FsmSw_Fips202_Shake256_IncSqueeze(tmp, FsmSw_Convert_u8_to_u32(bsize), state);
    uint8 *output_tmp = &output[outlen - FsmSw_Convert_u8_to_u32(remainder)];
    for (uint8 i = 0; i < remainder; ++i)
    {
      output_tmp[i] = tmp[i];
    }
  }
} // end: FsmSw_Hqc256_SeedExpander

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */