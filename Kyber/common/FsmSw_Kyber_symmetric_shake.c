/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup common
*    includes the modules for common
 ** @{ */
/** \addtogroup Kyber_symmetric_shake
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Kyber_symmetric_shake.c
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
#include "FsmSw_Kyber_params.h"
#include "FsmSw_Types.h"

#include "FsmSw_Kyber_symmetric.h"
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
* \brief Absorb step of the SHAKE128 specialized for the Kyber context.
*
* \param[out] xof_state      *s : pointer to (uninitialized) output Keccak state
* \param[in]  const uint8 *seed : pointer to KYBER_SYMBYTES input to be absorbed into state
* \param[in]  uint8           i : additional byte of input
* \param[in]  uint8           j : additional byte of input
*/
void FsmSw_Kyber_Shake128_Absorb(xof_state *const s, const uint8 seed[KYBER_SYMBYTES], uint8 x, uint8 y)
{
  uint8 extseed[KYBER_SYMBYTES + 2u] = {0};

  FsmSw_CommonLib_MemCpy(extseed, seed, KYBER_SYMBYTES);
  extseed[KYBER_SYMBYTES]      = x;
  extseed[KYBER_SYMBYTES + 1u] = y;

  FsmSw_Fips202_Shake128_Absorb(s, extseed, sizeof(extseed));
} // end: FsmSw_Kyber_Shake128_Absorb

/*====================================================================================================================*/
/**
* \brief Usage of SHAKE256 as a PRF, concatenates secret and public input
*        and then generates outlen bytes of SHAKE256 output
*
* \param[out] uint8       *out : pointer to output
* \param[out] uint32    outlen : number of requested output bytes
* \param[in]  const uint8 *key : pointer to the key (of length KYBER_SYMBYTES)
* \param[in]  uint8      nonce : single-byte nonce (public PRF input)
*/
void FsmSw_Kyber_Shake256_Prf(uint8 *const out, uint32 outlen, const uint8 key[KYBER_SYMBYTES], uint8 nonce)
{
  uint8 extkey[KYBER_SYMBYTES + 1u] = {0};

  FsmSw_CommonLib_MemCpy(extkey, key, KYBER_SYMBYTES);
  extkey[KYBER_SYMBYTES] = nonce;

  FsmSw_Fips202_Shake256(out, outlen, extkey, sizeof(extkey));
} // end: FsmSw_Kyber_Shake256_Prf

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */