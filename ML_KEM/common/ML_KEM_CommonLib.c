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
/** \addtogroup common
*    includes the modules for common
 ** @{ */
/** \addtogroup ML_KEM_CommonLib
 ** @{ */

/*====================================================================================================================*/
/** \file ML_KEM_CommonLib.c
* \brief  description of ML_KEM_CommonLib.c
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
#include "ML_KEM_CommonLib.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define ML_KEM_COMMON_LIB_CBD2_NIBBLES_NUM 8u
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
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
* \brief load 4 bytes into a 32-bit integer
*              in little-endian order
*
* \param[in] const uint8 *x : pointer to input byte array
*
* \returns 32-bit unsigned integer loaded from x
*/

/* 
  polyspace +5 CERT-C:DCL19-C [Justified:] 
  polyspace +4 CERT-C:DCL15-C [Justified:] 
  polyspace +3 MISRA2012:8.7 [Justified:] 
  "Common library utilities should be avaliable for usage if needed"
*/
uint32 ML_KEM_Load32LittleEndian(const uint8 x[4])
{
  uint32 r = 0;

  r = (uint32)x[0];
  r |= (uint32)x[1] << 8u;
  r |= (uint32)x[2] << 16u;
  r |= (uint32)x[3] << 24u;

  return r;
} // end: ML_KEM_Load32LittleEndian

/*====================================================================================================================*/
/**
* \brief       Given an array of uniformly random bytes, compute
*              polynomial with coefficients distributed according to
*              a centered binomial distribution with parameter eta=2
*
* \param[out] poly          *r : pointer to output polynomial
* \param[in]  const uint8 *buf : pointer to input byte array
*/
void ML_KEM_Cbd2(poly *r, const uint8 buf[2u * ML_KEM_N / 4u])
{
  uint8 i  = 0;
  uint8 j  = 0;
  uint32 t = 0;
  uint32 d = 0;
  sint16 a = 0;
  sint16 b = 0;

  for (i = 0; i < (ML_KEM_N / 8u); i++)
  {
    t = ML_KEM_Load32LittleEndian(&buf[4u * i]);
    d = t & 0x55555555u;
    d += (t >> 1u) & 0x55555555u;

    for (j = 0; j < ML_KEM_COMMON_LIB_CBD2_NIBBLES_NUM; j++)
    {
      a                       = (sint16)((uint16)((uint16)((d >> ((4u * j)))) & 0x3u));
      b                       = (sint16)((uint16)((uint16)((d >> ((4u * j) + 2u))) & 0x3u));
      r->coeffs[(8u * i) + j] = a - b;
    }
  }
} // end: ML_KEM_Cbd2

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */