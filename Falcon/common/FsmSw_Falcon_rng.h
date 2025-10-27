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
/** \addtogroup Falcon_rng
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Falcon_rng.h
* \brief  description of FsmSw_Falcon_rng.h
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
#ifndef FSMSW_FALCON_RNG_H
#define FSMSW_FALCON_RNG_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/

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
    uint8 d[512]; /* MUST be 512, exactly */
    uint64 dummy_u64;
  } buf;

  uint32 ptr;

  /* polyspace +2 MISRA2012:19.2 [Justified:]"The buffer and the remainder of the code require 
    the use of the union keyword." */
  union
  {
    uint8 d[256];
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
void FsmSw_Falcon_Prng_Init(prng *p, inner_shake256_context *const src);
void FsmSw_Falcon_Prng_GetBytes(prng *const p, void *const dst, uint32 len);
uint64 FsmSw_Falcon_Prng_GetU64(prng *const p);
uint32 FsmSw_Falcon_Prng_GetU8(prng *const p);

#endif /* FSMSW_FALCON_RNG_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */