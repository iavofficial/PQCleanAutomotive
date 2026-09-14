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
/** \addtogroup ML_KEM_poly
 ** @{ */

/*====================================================================================================================*/
/** \file ML_KEM_poly.c
* \brief  description of ML_KEM_poly.c
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
#include "ML_KEM_ntt.h"
#include "ML_KEM_reduce.h"
#include "ML_KEM_symmetric.h"
#include "Std_Types.h"

#include "ML_KEM_poly.h"
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
* \brief Serialization of a polynomial
*
* \param[out] uint8      *r : pointer to output byte array (needs space for ML_KEM_POLYBYTES bytes)
* \param[in]  const poly *a : pointer to input polynomial
*/
void ML_KEM_Poly_ToBytes(uint8 r[ML_KEM_POLYBYTES], const poly *a)
{
  uint16 i  = 0;
  uint16 t0 = 0;
  uint16 t1 = 0;

  for (i = 0; i < (ML_KEM_N / 2u); i++)
  {
    /* map to positive standard representatives */
    t0 = (uint16)(a->coeffs[2u * i]);

    /* Shift to get the first bit */
    if ((t0 >> 15u) != 0u)
    {
      t0 = t0 + ML_KEM_Q;
    }

    t1 = (uint16)(a->coeffs[(2u * i) + 1u]);
    /* Shift to get the first bit */
    if ((t1 >> 15u) != 0u)
    {
      t1 = t1 + ML_KEM_Q;
    }

    r[3u * i]        = (uint8)(t0 >> 0);
    r[(3u * i) + 1u] = (uint8)((t0 >> 8u) | (t1 << 4u));
    r[(3u * i) + 2u] = (uint8)(t1 >> 4u);
  }
} // end: ML_KEM_Poly_ToBytes

/*====================================================================================================================*/
/**
* \brief De-serialization of a polynomial;
*        inverse of ML_KEM_Poly_ToBytes
*
* \param[out] poly        *r : pointer to output polynomial
* \param[in]  const uint8 *a : pointer to input byte array (of ML_KEM_POLYBYTES bytes)
*/
void ML_KEM_Poly_FromBytes(poly *r, const uint8 a[ML_KEM_POLYBYTES])
{
  uint16 i = 0;

  for (i = 0; i < (ML_KEM_N / 2u); i++)
  {
    r->coeffs[2u * i] =
        (sint16)((uint16)((((((uint16)a[3u * i]) >> 0u) | (((uint16)a[((3u * i) + 1u)]) << 8u))) & 0xFFFu));

    r->coeffs[(2u * i) + 1u] =
        (sint16)((uint16)((((((uint16)a[(3u * i) + 1u]) >> 4u) | (((uint16)a[(3u * i) + 2u]) << 4u))) & 0xFFFu));
  }
} // end: ML_KEM_Poly_FromBytes

/*====================================================================================================================*/
/**
* \brief Computes negacyclic number-theoretic transform (NTT) of
*        a polynomial in place;
*        inputs assumed to be in normal order, output in bitreversed order
*
* \param[in,out] uint16 *r : pointer to in/output polynomial
*/
void ML_KEM_Poly_Ntt(poly *r)
{
  ML_KEM_Ntt(r->coeffs);
  ML_KEM_Poly_Reduce(r);
} // end: ML_KEM_Poly_Ntt

/*====================================================================================================================*/
/**
* \brief Computes inverse of negacyclic number-theoretic transform (NTT)
*        of a polynomial in place;
*        inputs assumed to be in bitreversed order, output in normal order
*
* \param[in,out] uint16 *a : pointer to in/output polynomial
*/
void ML_KEM_Poly_InvnttTomont(poly *r)
{
  ML_KEM_Invntt(r->coeffs);
} // end: ML_KEM_Poly_InvnttTomont

/*====================================================================================================================*/
/**
* \brief Multiplication of two polynomials in NTT domain
*
* \param[out] poly       *r : pointer to output polynomial
* \param[in]  const poly *a : pointer to first input polynomial
* \param[in]  const poly *b : pointer to second input polynomial
*/
void ML_KEM_Poly_BasemulMontgomery(poly *r, const poly *a, const poly *b)
{
  uint16 i = 0;

  for (i = 0; i < (ML_KEM_N / 4u); i++)
  {
    ML_KEM_Basemul(&r->coeffs[4u * i], &a->coeffs[4u * i], &b->coeffs[4u * i], ML_KEM_zetas[64u + i]);

    ML_KEM_Basemul(&r->coeffs[(4u * i) + 2u], &a->coeffs[(4u * i) + 2u], &b->coeffs[(4u * i) + 2u],
                        -ML_KEM_zetas[64u + i]);
  }
} // end: ML_KEM_Poly_BasemulMontgomery

/*====================================================================================================================*/
/**
* \brief Inplace conversion of all coefficients of a polynomial
*        from normal domain to Montgomery domain
*
* \param[in,out] poly *r : pointer to input/output polynomial
*/
void ML_KEM_Poly_Tomont(poly *r)
{
  uint16 i       = 0;
  const sint16 f = (sint16)((1ULL << 32u) % ML_KEM_Q);

  for (i = 0; i < ML_KEM_N; i++)
  {
    r->coeffs[i] = ML_KEM_MontgomeryReduce((sint32)r->coeffs[i] * (sint32)f);
  }
} // end: ML_KEM_Poly_Tomont

/*====================================================================================================================*/
/**
* \brief Applies Barrett reduction to all coefficients of a polynomial
*        for details of the Barrett reduction see comments in reduce.c
*
* \param[in,out] poly *r : pointer to input/output polynomial
*/
void ML_KEM_Poly_Reduce(poly *r)
{
  uint16 i = 0;

  for (i = 0; i < ML_KEM_N; i++)
  {
    r->coeffs[i] = ML_KEM_BarrettReduce(r->coeffs[i]);
  }
} // end: ML_KEM_Poly_Reduce

/*====================================================================================================================*/
/**
* \brief Add two polynomials; no modular reduction is performed
*
* \param[out] poly       *r : pointer to output polynomial
* \param[in]  const poly *a : pointer to first input polynomial
* \param[in]  const poly *b : pointer to second input polynomial
*/
void ML_KEM_Poly_Add(poly *r, const poly *a, const poly *b)
{
  uint16 i = 0;

  for (i = 0; i < ML_KEM_N; i++)
  {
    r->coeffs[i] = a->coeffs[i] + b->coeffs[i];
  }
} // end: ML_KEM_Poly_Add

/*====================================================================================================================*/
/**
* \brief Subtract two polynomials; no modular reduction is performed
*
* \param[out] poly       *r : pointer to output polynomial
* \param[in]  const poly *a : pointer to first input polynomial
* \param[in]  const poly *b : pointer to second input polynomial
*/
void ML_KEM_Poly_Sub(poly *r, const poly *a, const poly *b)
{
  uint16 i = 0;

  for (i = 0; i < ML_KEM_N; i++)
  {
    r->coeffs[i] = a->coeffs[i] - b->coeffs[i];
  }
} // end: ML_KEM_Poly_Sub

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */