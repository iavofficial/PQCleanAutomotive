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
/** \addtogroup ML_KEM_768
*    includes the modules for ML_KEM_768
 ** @{ */
/** \addtogroup ML_KEM_768_polyvec
 ** @{ */

/*====================================================================================================================*/
/** \file ML_KEM_768_polyvec.c
* \brief  description of ML_KEM_768_polyvec.c
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
#include "ML_KEM_768_params.h"
#include "ML_KEM_768_poly.h"
#include "ML_KEM_poly.h"
#include "Std_Types.h"

#include "ML_KEM_768_polyvec.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define ML_KEM_768_POLYVEC_COMPRESS_BLOCK_SIZE 4u
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
* \brief Compress and serialize vector of polynomials
*
* \param[out] uint8         *r : pointer to output byte array
* \param[in]  const polyvec *a : pointer to input vector of polynomials
*/
void ML_KEM_768_Polyvec_Compress(uint8 r[ML_KEM_768_POLYVECCOMPRESSEDBYTES], const polyvec768 *const a)
{
  uint8 i                                              = 0;
  uint8 k                                              = 0;
  uint16 j                                             = 0;
  uint16 t[ML_KEM_768_POLYVEC_COMPRESS_BLOCK_SIZE] = {0};

  /* r_temp is used to avoid modifying the input. */
  uint8 *r_temp = r;

  for (i = 0; i < ML_KEM_768_K; i++)
  {
    for (j = 0; j < (ML_KEM_N / ML_KEM_768_POLYVEC_COMPRESS_BLOCK_SIZE); j++)
    {
      for (k = 0; (k < ML_KEM_768_POLYVEC_COMPRESS_BLOCK_SIZE); k++)
      {
        t[k] = (uint16)(a->vec[i].coeffs[(4u * j) + k]);
        /* Shift to get the first bit */
        if ((t[k] >> 15u) != 0u)
        {
          t[k] = t[k] + ML_KEM_Q;
        }
        t[k] = (uint16)(((t[k] << 10u) + (ML_KEM_Q / 2u)) / ML_KEM_Q) & 0x3ffu;
      }

      r_temp[0] = (uint8)(t[0] >> 0);
      r_temp[1] = (uint8)((t[0] >> 8) | (t[1] << 2));
      r_temp[2] = (uint8)((t[1] >> 6) | (t[2] << 4));
      r_temp[3] = (uint8)((t[2] >> 4) | (t[3] << 6));
      r_temp[4] = (uint8)(t[3] >> 2);
      r_temp    = &(r_temp[5]);
    }
  }
} // end: ML_KEM_768_Polyvec_Compress

/*====================================================================================================================*/
/**
* \brief De-serialize and decompress vector of polynomials;
*        approximate inverse of ML_KEM_768_Polyvec_Compress
*
* \param[out] polyvec768  *r : pointer to output vector of polynomials
* \param[in]  const uint8 *a : pointer to input byte array (of length ML_KEM_768_POLYVECCOMPRESSEDBYTES bytes)
*/
void ML_KEM_768_Polyvec_Decompress(polyvec768 *const r, const uint8 a[ML_KEM_768_POLYVECCOMPRESSEDBYTES])
{
  uint8 i                                              = 0;
  uint8 k                                              = 0;
  uint16 j                                             = 0;
  uint16 t[ML_KEM_768_POLYVEC_COMPRESS_BLOCK_SIZE] = {0};

  /* a_temp is used to avoid modifying the input. */
  const uint8 *a_temp = a;

  for (i = 0; i < ML_KEM_768_K; i++)
  {
    for (j = 0; j < (ML_KEM_N / 4u); j++)
    {
      t[0] = ((uint16)a_temp[0] >> 0) | ((uint16)a_temp[1] << 8);
      t[1] = ((uint16)a_temp[1] >> 2) | ((uint16)a_temp[2] << 6);
      t[2] = ((uint16)a_temp[2] >> 4) | ((uint16)a_temp[3] << 4);
      t[3] = ((uint16)a_temp[3] >> 6) | ((uint16)a_temp[4] << 2);
      /* Set address from pointer a[4] to address a[5] */
      a_temp = &(a_temp[5]);

      for (k = 0; k < ML_KEM_768_POLYVEC_COMPRESS_BLOCK_SIZE; k++)
      {
        r->vec[i].coeffs[(4u * j) + k] = (sint16)((uint16)((((t[k] & 0x3FFu) * ML_KEM_Q) + ML_KEM_768_IMPLBYTES) >> 10u));
      }
    }
  }
} // end: ML_KEM_768_Polyvec_Decompress

/*====================================================================================================================*/
/**
* \brief Serialize vector of polynomials
*
* \param[out] uint8            *r : pointer to output byte array (of length ML_KEM_768_POLYVECBYTES bytes)
* \param[in]  const polyvec768 *a : pointer to input vector of polynomials
*/
void ML_KEM_768_Polyvec_ToBytes(uint8 r[ML_KEM_768_POLYVECBYTES], const polyvec768 *const a)
{
  uint8 i = 0;

  for (i = 0; i < ML_KEM_768_K; i++)
  {
    ML_KEM_Poly_ToBytes(&(r[i * ML_KEM_POLYBYTES]), &a->vec[i]);
  }
} // end: ML_KEM_768_Polyvec_ToBytes

/*====================================================================================================================*/
/**
* \brief De-serialize vector of polynomials;
*        inverse of ML_KEM_768_Polyvec_ToBytes
*
* \param[out] uint8            *r : pointer to output byte array
* \param[in]  const polyvec768 *a : pointer to input vector of polynomials (of length ML_KEM_768_POLYVECBYTES bytes)
*/
void ML_KEM_768_Polyvec_FromBytes(polyvec768 *r, const uint8 a[ML_KEM_768_POLYVECBYTES])
{
  uint8 i = 0;

  for (i = 0; i < ML_KEM_768_K; i++)
  {
    ML_KEM_Poly_FromBytes(&r->vec[i], &a[i * ML_KEM_POLYBYTES]);
  }
} // end: ML_KEM_768_Polyvec_FromBytes

/*====================================================================================================================*/
/**
* \brief Apply forward NTT to all elements of a vector of polynomials
*
* \param[in,out] polyvec768 *r : pointer to in/output vector of polynomials
*/
void ML_KEM_768_Polyvec_Ntt(polyvec768 *r)
{
  uint8 i = 0;

  for (i = 0; i < ML_KEM_768_K; i++)
  {
    ML_KEM_Poly_Ntt(&r->vec[i]);
  }
} // end: ML_KEM_768_Polyvec_Ntt

/*====================================================================================================================*/
/**
* \brief Apply inverse NTT to all elements of a vector of polynomials
*        and multiply by Montgomery factor 2^16
*
* \param[in,out] polyvec768 *r : pointer to in/output vector of polynomials
*/
void ML_KEM_768_Polyvec_InvnttTomont(polyvec768 *r)
{
  uint8 i = 0;

  for (i = 0; i < ML_KEM_768_K; i++)
  {
    ML_KEM_Poly_InvnttTomont(&r->vec[i]);
  }
} // end: ML_KEM_768_Polyvec_InvnttTomont

/*====================================================================================================================*/
/**
* \brief Multiply elements of a and b in NTT domain, accumulate into r,
*        and multiply by 2^-16.
*
* \param[out] poly             *r : pointer to output polynomial
* \param[in]  const polyvec768 *a : pointer to first input vector of polynomials
* \param[in]  const polyvec768 *b : pointer to second input vector of polynomials
*/
void ML_KEM_768_Polyvec_BasemulAccMontgomery(poly *const r, const polyvec768 *const a, const polyvec768 *const b)
{
  uint8 i = 0;
  poly t  = {{0}};

  ML_KEM_Poly_BasemulMontgomery(r, &a->vec[0], &b->vec[0]);

  for (i = 1; i < ML_KEM_768_K; i++)
  {
    ML_KEM_Poly_BasemulMontgomery(&t, &a->vec[i], &b->vec[i]);
    ML_KEM_Poly_Add(r, r, &t);
  }

  ML_KEM_Poly_Reduce(r);
} // end: ML_KEM_768_Polyvec_BasemulAccMontgomery

/*====================================================================================================================*/
/**
* \brief Applies Barrett reduction to each coefficient
*        of each element of a vector of polynomials;
*        for details of the Barrett reduction see comments in reduce.c
*
* \param[in,out] polyvec768 *r : pointer to input/output polynomial
*/
void ML_KEM_768_Polyvec_Reduce(polyvec768 *r)
{
  uint8 i = 0;

  for (i = 0; i < ML_KEM_768_K; i++)
  {
    ML_KEM_Poly_Reduce(&r->vec[i]);
  }
} // end: ML_KEM_768_Polyvec_Reduce

/*====================================================================================================================*/
/**
* \brief Add vectors of polynomials
*
* \param[out] polyvec768       *r : pointer to output vector of polynomials
* \param[in]  const polyvec768 *a : pointer to first input vector of polynomials
* \param[in]  const polyvec768 *b : pointer to second input vector of polynomials
*/
void ML_KEM_768_Polyvec_Add(polyvec768 *r, const polyvec768 *const a, const polyvec768 *const b)
{
  uint8 i = 0;

  for (i = 0; i < ML_KEM_768_K; i++)
  {
    ML_KEM_Poly_Add(&r->vec[i], &a->vec[i], &b->vec[i]);
  }
} // end: ML_KEM_768_Polyvec_Add

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */