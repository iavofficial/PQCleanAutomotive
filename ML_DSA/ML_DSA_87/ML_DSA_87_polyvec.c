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
/** \addtogroup ML_DSA_87
*    includes the modules for ML_DSA_87
 ** @{ */
/** \addtogroup ML_DSA_87_polyvec
 ** @{ */

/*====================================================================================================================*/
/** \file ML_DSA_87_polyvec.c
* \brief  description of ML_DSA_87_polyvec.c
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
#include "ML_DSA_87_params.h"
#include "ML_DSA_87_poly.h"
#include "Std_Types.h"

#include "ML_DSA_87_polyvec.h"
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
static void ML_DSA_87_Polyvecl_Pointwise_Acc_Montgomery(poly_D5 *const w, const polyvecl_D5 *const u,
                                                               const polyvecl_D5 *const v);
/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/
/*====================================================================================================================*/
/**
* \brief Pointwise multiply vectors of polynomials of length L, multiply resulting vector by 2^{-32}
*              and add (accumulate) polynomials in it. Input/output vectors are in NTT domain representation.
*
* \param[out] poly_D5           *w : output polynomial
* \param[in]  const polyvecl_D5 *u : pointer to first input vector
* \param[in]  const polyvecl_D5 *v : pointer to second input vector
*/
static void ML_DSA_87_Polyvecl_Pointwise_Acc_Montgomery(poly_D5 *const w, const polyvecl_D5 *const u,
                                                               const polyvecl_D5 *const v)
{

  uint8 i   = 0;
  poly_D5 t = {{0}};

  ML_DSA_87_Poly_PointwiseMontgomery(w, &u->vec[0], &v->vec[0]);
  for (i = 1; i < L_ML_DSA_87; ++i)
  {
    ML_DSA_87_Poly_PointwiseMontgomery(&t, &u->vec[i], &v->vec[i]);
    ML_DSA_87_Poly_Add(w, w, &t);
  }
} // end: ML_DSA_87_Polyvecl_Pointwise_Acc_Montgomery
/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/*====================================================================================================================*/
/**
* \brief Implementation of ExpandA. Generates matrix mat with uniformly random coefficients a_{i,j}
*              by performing rejection sampling on the output stream of SHAKE128(rho|j|i)
*
* \param[out] polyvecl_D5        mat[K_ML_DSA_87] : output matrix
* \param[in]  const uint8 rho[SEEDBYTES_ML_DSA] : byte array containing seed rho
*/
void ML_DSA_87_Polyvec_MatrixExpand(polyvecl_D5 mat[K_ML_DSA_87], const uint8 rho[SEEDBYTES_ML_DSA])
{
  uint8 i = 0;
  uint8 j = 0;

  for (i = 0; i < K_ML_DSA_87; ++i)
  {
    for (j = 0; j < L_ML_DSA_87; ++j)
    {
      ML_DSA_87_Poly_Uniform(&mat[i].vec[j], rho, (uint16)(((uint16)i << 8) + j));
    }
  }
} // end: ML_DSA_87_Polyvec_MatrixExpand
/*====================================================================================================================*/
/**
* \brief t.b.d
*
* \param[out] polyvecl_D5                      *t : t.b.d
* \param[in]  const polyvecl_D5 mat[K_ML_DSA_87] : t.b.d
* \param[in]  const polyvecl_D5                *v : t.b.d
***********************************************************************************************************************/
void ML_DSA_87_Polyvec_MatrixPointwiseMontgomery(polyveck_D5 *t, const polyvecl_D5 mat[K_ML_DSA_87],
                                                        const polyvecl_D5 *const v)
{
  uint8 i = 0;

  for (i = 0; i < K_ML_DSA_87; ++i)
  {
    ML_DSA_87_Polyvecl_Pointwise_Acc_Montgomery(&t->vec[i], &mat[i], v);
  }
} // end: ML_DSA_87_Polyvec_MatrixPointwiseMontgomery
/*====================================================================================================================*/
/**
* \brief t.b.d
*
* \param[out] polyvecl_D5                       *v : t.b.d
* \param[in]  const uint8 seed[CRHBYTES_ML_DSA] : t.b.d
* \param[in]  uint16                         nonce : t.b.d
*/
void ML_DSA_87_Polyvecl_UniformEta(polyvecl_D5 *v, const uint8 seed[CRHBYTES_ML_DSA], uint16 nonce)
{
  uint8 i = 0;
  /* nonce_temp is used to avoid modifying the input. */
  uint16 nonce_temp = nonce;

  for (i = 0; i < L_ML_DSA_87; ++i)
  {
    ML_DSA_87_Poly_UniformEta(&v->vec[i], seed, nonce_temp);
    nonce_temp++;
  }
} // end: ML_DSA_87_Polyvecl_UniformEta
/*====================================================================================================================*/
/**
* \brief t.b.d
*
* \param[out] polyvecl_D5                       *v : t.b.d
* \param[in]  const uint8 seed[CRHBYTES_ML_DSA] : t.b.d
* \param[in]  uint16                         nonce : t.b.d
*/
void ML_DSA_87_Polyvecl_UniformGamma1(polyvecl_D5 *v, const uint8 seed[CRHBYTES_ML_DSA], uint16 nonce)
{
  uint8 i = 0;

  for (i = 0; i < L_ML_DSA_87; ++i)
  {
    ML_DSA_87_Poly_UniformGamma1(&v->vec[i], seed, (uint16)((L_ML_DSA_87 * nonce) + i));
  }
} // end: ML_DSA_87_Polyvecl_UniformGamma1
/*====================================================================================================================*/
/**
* \brief t.b.d
*
* \param[out] polyvecl_D5 *v : t.b.d
*/
void ML_DSA_87_Polyvecl_Reduce(polyvecl_D5 *v)
{
  uint8 i = 0;

  for (i = 0; i < L_ML_DSA_87; ++i)
  {
    ML_DSA_87_Poly_Reduce(&v->vec[i]);
  }
} // end: ML_DSA_87_Polyvecl_Reduce
/*====================================================================================================================*/
/**
* \brief Add vectors of polynomials of length L_ML_DSA_87. No modular reduction is performed.
*
* \param[out] polyvecl_D5       *w : pointer to output vector
* \param[in]  const polyvecl_D5 *u : pointer to first summand
* \param[in]  const polyvecl_D5 *v : pointer to second summand
*/
void ML_DSA_87_Polyvecl_Add(polyvecl_D5 *w, const polyvecl_D5 *const u, const polyvecl_D5 *const v)
{
  uint8 i = 0;

  for (i = 0; i < L_ML_DSA_87; ++i)
  {
    ML_DSA_87_Poly_Add(&w->vec[i], &u->vec[i], &v->vec[i]);
  }
} // end: ML_DSA_87_Polyvecl_Add
/*====================================================================================================================*/
/**
* \brief Forward NTT of all polynomials in vector of length L_ML_DSA_87.
*              Output coefficients can be up to 16*Q larger than input coefficients.
*
* \param[in,out] polyvecl_D5 *v : pointer to input/output vector
*/
void ML_DSA_87_Polyvecl_Ntt(polyvecl_D5 *v)
{
  uint8 i = 0;

  for (i = 0; i < L_ML_DSA_87; ++i)
  {
    ML_DSA_87_Poly_Ntt(&v->vec[i]);
  }
} // end: ML_DSA_87_Polyvecl_Ntt
/*====================================================================================================================*/
/**
* \brief t.b.d
*
* \param[in,out] polyvecl_D5 *v: pointer to input/output vector
*/
void ML_DSA_87_Polyvecl_InvnttTomont(polyvecl_D5 *v)
{
  uint8 i = 0;

  for (i = 0; i < L_ML_DSA_87; ++i)
  {
    ML_DSA_87_Poly_InvnttTomont(&v->vec[i]);
  }
} // end: ML_DSA_87_Polyvecl_InvnttTomont
/*====================================================================================================================*/
/**
* \brief t.b.d
*
* \param[out] polyvecl_D5       *r : t.b.d
* \param[in]  const poly_D5     *a : t.b.d
* \param[in]  const polyvecl_D5 *v : t.b.d
*/
void ML_DSA_87_Polyvecl_PointwisePolyMontgomery(polyvecl_D5 *r, const poly_D5 *const a,
                                                       const polyvecl_D5 *const v)
{
  uint8 i = 0;

  for (i = 0; i < L_ML_DSA_87; ++i)
  {
    ML_DSA_87_Poly_PointwiseMontgomery(&r->vec[i], a, &v->vec[i]);
  }
} // end: ML_DSA_87_Polyvecl_PointwisePolyMontgomery
/*====================================================================================================================*/
/**
* \brief Check infinity norm of polynomials in vector of length L.
*              Assumes input polyvecl_D5 to be reduced by ML_DSA_87_Polyvecl_Reduce().
*
* \param[in] const polyvecl_D5 *v : pointer to vector
* \param[in] int32_t            B : norm bound
*
* \returns 0 if norm of all polynomials is strictly smaller than B <= (Q-1)/8
* and 1 otherwise.
*/
sint8 ML_DSA_87_Polyvecl_Chknorm(const polyvecl_D5 *const v, sint32 bound)
{
  uint8 i      = 0;
  sint8 retVal = 0;

  for (i = 0; i < L_ML_DSA_87; ++i)
  {
    if (0 < ML_DSA_87_Poly_Chknorm(&v->vec[i], bound))
    {
      retVal = 1;
    }
  }

  return retVal;
} // end: ML_DSA_87_Polyvecl_Chknorm
/*====================================================================================================================*/
/**
* \brief t.b.d
*
* \param[out] polyvecl_D5                       *v : t.b.d
* \param[in]  const uint8 seed[CRHBYTES_ML_DSA] : t.b.d
* \param[in]  uint16                         nonce : t.b.d
*/
void ML_DSA_87_Polyveck_UniformEta(polyveck_D5 *v, const uint8 seed[CRHBYTES_ML_DSA], uint16 nonce)
{
  uint8 i = 0;
  /* nonce_temp is used to avoid modifying the input. */
  uint16 nonce_temp = nonce;

  for (i = 0; i < K_ML_DSA_87; ++i)
  {
    ML_DSA_87_Poly_UniformEta(&v->vec[i], seed, nonce_temp);
    nonce_temp++;
  }
} // end: ML_DSA_87_Polyveck_UniformEta
/*====================================================================================================================*/
/**
* \brief Reduce coefficients of polynomials in vector of length K_ML_DSA_87 to representatives in
*              [-6283008,6283008].
*
* \param[in,out] polyveck_D5 *v : pointer to input/output vector
*/
void ML_DSA_87_Polyveck_Reduce(polyveck_D5 *v)
{
  uint8 i = 0;

  for (i = 0; i < K_ML_DSA_87; ++i)
  {
    ML_DSA_87_Poly_Reduce(&v->vec[i]);
  }
} // end: ML_DSA_87_Polyveck_Reduce
/*====================================================================================================================*/
/**
* \brief For all coefficients of polynomials in vector of length K_ML_DSA_87 add Q if coefficient is negative.
*
* \param[in,out] polyveck_D5 *v : pointer to input/output vector
*/
void ML_DSA_87_Polyveck_CAddQ(polyveck_D5 *v)
{
  uint8 i = 0;

  for (i = 0; i < K_ML_DSA_87; ++i)
  {
    ML_DSA_87_Poly_CAddQ(&v->vec[i]);
  }
} // end: ML_DSA_87_Polyveck_CAddQ
/*====================================================================================================================*/
/**
* \brief Add vectors of polynomials of length K_ML_DSA_87. No modular reduction is performed.
*
* \param[out] polyveck_D5       *w : pointer to output vector
* \param[in]  const polyveck_D5 *u : pointer to first summand
* \param[in]  const polyveck_D5 *v : pointer to second summand
*/
void ML_DSA_87_Polyveck_Add(polyveck_D5 *w, const polyveck_D5 *const u, const polyveck_D5 *const v)
{
  uint8 i = 0;

  for (i = 0; i < K_ML_DSA_87; ++i)
  {
    ML_DSA_87_Poly_Add(&w->vec[i], &u->vec[i], &v->vec[i]);
  }
} // end: ML_DSA_87_Polyveck_Add
/*====================================================================================================================*/
/**
* \brief Subtract vectors of polynomials of length K_ML_DSA_87. No modular reduction is performed.
*
* \param[out] polyveck_D5       *w : pointer to output vector
* \param[in]  const polyveck_D5 *u : pointer to first input vector
* \param[in]  const polyveck_D5 *v : pointer to second input vector to be subtracted from first input vector
*/
void ML_DSA_87_Polyveck_Sub(polyveck_D5 *w, const polyveck_D5 *const u, const polyveck_D5 *const v)
{
  uint8 i = 0;

  for (i = 0; i < K_ML_DSA_87; ++i)
  {
    ML_DSA_87_Poly_Sub(&w->vec[i], &u->vec[i], &v->vec[i]);
  }
} // end: ML_DSA_87_Polyveck_Sub
/*====================================================================================================================*/
/**
* \brief Multiply vector of polynomials of Length K_ML_DSA_87 by 2^D without modular reduction.
*              Assumes input coefficients to be less than 2^{31-D}.
*
* \param[in,out] polyveck_D5 *v : pointer to input/output vector
*/
void ML_DSA_87_Polyveck_Shiftl(polyveck_D5 *v)
{
  uint8 i = 0;

  for (i = 0; i < K_ML_DSA_87; ++i)
  {
    ML_DSA_87_Poly_Shiftl(&v->vec[i]);
  }
} // end: ML_DSA_87_Polyveck_Shiftl
/*====================================================================================================================*/
/**
* \brief Forward NTT of all polynomials in vector of length K_ML_DSA_87.
*              Output coefficients can be up to 16*Q larger than input coefficients.
*
* \param[in,out] polyveck_D5 *v : pointer to input/output vector
*/
void ML_DSA_87_Polyveck_Ntt(polyveck_D5 *v)
{
  uint8 i = 0;

  for (i = 0; i < K_ML_DSA_87; ++i)
  {
    ML_DSA_87_Poly_Ntt(&v->vec[i]);
  }
} // end: ML_DSA_87_Polyveck_Ntt
/*====================================================================================================================*/
/**
* \brief Inverse NTT and multiplication by 2^{32} of polynomials in vector of length K_ML_DSA_87.
*              Input coefficients need to be less than 2*Q.
*
* \param[in,out] polyveck_D5 *v : pointer to input/output vector
*/
void ML_DSA_87_Polyveck_InvnttTomont(polyveck_D5 *v)
{
  uint8 i = 0;

  for (i = 0; i < K_ML_DSA_87; ++i)
  {
    ML_DSA_87_Poly_InvnttTomont(&v->vec[i]);
  }
} // end: ML_DSA_87_Polyveck_InvnttTomont
/*====================================================================================================================*/
/**
* \brief t.b.d
*
* \param[out] polyveck_D5       *r : t.b.d
* \param[in]  const poly_D5     *a : t.b.d
* \param[in]  const polyveck_D5 *v : t.b.d
*/
void ML_DSA_87_Polyveck_PointwisePolyMontgomery(polyveck_D5 *r, const poly_D5 *const a,
                                                       const polyveck_D5 *const v)
{
  uint8 i = 0;

  for (i = 0; i < K_ML_DSA_87; ++i)
  {
    ML_DSA_87_Poly_PointwiseMontgomery(&r->vec[i], a, &v->vec[i]);
  }
} // end: ML_DSA_87_Polyveck_PointwisePolyMontgomery
/*====================================================================================================================*/
/**
* \brief Check infinity norm of polynomials in vector of length K_ML_DSA_87.
*              Assumes input polyveck_D5to be reduced by ML_DSA_87_Polyveck_Reduce().
*
* \param[in] const polyveck_D5 *v : pointer to vector
* \param[in] sint32         bound : norm bound
*
* \returns 0 if norm of all polynomials are strictly smaller than B <= (Q-1)/8 and 1 otherwise.
*/
sint8 ML_DSA_87_Polyveck_Chknorm(const polyveck_D5 *const v, sint32 bound)
{
  uint8 i      = 0;
  sint8 retVal = 0;

  for (i = 0; i < K_ML_DSA_87; ++i)
  {
    if (0 < ML_DSA_87_Poly_Chknorm(&v->vec[i], bound))
    {
      retVal = 1;
    }
  }

  return retVal;
} // end: ML_DSA_87_Polyveck_Chknorm
/*====================================================================================================================*/
/**
* \brief For all coefficients a of polynomials in vector of length K_ML_DSA_87, compute a0, a1 such that a
*              mod^+ Q = a1*2^D + a0 with -2^{D-1} < a0 <= 2^{D-1}. Assumes coefficients to be standard representatives.
*
* \param[out] polyveck_D5      *v1 : pointer to output vector of polynomials with coefficients a1
* \param[out] polyveck_D5      *v0 : pointer to output vector of polynomials with coefficients a0
* \param[in]  const polyveck_D5 *v :  pointer to input vector
*/
void ML_DSA_87_Polyveck_Power2Round(polyveck_D5 *v1, polyveck_D5 *v0, const polyveck_D5 *const v)
{
  uint8 i = 0;

  for (i = 0; i < K_ML_DSA_87; ++i)
  {
    ML_DSA_87_Poly_Power2Round(&v1->vec[i], &v0->vec[i], &v->vec[i]);
  }
} // end: ML_DSA_87_Polyveck_Power2Round
/*====================================================================================================================*/
/**
* \brief For all coefficients a of polynomials in vector of length K_ML_DSA_87, compute high and low bits
*              a0, a1 such a mod^+ Q = a1*ALPHA + a0 with -ALPHA/2 < a0 <= ALPHA/2 except a1 = (Q-1)/ALPHA where we
*              set a1 = 0 and -ALPHA/2 <= a0 = a mod Q - Q < 0. Assumes coefficients to be standard representatives.
*
* \param[out] polyveck_D5 *v1: pointer to output vector of polynomials with coefficients a1
* \param[out] polyveck_D5 *v0: pointer to output vector of polynomials with coefficients a0
* \param[in]  const polyveck_D5 *v:  pointer to input vector
*/
void ML_DSA_87_Polyveck_Decompose(polyveck_D5 *v1, polyveck_D5 *v0, const polyveck_D5 *const v)
{
  uint8 i = 0;

  for (i = 0; i < K_ML_DSA_87; ++i)
  {
    ML_DSA_87_Poly_Decompose(&v1->vec[i], &v0->vec[i], &v->vec[i]);
  }
} // end: ML_DSA_87_Polyveck_Decompose
/*====================================================================================================================*/
/**
* \brief Compute hint vector.
*
* \param[out] polyveck_D5 *h:  pointer to output vector
* \param[in]  const polyveck_D5 *v0: pointer to low part of input vector
* \param[in]  const polyveck_D5 *v1: pointer to high part of input vector
*
* \returns number of 1 bits.
*/
uint32 ML_DSA_87_Polyveck_MakeHint(polyveck_D5 *h, const polyveck_D5 *const v0, const polyveck_D5 *const v1)
{
  uint8 i  = 0;
  uint32 s = 0;

  for (i = 0; i < K_ML_DSA_87; ++i)
  {
    s += ML_DSA_87_Poly_MakeHint(&h->vec[i], &v0->vec[i], &v1->vec[i]);
  }

  return s;
} // end: ML_DSA_87_Polyveck_MakeHint
/*====================================================================================================================*/
/**
* \brief Use hint vector to correct the high bits of input vector.
*
* \param[out] polyveck_D5       *w : pointer to output vector of polynomials with corrected high bits
* \param[in]  const polyveck_D5 *v : pointer to input vector
* \param[in]  const polyveck_D5 *h : pointer to input hint vector
*/
void ML_DSA_87_Polyveck_UseHint(polyveck_D5 *w, const polyveck_D5 *const v, const polyveck_D5 *const h)
{
  uint8 i = 0;

  for (i = 0; i < K_ML_DSA_87; ++i)
  {
    ML_DSA_87_Poly_UseHint(&w->vec[i], &v->vec[i], &h->vec[i]);
  }
} // end: ML_DSA_87_Polyveck_UseHint
/*====================================================================================================================*/
/**
* \brief t.b.d
*
* \param[in] uint8 r[K_ML_DSA_87 * POLYW1_PACKEDBYTES_ML_DSA_87] : t.b.d
* \param[in] const polyveck_D5                                 *w1 : t.b.d
*/
void ML_DSA_87_Polyveck_PackW1(uint8 r[K_ML_DSA_87 * POLYW1_PACKEDBYTES_ML_DSA_87],
                                      const polyveck_D5 *const w1)
{
  uint8 i = 0;

  for (i = 0; i < K_ML_DSA_87; ++i)
  {
    ML_DSA_87_Poly_W1Pack(&r[i * POLYW1_PACKEDBYTES_ML_DSA_87], &w1->vec[i]);
  }
} // end: ML_DSA_87_Polyveck_PackW1

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
