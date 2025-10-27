/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Dilithium2
*    includes the modules for Dilithium2
 ** @{ */
/** \addtogroup FsmSw_Dilithium2_polyvec
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Dilithium_polyvec.c
* \brief  description of FsmSw_Dilithium_polyvec
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
#include "FsmSw_Dilithium2_params.h"
#include "FsmSw_Dilithium2_poly.h"
#include "FsmSw_Types.h"

#include "FsmSw_Dilithium2_polyvec.h"
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
static void FsmSw_Dilithium2_Polyvecl_Pointwise_Acc_Montgomery(poly_D2 *const w, const polyvecl_D2 *const u,
                                                               const polyvecl_D2 *const v);
/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/
/*====================================================================================================================*/
/**
* \brief Pointwise multiply vectors of polynomials of length L_DILITHIUM2, multiply resulting vector by 2^{-32}
*              and add (accumulate) polynomials in it. Input/output vectors are in NTT domain representation.
*
* \param[out] poly_D2           *w : output polynomial
* \param[in]  const polyvecl_D2 *u : pointer to first input vector
* \param[in]  const polyvecl_D2 *v : pointer to second input vector
*/
static void FsmSw_Dilithium2_Polyvecl_Pointwise_Acc_Montgomery(poly_D2 *const w, const polyvecl_D2 *const u,
                                                               const polyvecl_D2 *const v)
{
  uint8 i = 0;
  poly_D2 t;

  FsmSw_Dilithium2_Poly_PointwiseMontgomery(w, &u->vec[0], &v->vec[0]);
  for (i = 1; i < L_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_PointwiseMontgomery(&t, &u->vec[i], &v->vec[i]);
    FsmSw_Dilithium2_Poly_Add(w, w, &t);
  }
} // end: FsmSw_Dilithium2_Polyvecl_Pointwise_Acc_Montgomery
/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/*====================================================================================================================*/
/**
* \brief Implementation of ExpandA. Generates matrix mat with uniformly random coefficients a_{i,j}
*              by performing rejection sampling on the output stream of SHAKE128(rho|j|i)
*
* \param[out] polyvecl_D2        mat[K_DILITHIUM2] :        output matrix
* \param[in]  const uint8 rho[SEEDBYTES_DILITHIUM] : byte array containing seed rho
*/
void FsmSw_Dilithium2_Polyvec_MatrixExpand(polyvecl_D2 mat[K_DILITHIUM2], const uint8 rho[SEEDBYTES_DILITHIUM])
{
  uint8 i = 0;
  uint8 j = 0;

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    for (j = 0; j < L_DILITHIUM2; ++j)
    {
      FsmSw_Dilithium2_Poly_Uniform(&mat[i].vec[j], rho, (uint16)(((uint16)i << 8) + j));
    }
  }
} // end: FsmSw_Dilithium2_Polyvec_MatrixExpand
/*====================================================================================================================*/
/**
* \brief t.b.d
*
* \param[out] polyvecl_D2                       *t : t.b.d
* \param[in]  const polyvecl_D2  mat[K_DILITHIUM2] : t.b.d
* \param[in]  const polyvecl_D2                 *v : t.b.d
*/
/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_Dilithium2_Polyvec_MatrixPointwiseMontgomery(polyveck_D2 *t, const polyvecl_D2 mat[K_DILITHIUM2],
                                                        const polyvecl_D2 *const v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Polyvecl_Pointwise_Acc_Montgomery(&t->vec[i], &mat[i], v);
  }
} // end: FsmSw_Dilithium2_Polyvec_MatrixPointwiseMontgomery
/*====================================================================================================================*/
/**
* \brief t.b.d
*
* \param[out]  polyvecl_D2                       *v : t.b.d
* \param[in]   const uint8 seed[CRHBYTES_DILITHIUM] : t.b.d
* \param[in]   uint16                         nonce : t.b.d
*/
void FsmSw_Dilithium2_Polyvecl_UniformEta(polyvecl_D2 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce)
{
  uint8 i = 0;
  /* nonce_temp is used to avoid modifying the input. */
  uint16 nonce_temp = nonce;

  for (i = 0; i < L_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_UniformEta(&v->vec[i], seed, nonce_temp);
    nonce_temp++;
  }
} // end: FsmSw_Dilithium2_Polyvecl_UniformEta
/*====================================================================================================================*/
/**
* \brief t.b.d
*
* \param[out]  polyvecl_D2                       *v : t.b.d
* \param[in]   const uint8 seed[CRHBYTES_DILITHIUM] : t.b.d
* \param[in]   uint16                         nonce : t.b.d
*/
/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_Dilithium2_Polyvecl_UniformGamma1(polyvecl_D2 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce)
{
  uint8 i = 0;

  for (i = 0; i < L_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_UniformGamma1(&v->vec[i], seed, (uint16)((L_DILITHIUM2 * nonce) + i));
  }
} // end: FsmSw_Dilithium2_Polyvecl_UniformGamma1
/*====================================================================================================================*/
/**
* \brief t.b.d
*
* \param[out] polyvecl_D2 *v : t.b.d
*/
void FsmSw_Dilithium2_Polyvecl_Reduce(polyvecl_D2 *v)
{
  uint8 i = 0;

  for (i = 0; i < L_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_Reduce(&v->vec[i]);
  }
} // end: FsmSw_Dilithium2_Polyvecl_Reduce
/*====================================================================================================================*/
/**
* \brief Add vectors of polynomials of length L_DILITHIUM2. No modular reduction is performed.
*
* \param[in] polyvecl_D2        *w : pointer to output vector
* \param[in] const polyvecl_D2  *u : pointer to first summand
* \param[in] const polyvecl_D2  *v : pointer to second summand
*/
void FsmSw_Dilithium2_Polyvecl_Add(polyvecl_D2 *w, const polyvecl_D2 *const u, const polyvecl_D2 *const v)
{
  uint8 i = 0;

  for (i = 0; i < L_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_Add(&w->vec[i], &u->vec[i], &v->vec[i]);
  }
} // end: FsmSw_Dilithium2_Polyvecl_Add
/*====================================================================================================================*/
/**
* \brief Forward NTT of all polynomials in vector of length L_DILITHIUM2.
*              Output coefficients can be up to 16*Q larger than input coefficients.
*
* \param[in,out] polyvecl_D2 *v : pointer to input/output vector
*/
void FsmSw_Dilithium2_Polyvecl_Ntt(polyvecl_D2 *v)
{
  uint8 i = 0;

  for (i = 0; i < L_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_Ntt(&v->vec[i]);
  }
} // end: FsmSw_Dilithium2_Polyvecl_Ntt
/*====================================================================================================================*/
/**
* \brief t.b.d
*
* \param[in,out] polyvecl_D2 *v : pointer to input/output vector
*/
void FsmSw_Dilithium2_Polyvecl_InvnttTomont(polyvecl_D2 *v)
{
  uint8 i = 0;

  for (i = 0; i < L_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_InvnttTomont(&v->vec[i]);
  }
} // end: FsmSw_Dilithium2_Polyvecl_InvnttTomont
/*====================================================================================================================*/
/**
* \brief t.b.d
*
* \param[out] polyvecl_D2       *r : t.b.d
* \param[in]  const poly_D2     *a : t.b.d
* \param[in]  const polyvecl_D2 *v : t.b.d
*/
void FsmSw_Dilithium2_Polyvecl_PointwisePolyMontgomery(polyvecl_D2 *r, const poly_D2 *const a,
                                                       const polyvecl_D2 *const v)
{
  uint8 i = 0;

  for (i = 0; i < L_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_PointwiseMontgomery(&r->vec[i], a, &v->vec[i]);
  }
} // end: FsmSw_Dilithium2_Polyvecl_PointwisePolyMontgomery
/*====================================================================================================================*/
/**
* \brief Check infinity norm of polynomials in vector of length L_DILITHIUM2.
*              Assumes input polyvecl_D2 to be reduced by FsmSw_Dilithium2_Polyvecl_Reduce().
*
* \param[in] const polyvecl_D2 *v : pointer to vector
* \param[in] sint32         bound : norm bound
*
* \returns 0 if norm of all polynomials is strictly smaller than bound <= (Q-1)/8
* and 1 otherwise.
*/
sint8 FsmSw_Dilithium2_Polyvecl_Chknorm(const polyvecl_D2 *const v, sint32 bound)
{
  uint8 i      = 0;
  sint8 retVal = 0;

  for (i = 0; i < L_DILITHIUM2; ++i)
  {
    if (0 < FsmSw_Dilithium2_Poly_Chknorm(&v->vec[i], bound))
    {
      retVal = 1;
    }
  }

  return retVal;
} // end: FsmSw_Dilithium2_Polyvecl_Chknorm
/*====================================================================================================================*/
/**
* \brief t.b.d
*
* \param[in,out] polyvecl_D2                       *v : t.b.d
* \param[in]     const uint8 seed[CRHBYTES_DILITHIUM] : t.b.d
* \param[in]     uint16                         nonce : t.b.d
*/
void FsmSw_Dilithium2_Polyveck_UniformEta(polyveck_D2 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce)
{
  uint8 i = 0;
  /* nonce_temp is used to avoid modifying the input. */
  uint16 nonce_temp = nonce;

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_UniformEta(&v->vec[i], seed, nonce_temp);
    nonce_temp++;
  }
} // end: FsmSw_Dilithium2_Polyveck_UniformEta
/*====================================================================================================================*/
/**
* \brief Reduce coefficients of polynomials in vector of length K_DILITHIUM2 to representatives in
*              [-6283009,6283007].
*
* \param[in,out] polyveck_D2 *v : pointer to input/output vector
*/
void FsmSw_Dilithium2_Polyveck_Reduce(polyveck_D2 *v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_Reduce(&v->vec[i]);
  }
} // end: FsmSw_Dilithium2_Polyveck_Reduce
/*====================================================================================================================*/
/**
* \brief For all coefficients of polynomials in vector of length K_DILITHIUM2 add Q if coefficient is negative.
*
* \param[in,out] polyveck_D2 *v : pointer to input/output vector
*/
void FsmSw_Dilithium2_Polyveck_CAddQ(polyveck_D2 *v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_CAddQ(&v->vec[i]);
  }
} // end: FsmSw_Dilithium2_Polyveck_CAddQ
/*====================================================================================================================*/
/**
* \brief Add vectors of polynomials of length K_DILITHIUM2. No modular reduction is performed.
*
* \param[in] polyveck_D2       *w : pointer to output vector
* \param[in] const polyveck_D2 *u : pointer to first summand
* \param[in] const polyveck_D2 *v : pointer to second summand
*/
void FsmSw_Dilithium2_Polyveck_Add(polyveck_D2 *w, const polyveck_D2 *const u, const polyveck_D2 *const v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_Add(&w->vec[i], &u->vec[i], &v->vec[i]);
  }
} // end: FsmSw_Dilithium2_Polyveck_Add
/*====================================================================================================================*/
/**
* \brief Subtract vectors of polynomials of length K_DILITHIUM2. No modular reduction is performed.
*
*  \param[in] polyveck_D2       *w : pointer to output vector
*  \param[in] const polyveck_D2 *u : pointer to first input vector
*  \param[in] const polyveck_D2 *v : pointer to second input vector to be subtracted from first input vector
*/
void FsmSw_Dilithium2_Polyveck_Sub(polyveck_D2 *w, const polyveck_D2 *const u, const polyveck_D2 *const v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_Sub(&w->vec[i], &u->vec[i], &v->vec[i]);
  }
} // end: FsmSw_Dilithium2_Polyveck_Sub
/*====================================================================================================================*/
/**
* \brief Multiply vector of polynomials of Length K_DILITHIUM2 by 2^D without modular reduction.
*              Assumes input coefficients to be less than 2^{31-D}.
*
* \param[in,out] polyveck_D2 *v : pointer to input/output vector
*/
void FsmSw_Dilithium2_Polyveck_Shiftl(polyveck_D2 *v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_Shiftl(&v->vec[i]);
  }
} // end: FsmSw_Dilithium2_Polyveck_Shiftl
/*====================================================================================================================*/
/**
* \brief Forward NTT of all polynomials in vector of length K_DILITHIUM2.
*              Output coefficients can be up to 16*Q larger than input coefficients.
*
* \param[in,out] polyveck_D2 *v : pointer to input/output vector
*/
void FsmSw_Dilithium2_Polyveck_Ntt(polyveck_D2 *v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_Ntt(&v->vec[i]);
  }
} // end: FsmSw_Dilithium2_Polyveck_Ntt
/*====================================================================================================================*/
/**
* \brief Inverse NTT and multiplication by 2^{32} of polynomials in vector of length K_DILITHIUM2.
*              Input coefficients need to be less than 2*Q.
*
* \param[in,out] polyveck_D2 *v: pointer to input/output vector
*/
void FsmSw_Dilithium2_Polyveck_InvnttTomont(polyveck_D2 *v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_InvnttTomont(&v->vec[i]);
  }
} // end: FsmSw_Dilithium2_Polyveck_InvnttTomont
/*====================================================================================================================*/
/**
* \brief t.b.d
*
* \param[out] polyveck_D2       *r : t.b.d
* \param[in]  const poly_D2     *a : t.b.d
* \param[in]  const polyveck_D2 *v : t.b.d
*/
void FsmSw_Dilithium2_Polyveck_PointwisePolyMontgomery(polyveck_D2 *r, const poly_D2 *const a,
                                                       const polyveck_D2 *const v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_PointwiseMontgomery(&r->vec[i], a, &v->vec[i]);
  }
} // end: FsmSw_Dilithium2_Polyveck_PointwisePolyMontgomery
/*====================================================================================================================*/
/**
* \brief Check infinity norm of polynomials in vector of length K_DILITHIUM2.
*              Assumes input polyveck_D2to be reduced by FsmSw_Dilithium2_Polyveck_Reduce().
*
* \param[in] const polyveck_D2 *v : pointer to vector
* \param[in] sint32         bound : norm bound
*
* Returns 0 if norm of all polynomials are strictly smaller than bound <= (Q-1)/8 and 1 otherwise.
*/
sint8 FsmSw_Dilithium2_Polyveck_Chknorm(const polyveck_D2 *const v, sint32 bound)
{
  uint8 i      = 0;
  sint8 retVal = 0;

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    if (0 < FsmSw_Dilithium2_Poly_Chknorm(&v->vec[i], bound))
    {
      retVal = 1;
    }
  }

  return retVal;
} // end: FsmSw_Dilithium2_Polyveck_Chknorm
/*====================================================================================================================*/
/**
* \brief For all coefficients a of polynomials in vector of length K_DILITHIUM2, compute a0, a1 such that a
*              mod^+ Q = a1*2^D + a0 with -2^{D-1} < a0 <= 2^{D-1}. Assumes coefficients to be standard representatives.
*
* \param[out] polyveck_D2      *v1 : pointer to output vector of polynomials with coefficients a1
* \param[out] polyveck_D2      *v0 : pointer to output vector of polynomials with coefficients a0
* \param[in]  const polyveck_D2 *v : pointer to input vector
*/
void FsmSw_Dilithium2_Polyveck_Power2Round(polyveck_D2 *v1, polyveck_D2 *v0, const polyveck_D2 *const v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_Power2Round(&v1->vec[i], &v0->vec[i], &v->vec[i]);
  }
} // end: FsmSw_Dilithium2_Polyveck_Power2Round
/*====================================================================================================================*/
/**
* \brief For all coefficients a of polynomials in vector of length K_DILITHIUM2, compute high and low bits
*              a0, a1 such a mod^+ Q = a1*ALPHA + a0 with -ALPHA/2 < a0 <= ALPHA/2 except a1 = (Q-1)/ALPHA where we
*              set a1 = 0 and -ALPHA/2 <= a0 = a mod Q - Q < 0. Assumes coefficients to be standard representatives.
*
* \param[out]  polyveck_D2      *v1 : pointer to output vector of polynomials with coefficients a1
* \param[out]  polyveck_D2      *v0 : pointer to output vector of polynomials with coefficients a0
* \param[in]   const polyveck_D2 *v : pointer to input vector
*/
void FsmSw_Dilithium2_Polyveck_Decompose(polyveck_D2 *v1, polyveck_D2 *v0, const polyveck_D2 *const v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_Decompose(&v1->vec[i], &v0->vec[i], &v->vec[i]);
  }
} // end: FsmSw_Dilithium2_Polyveck_Decompose
/*====================================================================================================================*/
/**
* \brief Compute hint vector.
*
* \param[out] polyveck_D2        *h : pointer to output vector
* \param[in]  const polyveck_D2 *v0 : pointer to low part of input vector
* \param[in]  const polyveck_D2 *v1 : pointer to high part of input vector
*
* \returns number of 1 bits.
*/
uint32 FsmSw_Dilithium2_Polyveck_MakeHint(polyveck_D2 *h, const polyveck_D2 *const v0, const polyveck_D2 *const v1)
{
  uint8 i  = 0;
  uint32 s = 0;

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    s += FsmSw_Dilithium2_Poly_MakeHint(&h->vec[i], &v0->vec[i], &v1->vec[i]);
  }

  return s;
} // end: FsmSw_Dilithium2_Polyveck_MakeHint
/*====================================================================================================================*/
/**
* \brief Use hint vector to correct the high bits of input vector.
*
* \param[out] polyveck_D2        *w : pointer to output vector of polynomials with corrected high bits
* \param[in]  const polyveck_D2  *u : pointer to input vector
* \param[in]  const polyveck_D2  *h : pointer to input hint vector
*/
void FsmSw_Dilithium2_Polyveck_UseHint(polyveck_D2 *w, const polyveck_D2 *const u, const polyveck_D2 *const h)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_UseHint(&w->vec[i], &u->vec[i], &h->vec[i]);
  }
} // end: FsmSw_Dilithium2_Polyveck_UseHint
/*====================================================================================================================*/
/**
* \brief t.b.d
*
* \param[out] uint8 r[K_DILITHIUM2 * POLYW1_PACKEDBYTES_DILITHIUM2] : t.b.d
* \param[in]  const polyveck_D2 *w1 : t.b.d
*/
void FsmSw_Dilithium2_Polyveck_PackW1(uint8 r[K_DILITHIUM2 * POLYW1_PACKEDBYTES_DILITHIUM2],
                                      const polyveck_D2 *const w1)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_W1Pack(&r[i * POLYW1_PACKEDBYTES_DILITHIUM2], &w1->vec[i]);
  }
} // end: FsmSw_Dilithium2_Polyveck_PackW1

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */