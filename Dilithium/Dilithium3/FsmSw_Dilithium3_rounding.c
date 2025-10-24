/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Dilithium3
*    includes the modules for Dilithium3
 ** @{ */
/** \addtogroup FsmSw_Dilithium3_rounding
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Dilithium3_rounding.c
* \brief  description of FsmSw_Dilithium3_rounding.c
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
#include "FsmSw_Dilithium3_params.h"

#include "FsmSw_Dilithium3_rounding.h"
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
* \brief For finite field element a, compute a0, a1 such that a mod^+ Q = a1*2^D + a0 with
*              -2^{D-1} < a0 <= 2^{D-1}. Assumes a to be standard representative.
*
* \param[in]  sint32   a : input element
* \param[out] sint32 *a0 : pointer to output element a0
*
* \returns a1.
*/
sint32 FsmSw_Dilithium3_Power2Round(sint32 *const a0, sint32 a)
{
  sint32 a1   = 0;
  sint32 temp = 0;

  temp = (a + (sint32)4095u);
  a1   = (sint32)((uint32)((uint32)temp >> D_DILITHIUM));
  *a0  = a - (sint32)((uint32)((uint32)a1 << D_DILITHIUM));

  return a1;
} // end: FsmSw_Dilithium3_Power2Round
/*====================================================================================================================*/
/**
* \brief For finite field element a, compute high and low bits a0, a1 such that a mod^+ Q = a1*ALPHA + a0 with
*              -ALPHA/2 < a0 <= ALPHA/2 except if a1 = (Q-1)/ALPHA where we set a1 = 0 and
*              -ALPHA/2 <= a0 = a mod^+ Q - Q < 0. Assumes a to be standard representative.
*
* \param[in]  sint32   a : input element
* \param[out] sint32 *a0 : pointer to output element a0
*
* \returns a1.
*/
sint32 FsmSw_Dilithium3_Decompose(sint32 *const a0, sint32 a)
{
  sint32 a1    = 0;
  sint32 temp1 = 0;
  sint32 temp2 = 0;
  sint32 temp3 = 0;

  a1 = (sint32)((uint32)(((uint32)a + 127u) >> 7));

  /* (1u << 21) = 2.097.152 */
  temp1 = ((a1 * 1025) + (sint32)2097152u);
  a1    = (sint32)((uint32)((uint32)temp1 >> 22));
  a1    = (sint32)((uint32)((uint32)a1 & 15u));

  *a0   = a - (a1 * (2 * GAMMA2_DILITHIUM3));
  temp2 = ((Q_DILITHIUM - 1) / 2 - *a0);
  temp3 = (sint32)((uint32)((uint64)temp2 >> 31));
  *a0   = *a0 - (sint32)((uint32)((uint32)temp3 & (uint32)Q_DILITHIUM));

  return a1;
} // end: FsmSw_Dilithium3_Decompose
/*====================================================================================================================*/
/**
* \brief Compute hint bit indicating whether the low bits of the input element overflow into the high bits.
*
* \param[in] sint32 a0 : low bits of input element
* \param[in] sint32 a1 : high bits of input element
*
* \returns 1 if overflow.
*/
uint8 FsmSw_Dilithium3_MakeHint(sint32 a0, sint32 a1)
{
  uint8 retVal = 0;

  if ((a0 > GAMMA2_DILITHIUM3) || (a0 < -GAMMA2_DILITHIUM3) || (((a0 == -GAMMA2_DILITHIUM3) && (a1 != 0)) != 0))
  {
    retVal = 1;
  }

  return retVal;
} // end: FsmSw_Dilithium3_MakeHint
/*====================================================================================================================*/
/**
* \brief Correct high bits according to hint.
*
* \param[in] sint32    a : input element
* \param[in] uint32 hint : hint bit
*
* \returns corrected high bits.
*/
sint32 FsmSw_Dilithium3_UseHint(sint32 a, uint32 hint)
{
  sint32 a0     = 0;
  sint32 a1     = 0;
  sint32 retVal = 0;

  a1 = FsmSw_Dilithium3_Decompose(&a0, a);

  if (hint == 0u)
  {
    retVal = a1;
  }

  else if (a0 > 0)
  {
    retVal = (sint32)((uint32)(((uint32)a1 + 1u) & 15u));
  }

  else
  {
    retVal = (sint32)((uint32)(((uint32)a1 - 1u) & 15u));
  }

  return retVal;
} // end: FsmSw_Dilithium3_UseHint

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
