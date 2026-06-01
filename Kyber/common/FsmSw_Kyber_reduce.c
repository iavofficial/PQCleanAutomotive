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
/** \addtogroup Kyber_reduce
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Kyber_reduce.c
* \brief  description of FsmSw_Kyber_reduce.c
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
#include "FsmSw_Kyber_params.h"
#include "Std_Types.h"

#include "FsmSw_Kyber_reduce.h"
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
* \brief Montgomery reduction; given a 32-bit integer a, computes
*        16-bit integer congruent to a * R^-1 mod q, where R=2^16
*
* \param[in] sint32 a : input integer to be reduced;
*                       has to be in {-q2^15,...,q2^15-1}
*
* \returns integer in {-q+1,...,q-1} congruent to a * R^-1 modulo q.
*/
sint16 FsmSw_Kyber_MontgomeryReduce(sint32 a)
{
  sint16 t = 0;

  t = (sint16)a * QINV;
  t = (sint16)((uint16)(((uint32)a - ((uint32)t * KYBER_Q)) >> 16));
  return t;
} // end: FsmSw_Kyber_MontgomeryReduce

/*====================================================================================================================*/
/**
* \brief Barrett reduction; given a 16-bit integer a, computes
*        centered representative congruent to a mod q in {-(q-1)/2,...,(q-1)/2}
*
* \param[in] sint16 a : input integer to be reduced
*
* \returns integer in {-(q-1)/2,...,(q-1)/2} congruent to a modulo q.
*/
sint16 FsmSw_Kyber_BarrettReduce(sint16 a)
{
  sint16 t = 0;
  /* polyspace +3 DEFECT:UINT_CONSTANT_OVFL [To fix:]"Due to the shift and the cast, temp0 becomes 0. Because we don't 
     want to deviate too much from the original functionality of the repo, the shift key remains active. We will check  
     the violation again once the repo has been set to read-only." */
  const sint16 temp0 = (sint16)(uint16)((uint32)1u << (uint16)25u);
  sint32 temp1       = 0;
  uint32 temp2       = 0;
  const sint16 v     = (sint16)((uint16)((((uint32)1 << 26u) + KYBER_Q / 2u) / KYBER_Q));

  /* Polyspace cannot resolve the operation if the shift operation is inserted instead of temp0. */
  /* polyspace +3 MISRA2012:2.2 [To fix:]"Due to the shift and the cast, temp0 becomes 0. Because we don't want to 
     deviate too much from the original functionality of the repo, the shift key remains active. We will check the 
     violation again once the repo has been set to read-only." */
  temp1 = ((sint32)v * (sint32)a) + (sint32)temp0;

  /* Check the first bit */
  if (((uint32)temp1 & 0x80000000u) != 0u)
  {
    temp2 = ((uint32)temp1 >> 26u) | 0xFFFFFFC0u;
  }
  else
  {
    temp2 = ((uint32)temp1 >> 26u);
  }
  t = (sint16)((sint32)temp2);
  t = (sint16)((uint16)(((uint32)t * KYBER_Q)));

  return a - t;
} // end: FsmSw_Kyber_BarrettReduce

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */