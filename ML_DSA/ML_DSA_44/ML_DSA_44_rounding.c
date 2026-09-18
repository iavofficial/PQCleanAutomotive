/***********************************************************************************************************************
 *
 * Original implementation: PQClean, ML-DSA
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
/** \addtogroup ML_DSA_44
*    includes the modules for ML_DSA_44
 ** @{ */
/** \addtogroup ML_DSA_44_rounding
 ** @{ */

/*====================================================================================================================*/
/** \file ML_DSA_rounding.c
* \brief  description of ML_DSA_rounding.c
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
#include "ML_DSA_44_params.h"

#include "ML_DSA_44_rounding.h"
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
sint32 ML_DSA_44_Power2Round(sint32 *const a0, sint32 a)
{
  sint32 a1   = 0;
  sint32 temp = 0;

  temp = (a + (sint32)4095u);
  a1   = (sint32)((uint32)((uint32)temp >> D_ML_DSA));
  *a0  = a - (sint32)((uint32)((uint32)a1 << D_ML_DSA));

  return a1;
} // end: ML_DSA_44_Power2Round
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
sint32 ML_DSA_44_Decompose(sint32 *const a0, sint32 a)
{
  sint32 a1    = 0;
  sint32 temp1 = 0;
  sint32 temp2 = 0;
  sint32 temp3 = 0;
  sint32 temp4 = 0;
  sint32 temp5 = 0;

  a1 = (sint32)((uint32)(((uint32)a + 127u) >> 7));
  /* (1u << 23) = 8.388.608 */
  temp1 = (a1 * 11275) + (sint32)8388608u;
  a1    = (sint32)((uint32)((uint32)temp1 >> 24));
  temp2 = ((sint32)43u - a1);
  temp3 = (sint32)((uint32)((uint64)temp2 >> 31));
  a1    = (sint32)((uint32)((uint32)a1 ^ ((uint32)temp3 & (uint32)a1)));

  *a0   = a - (a1 * (2 * GAMMA2_ML_DSA_44));
  temp4 = ((Q_ML_DSA - 1) / 2 - *a0);
  temp5 = (sint32)((uint32)((uint64)temp4 >> 31));
  *a0   = *a0 - (sint32)((uint32)((uint32)temp5 & (uint32)Q_ML_DSA));

  return a1;
} // end: ML_DSA_44_Decompose
/*====================================================================================================================*/
/**
* \brief Compute hint bit indicating whether the low bits of the input element overflow into the high bits.
*
* \param[in] sint32 a0 : low bits of input element
* \param[in] sint32 a1 : high bits of input element
*
* \returns 1 if overflow.
*/
uint8 ML_DSA_44_MakeHint(sint32 a0, sint32 a1)
{
  uint8 retVal = 0;

  if ((a0 > GAMMA2_ML_DSA_44) || (a0 < -GAMMA2_ML_DSA_44) || (((a0 == -GAMMA2_ML_DSA_44) && (a1 != 0)) != 0))
  {
    retVal = 1;
  }

  return retVal;
} // end: ML_DSA_44_MakeHint
/*====================================================================================================================*/
/**
* \brief Correct high bits according to hint.
*
* \param[in] sint32    a : input element
* \param[in] uint32 hint : hint bit
*
* \returns corrected high bits.
*/
sint32 ML_DSA_44_UseHint(sint32 a, uint32 hint)
{
  sint32 a0 = 0;
  sint32 a1 = 0;
  sint32 retVal;
  boolean bStopFunc = FALSE;

  a1     = ML_DSA_44_Decompose(&a0, a);
  retVal = a1 - 1;

  if (hint == 0u)
  {
    bStopFunc = TRUE;
    retVal    = a1;
  }

  if ((FALSE == bStopFunc) && (a0 > 0))
  {
    if (a1 == 43)
    {
      bStopFunc = TRUE;
      retVal    = 0;
    }
    if (FALSE == bStopFunc)
    {
      bStopFunc = TRUE;
      retVal    = a1 + 1;
    }
  }

  if ((FALSE == bStopFunc) && (a1 == 0))
  {
    retVal = 43;
  }

  return retVal;
} // end: ML_DSA_44_UseHint

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */