/***********************************************************************************************************************
*
*                                          IAV GmbH
*
***********************************************************************************************************************/
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
#include "FsmSw_Dilithium3_poly.h"
#include "FsmSw_Types.h"

#include "FsmSw_Dilithium3_polyvec.h"
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
static void FsmSw_Dilithium3_Polyvecl_Pointwise_Acc_Montgomery(poly_D3 *w, const polyvecl_D3 *u, const polyvecl_D3 *v);
/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyvecl_Pointwise_Acc_Montgomery
*
* Description: Pointwise multiply vectors of polynomials of length L_DILITHIUM3, multiply resulting vector by 2^{-32}
*              and add (accumulate) polynomials in it. Input/output vectors are in NTT domain representation.
*
* Arguments:   -       poly_D3     *w: output polynomial
*              - const polyvecl_D3 *u: pointer to first input vector
*              - const polyvecl_D3 *v: pointer to second input vector
***********************************************************************************************************************/
static void FsmSw_Dilithium3_Polyvecl_Pointwise_Acc_Montgomery(poly_D3 *w, const polyvecl_D3 *u, const polyvecl_D3 *v)
{
  uint8 i   = 0;
  poly_D3 t = {{0}};

  FsmSw_Dilithium3_Poly_PointwiseMontgomery(w, &u->vec[0], &v->vec[0]);
  for (i = 1; i < L_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Poly_PointwiseMontgomery(&t, &u->vec[i], &v->vec[i]);
    FsmSw_Dilithium3_Poly_Add(w, w, &t);
  }
}
/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyvec_MatrixExpand
*
* Description: Implementation of ExpandA. Generates matrix mat with uniformly random coefficients a_{i,j}
*              by performing rejection sampling on the output stream of SHAKE128(rho|j|i)
*
* Arguments:   -       polyvecl_D3 mat[K_DILITHIUM3]:        output matrix
*              - const uint8       rho[SEEDBYTES_DILITHIUM]: byte array containing seed rho
***********************************************************************************************************************/
void FsmSw_Dilithium3_Polyvec_MatrixExpand(polyvecl_D3 mat[K_DILITHIUM3], const uint8 rho[SEEDBYTES_DILITHIUM])
{
  uint8 i = 0;
  uint8 j = 0;

  for (i = 0; i < K_DILITHIUM3; ++i)
  {
    for (j = 0; j < L_DILITHIUM3; ++j)
    {
      FsmSw_Dilithium3_Poly_Uniform(&mat[i].vec[j], rho, (uint16)(((uint16)i << 8) + j));
    }
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyvec_MatrixPointwiseMontgomery
*
* Description: t.b.d
*
* Arguments:   -       polyvecl_D3 *t:                 t.b.d
*              - const polyvecl_D3  mat[K_DILITHIUM3]: t.b.d
*              - const polyvecl_D3 *v:                 t.b.d
***********************************************************************************************************************/
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_Dilithium3_Polyvec_MatrixPointwiseMontgomery(polyveck_D3 *t, const polyvecl_D3 mat[K_DILITHIUM3],
                                                        const polyvecl_D3 *v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Polyvecl_Pointwise_Acc_Montgomery(&t->vec[i], &mat[i], v);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyvecl_Uniform_Eta
*
* Description: t.b.d
*
* Arguments:   -       polyvecl_D3 *v:                        t.b.d
*              - const uint8        seed[CRHBYTES_DILITHIUM]: t.b.d
*              -       uint16       nonce:                    t.b.d
***********************************************************************************************************************/
void FsmSw_Dilithium3_Polyvecl_Uniform_Eta(polyvecl_D3 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce)
{
  uint8 i = 0;
  /* nonce_temp is used to avoid modifying the input. */
  uint16 nonce_temp = nonce;

  for (i = 0; i < L_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Poly_UniformEta(&v->vec[i], seed, nonce_temp);
    nonce_temp++;
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyvecl_UniformGamma1
*
* Description: t.b.d
*
* Arguments:   -       polyvecl_D3 *v:                        t.b.d
*              - const uint8        seed[CRHBYTES_DILITHIUM]: t.b.d
*              -       uint16       nonce:                    t.b.d
***********************************************************************************************************************/
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_Dilithium3_Polyvecl_UniformGamma1(polyvecl_D3 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce)
{
  uint8 i = 0;

  for (i = 0; i < L_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Poly_UniformGamma1(&v->vec[i], seed, (uint16)((L_DILITHIUM3 * nonce) + i));
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyvecl_Reduce
*
* Description: t.b.d
*
* Arguments:   -       polyvecl_D3 *v: t.b.d
***********************************************************************************************************************/
void FsmSw_Dilithium3_Polyvecl_Reduce(polyvecl_D3 *v)
{
  uint8 i = 0;

  for (i = 0; i < L_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Poly_Reduce(&v->vec[i]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyvecl_Add
*
* Description: Add vectors of polynomials of length L_DILITHIUM3. No modular reduction is performed.
*
* Arguments:   -       polyvecl_D3 *w: pointer to output vector
*              - const polyvecl_D3 *u: pointer to first summand
*              - const polyvecl_D3 *v: pointer to second summand
***********************************************************************************************************************/
void FsmSw_Dilithium3_Polyvecl_Add(polyvecl_D3 *w, const polyvecl_D3 *u, const polyvecl_D3 *v)
{
  uint8 i = 0;

  for (i = 0; i < L_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Poly_Add(&w->vec[i], &u->vec[i], &v->vec[i]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyvecl_Ntt
*
* Description: Forward NTT of all polynomials in vector of length L_DILITHIUM3.
*              Output coefficients can be up to 16*Q larger than input coefficients.
*
* Arguments:   - polyvecl_D3 *v: pointer to input/output vector
***********************************************************************************************************************/
void FsmSw_Dilithium3_Polyvecl_Ntt(polyvecl_D3 *v)
{
  uint8 i = 0;

  for (i = 0; i < L_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Poly_Ntt(&v->vec[i]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyvecl_InvnttTomont
*
* Description: t.b.d
*
* Arguments:   - polyvecl_D3 *v: pointer to input/output vector
***********************************************************************************************************************/
void FsmSw_Dilithium3_Polyvecl_InvnttTomont(polyvecl_D3 *v)
{
  uint8 i = 0;

  for (i = 0; i < L_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Poly_InvnttTomont(&v->vec[i]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyvecl_PointwisePolyMontgomery
*
* Description: t.b.d
*
* Arguments:   -       polyvecl_D3 *r: t.b.d
*              - const poly_D3     *a: t.b.d
*              - const polyvecl_D3 *v: t.b.d
***********************************************************************************************************************/
void FsmSw_Dilithium3_Polyvecl_PointwisePolyMontgomery(polyvecl_D3 *r, const poly_D3 *a, const polyvecl_D3 *v)
{
  uint8 i = 0;

  for (i = 0; i < L_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Poly_PointwiseMontgomery(&r->vec[i], a, &v->vec[i]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyvecl_Chknorm
*
* Description: Check infinity norm of polynomials in vector of length L_DILITHIUM3.
*              Assumes input polyvecl_D3 to be reduced by FsmSw_Dilithium3_Polyvecl_Reduce().
*
* Arguments:   - const polyvecl_D3 *v:      pointer to vector
*              -       sint32        bound: norm bound
*
* Returns 0 if norm of all polynomials is strictly smaller than bound <= (Q-1)/8
* and 1 otherwise.
***********************************************************************************************************************/
sint8 FsmSw_Dilithium3_Polyvecl_Chknorm(const polyvecl_D3 *v, sint32 bound)
{
  uint8 i      = 0;
  sint8 retVal = 0;

  for (i = 0; i < L_DILITHIUM3; ++i)
  {
    if (0 < FsmSw_Dilithium3_Poly_Chknorm(&v->vec[i], bound))
    {
      retVal = 1;
    }
  }

  return retVal;
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyveck_UniformEta
*
* Description: t.b.d
*
* Arguments:   -       polyvecl_D3 *v:                        t.b.d
*              - const uint8        seed[CRHBYTES_DILITHIUM]: t.b.d
*              -       uint16       nonce:                    t.b.d
***********************************************************************************************************************/
void FsmSw_Dilithium3_Polyveck_UniformEta(polyveck_D3 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce)
{
  uint8 i = 0;
  /* nonce_temp is used to avoid modifying the input. */
  uint16 nonce_temp = nonce;

  for (i = 0; i < K_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Poly_UniformEta(&v->vec[i], seed, nonce_temp);
    nonce_temp++;
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyveck_Reduce
*
* Description: Reduce coefficients of polynomials in vector of length K_DILITHIUM3 to representatives in
*              [-6283009,6283007].
*
* Arguments:   - polyveck_D3 *v: pointer to input/output vector
***********************************************************************************************************************/
void FsmSw_Dilithium3_Polyveck_Reduce(polyveck_D3 *v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Poly_Reduce(&v->vec[i]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyveck_CAddQ
*
* Description: For all coefficients of polynomials in vector of length K_DILITHIUM3 add Q if coefficient is negative.
*
* Arguments:   - polyveck_D3 *v: pointer to input/output vector
***********************************************************************************************************************/
void FsmSw_Dilithium3_Polyveck_CAddQ(polyveck_D3 *v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Poly_CAddQ(&v->vec[i]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyveck_Add
*
* Description: Add vectors of polynomials of length K_DILITHIUM3. No modular reduction is performed.
*
* Arguments:   - polyveck_D3       *w: pointer to output vector
*              - const polyveck_D3 *u: pointer to first summand
*              - const polyveck_D3 *v: pointer to second summand
***********************************************************************************************************************/
void FsmSw_Dilithium3_Polyveck_Add(polyveck_D3 *w, const polyveck_D3 *u, const polyveck_D3 *v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Poly_Add(&w->vec[i], &u->vec[i], &v->vec[i]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyveck_Sub
*
* Description: Subtract vectors of polynomials of length K_DILITHIUM3. No modular reduction is performed.
*
* Arguments:   -       polyveck_D3 *w: pointer to output vector
*              - const polyveck_D3 *u: pointer to first input vector
*              - const polyveck_D3 *v: pointer to second input vector to be subtracted from first input vector
***********************************************************************************************************************/
void FsmSw_Dilithium3_Polyveck_Sub(polyveck_D3 *w, const polyveck_D3 *u, const polyveck_D3 *v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Poly_Sub(&w->vec[i], &u->vec[i], &v->vec[i]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyveck_Shiftl
*
* Description: Multiply vector of polynomials of Length K_DILITHIUM3 by 2^D without modular reduction.
*              Assumes input coefficients to be less than 2^{31-D}.
*
* Arguments:   - polyveck_D3 *v: pointer to input/output vector
***********************************************************************************************************************/
void FsmSw_Dilithium3_Polyveck_Shiftl(polyveck_D3 *v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Poly_Shiftl(&v->vec[i]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyveck_Ntt
*
* Description: Forward NTT of all polynomials in vector of length K_DILITHIUM3.
*              Output coefficients can be up to 16*Q larger than input coefficients.
*
* Arguments:   - polyveck_D3 *v: pointer to input/output vector
***********************************************************************************************************************/
void FsmSw_Dilithium3_Polyveck_Ntt(polyveck_D3 *v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Poly_Ntt(&v->vec[i]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyveck_InvnttTomont
*
* Description: Inverse NTT and multiplication by 2^{32} of polynomials in vector of length K_DILITHIUM3.
*              Input coefficients need to be less than 2*Q.
*
* Arguments:   - polyveck_D3 *v: pointer to input/output vector
***********************************************************************************************************************/
void FsmSw_Dilithium3_Polyveck_InvnttTomont(polyveck_D3 *v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Poly_InvnttTomont(&v->vec[i]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyveck_PointwisePolyMontgomery
*
* Description: t.b.d
*
* Arguments:   -       polyveck_D3 *r: t.b.d
*              - const poly_D3     *a: t.b.d
*              - const polyveck_D3 *v: t.b.d
***********************************************************************************************************************/
void FsmSw_Dilithium3_Polyveck_PointwisePolyMontgomery(polyveck_D3 *r, const poly_D3 *a, const polyveck_D3 *v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Poly_PointwiseMontgomery(&r->vec[i], a, &v->vec[i]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyveck_Chknorm
*
* Description: Check infinity norm of polynomials in vector of length K_DILITHIUM3.
*              Assumes input polyveck_D3to be reduced by FsmSw_Dilithium3_Polyveck_Reduce().
*
* Arguments:   - const polyveck_D3 *v:     pointer to vector
*              -       sint32       bound: norm bound
*
* Returns 0 if norm of all polynomials are strictly smaller than B <= (Q-1)/8 and 1 otherwise.
***********************************************************************************************************************/
sint8 FsmSw_Dilithium3_Polyveck_Chknorm(const polyveck_D3 *v, sint32 bound)
{
  uint8 i      = 0;
  sint8 retVal = 0;

  for (i = 0; i < K_DILITHIUM3; ++i)
  {
    if (0 < FsmSw_Dilithium3_Poly_Chknorm(&v->vec[i], bound))
    {
      retVal = 1;
    }
  }

  return retVal;
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyveck_Power2Round
*
* Description: For all coefficients a of polynomials in vector of length K_DILITHIUM3, compute a0, a1 such that a
*              mod^+ Q = a1*2^D + a0 with -2^{D-1} < a0 <= 2^{D-1}. Assumes coefficients to be standard representatives.
*
* Arguments:   -       polyveck_D3 *v1: pointer to output vector of polynomials with coefficients a1
*              -       polyveck_D3 *v0: pointer to output vector of polynomials with coefficients a0
*              - const polyveck_D3 *v:  pointer to input vector
***********************************************************************************************************************/
void FsmSw_Dilithium3_Polyveck_Power2Round(polyveck_D3 *v1, polyveck_D3 *v0, const polyveck_D3 *v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Poly_Power2Round(&v1->vec[i], &v0->vec[i], &v->vec[i]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyveck_Decompose
*
* Description: For all coefficients a of polynomials in vector of length K_DILITHIUM3, compute high and low bits
*              a0, a1 such a mod^+ Q = a1*ALPHA + a0 with -ALPHA/2 < a0 <= ALPHA/2 except a1 = (Q-1)/ALPHA where we
*              set a1 = 0 and -ALPHA/2 <= a0 = a mod Q - Q < 0. Assumes coefficients to be standard representatives.
*
* Arguments:   -       polyveck_D3 *v1: pointer to output vector of polynomials with coefficients a1
*              -       polyveck_D3 *v0: pointer to output vector of polynomials with coefficients a0
*              - const polyveck_D3 *v:  pointer to input vector
***********************************************************************************************************************/
void FsmSw_Dilithium3_Polyveck_Decompose(polyveck_D3 *v1, polyveck_D3 *v0, const polyveck_D3 *v)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Poly_Decompose(&v1->vec[i], &v0->vec[i], &v->vec[i]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyveck_MakeHint
*
* Description: Compute hint vector.
*
* Arguments:   -       polyveck_D3 *h:  pointer to output vector
*              - const polyveck_D3 *v0: pointer to low part of input vector
*              - const polyveck_D3 *v1: pointer to high part of input vector
*
* Returns number of 1 bits.
***********************************************************************************************************************/
uint32 FsmSw_Dilithium3_Polyveck_MakeHint(polyveck_D3 *h, const polyveck_D3 *v0, const polyveck_D3 *v1)
{
  uint8 i  = 0;
  uint32 s = 0;

  for (i = 0; i < K_DILITHIUM3; ++i)
  {
    s += FsmSw_Dilithium3_Poly_MakeHint(&h->vec[i], &v0->vec[i], &v1->vec[i]);
  }

  return s;
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyveck_UseHint
*
* Description: Use hint vector to correct the high bits of input vector.
*
* Arguments:   -       polyveck_D3 *w: pointer to output vector of polynomials with corrected high bits
*              - const polyveck_D3 *u: pointer to input vector
*              - const polyveck_D3 *h: pointer to input hint vector
***********************************************************************************************************************/
void FsmSw_Dilithium3_Polyveck_UseHint(polyveck_D3 *w, const polyveck_D3 *u, const polyveck_D3 *h)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Poly_UseHint(&w->vec[i], &u->vec[i], &h->vec[i]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium3_Polyveck_PackW1
*
* Description: t.b.d
*
* Arguments:   -       uint8        r[K_DILITHIUM3 * POLYW1_PACKEDBYTES_DILITHIUM3]: t.b.d
*              - const polyveck_D3 *w1:                                              t.b.d
***********************************************************************************************************************/
void FsmSw_Dilithium3_Polyveck_PackW1(uint8 r[K_DILITHIUM3 * POLYW1_PACKEDBYTES_DILITHIUM3], const polyveck_D3 *w1)
{
  uint8 i = 0;

  for (i = 0; i < K_DILITHIUM3; ++i)
  {
    FsmSw_Dilithium3_Poly_W1Pack(&r[i * POLYW1_PACKEDBYTES_DILITHIUM3], &w1->vec[i]);
  }
}
