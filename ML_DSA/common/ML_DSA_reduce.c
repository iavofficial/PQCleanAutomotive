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
/** \addtogroup common
*    includes the modules for common
 ** @{ */
/** \addtogroup ML_DSA_reduce
 ** @{ */

/*====================================================================================================================*/
/** \file ML_DSA_reduce.c
* \brief  description of ML_DSA_reduce
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
#include "ML_DSA_params.h"
#include "Std_Types.h"

#include "ML_DSA_reduce.h"
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
* \brief For finite field element a with -2^{31}Q <= a <= Q*2^31,
*              compute t \equiv a*2^{-32} (mod Q) such that -Q < r < Q.
*
* \param[in] sint64 : finite field element a
*
* \returns t .
*/
sint32 ML_DSA_MontgomeryReduce(sint64 a)
{
  sint32 t    = 0;
  sint64 temp = 0;

  t = (sint32)((uint32)((uint64)a * (uint64)QINV));

  temp = a - ((sint64)t * (sint64)Q_ML_DSA);
  t    = (sint32)((uint32)((uint64)temp >> 32));

  return t;
} // end: ML_DSA_MontgomeryReduce

/*====================================================================================================================*/
/** 
* \brief For finite field element a with a <= 2^{31} - 2^{22} - 1,
*              compute t \equiv a (mod Q) such that -6283008 <= r <= 6283008.
*
* \param[in] sint32 a : finite field element a
*
* Returns t.
*/
sint32 ML_DSA_Reduce32(sint32 a)
{
  sint32 t    = 0;
  sint32 temp = 0;

  temp = a + (sint32)((uint32)((uint32)1 << 22u));
  t    = (sint32)((uint32)((uint64)temp >> 23));

  t = a - (t * Q_ML_DSA);

  return t;
} // end: ML_DSA_Reduce32

/*====================================================================================================================*/
/** 
* \brief Add Q if input coefficient is negative.
*
* \param[in] sint32 a : finite field element a
*
* Returns result of addition.
*/
sint32 ML_DSA_CAddQ(sint32 a)
{
  uint32 temp = 0;

  /* a_temp is used to avoid modifying the input. */
  sint32 a_temp = a;

  temp   = (uint32)((uint64)a_temp >> 31);
  a_temp = a_temp + (sint32)((uint32)(temp & (uint32)Q_ML_DSA));

  return a_temp;
} // end: ML_DSA_CAddQ

/*====================================================================================================================*/
/** 
* \brief For finite field element a, compute standard
*              representative r = a mod^+ Q.
*
* \param[in] sint32 a : finite field element a
*
* Returns r.
*/
sint32 ML_DSA_Freeze(sint32 a)
{
  /* a_temp is used to avoid modifying the input. */
  sint32 a_temp = a;

  a_temp = ML_DSA_Reduce32(a_temp);
  a_temp = ML_DSA_CAddQ(a_temp);

  return a_temp;
} // end: ML_DSA_Freeze

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
