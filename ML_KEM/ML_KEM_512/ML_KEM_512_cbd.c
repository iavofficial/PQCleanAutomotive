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
/** \addtogroup ML_KEM_512
*    includes the modules for ML_KEM_512
 ** @{ */
/** \addtogroup ML_KEM_512_cbd
 ** @{ */

/*====================================================================================================================*/
/** \file ML_KEM_512_cbd.c
* \brief  description of ML_KEM_512_cbd.c
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
#include "ML_KEM_512_params.h"
#include "ML_KEM_CommonLib.h"
#include "Std_Types.h"

#include "ML_KEM_512_cbd.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define ML_KEM_512_CBD_BLOCK_SIZE 4u
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
static uint32 ml_kem_512_Load24LittleEndian(const uint8 x[3]);
static void ml_kem_512_Cbd3(poly *const r, const uint8 buf[3u * ML_KEM_N / 4u]);

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
* \brief load 3 bytes into a 32-bit integer
*        in little-endian order.
*        This function is only needed for ML_KEM-512
*
* \param[in] const uint8 *x : pointer to input byte array
*
* \returns 32-bit unsigned integer loaded from x (most significant byte is zero)
*/
static uint32 ml_kem_512_Load24LittleEndian(const uint8 x[3])
{
  uint32 r = 0;

  r = (uint32)x[0];
  r |= (uint32)x[1] << 8;
  r |= (uint32)x[2] << 16;

  return r;
} // end: ml_kem_512_Load24LittleEndian

/*====================================================================================================================*/
/**
* \brief Given an array of uniformly random bytes, compute
*        polynomial with coefficients distributed according to
*        a centered binomial distribution with parameter eta=3.
*        This function is only needed for ML_KEM-512
*
* \param[out] poly          *r : pointer to output polynomial
* \param[in]  const uint8 *buf : pointer to input byte array
*/
static void ml_kem_512_Cbd3(poly *const r, const uint8 buf[3u * ML_KEM_N / 4u])
{
  uint8 i  = 0;
  uint8 j  = 0;
  uint32 t = 0;
  uint32 d = 0;
  sint16 a = 0;
  sint16 b = 0;

  for (i = 0; i < (ML_KEM_N / 4u); i++)
  {
    t = ml_kem_512_Load24LittleEndian(&buf[3u * i]);
    d = t & 0x00249249u;
    d += (t >> 1u) & 0x00249249u;
    d += (t >> 2u) & 0x00249249u;

    for (j = 0; j < ML_KEM_512_CBD_BLOCK_SIZE; j++)
    {
      a                       = (sint16)((uint16)((uint16)((d >> ((6u * j)))) & 0x7u));
      b                       = (sint16)((uint16)((uint16)((d >> ((6u * j) + 3u))) & 0x7u));
      r->coeffs[(4u * i) + j] = a - b;
    }
  }
} // end: ml_kem_512_Cbd3
/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
* \brief Given an array of uniformly random bytes, compute
*        polynomial with coefficients distributed according to
*        a centered binomial distribution with parameter eta=3.
*        This function is only needed for ML_KEM-512
*
* \param[out] poly          *r : pointer to output polynomial
* \param[in]  const uint8 *buf : pointer to input byte array
*/
void ML_KEM_512_Poly_Cbd_Eta1(poly *const r, const uint8 buf[ML_KEM_512_ETA1 * ML_KEM_N / 4u])
{
  ml_kem_512_Cbd3(r, buf);
} // end: ML_KEM_512_Poly_Cbd_Eta1

/*====================================================================================================================*/
/**
* \brief Given an array of uniformly random bytes, compute
*        polynomial with coefficients distributed according to
*        a centered binomial distribution with parameter eta=2
*
* \param[out] poly          *r : pointer to output polynomial
* \param[in]  const uint8 *buf : pointer to input byte array
*/
void ML_KEM_512_Poly_Cbd_Eta2(poly *const r, const uint8 buf[ML_KEM_512_ETA2 * ML_KEM_N / 4u])
{
  ML_KEM_Cbd2(r, buf);
} // end: ML_KEM_512_Poly_Cbd_Eta2

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */