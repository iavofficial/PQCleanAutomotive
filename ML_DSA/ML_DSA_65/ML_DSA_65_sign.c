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
/** \addtogroup ML_DSA_65
*    includes the modules for ML_DSA_65
 ** @{ */
/** \addtogroup ML_DSA_65_sign
 ** @{ */

/*====================================================================================================================*/
/** \file ML_DSA_65_sign.c
* \brief  description of ML_DSA_65_sign.c
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
#include "FsmSw_CommonLib.h"
#include "ML_DSA_65_packing.h"
#include "ML_DSA_65_params.h"
#include "ML_DSA_65_poly.h"
#include "ML_DSA_65_polyvec.h"
#include "ML_DSA_symmetric.h"
#include "FsmSw_Fips202.h"

#include "ML_DSA_65_sign.h"
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
* \brief Computes signature with context.
*
* \param[out] uint8     *sig    : pointer to output signature (of length FSMSW_ML_DSA_65_CRYPTO_BYTES)
* \param[out] uint32 *siglen    : pointer to output length of signature
* \param[in]  uint8       *m    : pointer to message to be signed
* \param[in]  uint32    mlen    : length of message
* \param[in]  const uint8 *ctx  : pointer to context string
* \param[in]  uint32    ctxlen  : length of context string
* \param[in]  uint8      *sk    : pointer to bit-packed secret key
* \returns ERR_OK if success ERR_NOT_OK otherwise (context string too long)
*/
/* polyspace +4 CERT-C:DCL19-C [Justified:]"see below" */
/* polyspace +3 CERT-C:DCL15-C [Justified:]"see below" */
/* polyspace +2 MISRA2012:8.7 [Justified:]"The interface is a needed to support optional context information. 
                                           The same interfaces is used in PQClean" */
uint8 ML_DSA_65_Crypto_Sign_Signature_Ctx(uint8 *const sig, uint32 *const siglen, const uint8 *const m,
                                                 uint32 mlen, const uint8 *ctx, uint32 ctxlen, const uint8 *const sk)
{
  uint32 n                                                                                                       = 0u;
  uint8 seedbuf[(2u * SEEDBYTES_ML_DSA) + TRBYTES_ML_DSA + RNDBYTES_ML_DSA + (2u * CRHBYTES_ML_DSA)] = {0u};
  uint8 *rho                    = (uint8 *)NULL_PTR;
  uint8 *tr                     = (uint8 *)NULL_PTR;
  uint8 *key                    = (uint8 *)NULL_PTR;
  uint8 *mu                     = (uint8 *)NULL_PTR;
  uint8 *rhoprime               = (uint8 *)NULL_PTR;
  uint8 *rnd                    = (uint8 *)NULL_PTR;
  uint16 nonce                  = 0u;
  polyvecl_D3 mat[K_ML_DSA_65] = {{{{{0}}}}};
  polyvecl_D3 s1                = {{{{0}}}};
  polyvecl_D3 y                 = {{{{0}}}};
  polyvecl_D3 z                 = {{{{0}}}};
  polyveck_D3 t0                = {{{{0}}}};
  polyveck_D3 s2                = {{{{0}}}};
  polyveck_D3 w1                = {{{{0}}}};
  polyveck_D3 w0                = {{{{0}}}};
  polyveck_D3 h                 = {{{{0}}}};
  poly_D3 cp                    = {{0}};
  shake256incctx state          = {{0u}};
  boolean loop                  = TRUE;
  uint8 retVal                  = ERR_OK;

  if (ctxlen > 255)
  {
    retVal = ERR_NOT_OK;
  }
  else
  {

    rho      = seedbuf;
    tr       = &rho[SEEDBYTES_ML_DSA];
    key      = &tr[TRBYTES_ML_DSA];
    rnd      = &key[SEEDBYTES_ML_DSA];
    mu       = &rnd[RNDBYTES_ML_DSA];
    rhoprime = &mu[CRHBYTES_ML_DSA];
    ML_DSA_65_UnpackSk(rho, tr, key, &t0, &s1, &s2, sk);

    /* Compute mu = CRH(tr, 0, ctxlen, ctx, msg) */
    mu[0] = 0;
    mu[1] = (uint8)ctxlen;
    FsmSw_Fips202_Shake256_IncInit(&state);
    FsmSw_Fips202_Shake256_IncAbsorb(&state, tr, TRBYTES_ML_DSA);
    FsmSw_Fips202_Shake256_IncAbsorb(&state, mu, 2);
    FsmSw_Fips202_Shake256_IncAbsorb(&state, ctx, ctxlen);
    FsmSw_Fips202_Shake256_IncAbsorb(&state, m, mlen);
    FsmSw_Fips202_Shake256_IncFinalize(&state);
    FsmSw_Fips202_Shake256_IncSqueeze(mu, CRHBYTES_ML_DSA, &state);

    for (n = 0u; n < RNDBYTES_ML_DSA; n++)
    {
      rnd[n] = 0u;
    }
    FsmSw_Fips202_Shake256(rhoprime, CRHBYTES_ML_DSA, key,
                           SEEDBYTES_ML_DSA + RNDBYTES_ML_DSA + CRHBYTES_ML_DSA);

    /* Expand matrix and transform vectors */
    ML_DSA_65_Polyvec_MatrixExpand(mat, rho);
    ML_DSA_65_Polyvecl_Ntt(&s1);
    ML_DSA_65_Polyveck_Ntt(&s2);
    ML_DSA_65_Polyveck_Ntt(&t0);

    while (TRUE == loop)
    {
      /* Sample intermediate vector y */
      ML_DSA_65_Polyvecl_UniformGamma1(&y, rhoprime, nonce);
      nonce++;

      /* Matrix-vector multiplication */
      z = y;
      ML_DSA_65_Polyvecl_Ntt(&z);
      ML_DSA_65_Polyvec_MatrixPointwiseMontgomery(&w1, mat, &z);
      ML_DSA_65_Polyveck_Reduce(&w1);
      ML_DSA_65_Polyveck_InvnttTomont(&w1);

      /* Decompose w and call the random oracle */
      ML_DSA_65_Polyveck_CAddQ(&w1);
      ML_DSA_65_Polyveck_Decompose(&w1, &w0, &w1);
      ML_DSA_65_Polyveck_PackW1(sig, &w1);

      FsmSw_Fips202_Shake256_IncInit(&state);
      FsmSw_Fips202_Shake256_IncAbsorb(&state, mu, CRHBYTES_ML_DSA);
      FsmSw_Fips202_Shake256_IncAbsorb(&state, sig, K_ML_DSA_65 * POLYW1_PACKEDBYTES_ML_DSA_65);
      FsmSw_Fips202_Shake256_IncFinalize(&state);
      FsmSw_Fips202_Shake256_IncSqueeze(sig, CTILDEBYTES_ML_DSA_65, &state);
      ML_DSA_65_Poly_Challenge(&cp, sig);
      ML_DSA_65_Poly_Ntt(&cp);

      /* Compute z, reject if it reveals secret */
      ML_DSA_65_Polyvecl_PointwisePolyMontgomery(&z, &cp, &s1);
      ML_DSA_65_Polyvecl_InvnttTomont(&z);
      ML_DSA_65_Polyvecl_Add(&z, &z, &y);
      ML_DSA_65_Polyvecl_Reduce(&z);
      if (0 < ML_DSA_65_Polyvecl_Chknorm(&z, (sint32)(GAMMA1_ML_DSA_65 - BETA_ML_DSA_65)))
      {
        continue;
      }

      /* Check that subtracting cs2 does not change high bits of w and low bits
           * do not reveal secret information */
      ML_DSA_65_Polyveck_PointwisePolyMontgomery(&h, &cp, &s2);
      ML_DSA_65_Polyveck_InvnttTomont(&h);
      ML_DSA_65_Polyveck_Sub(&w0, &w0, &h);
      ML_DSA_65_Polyveck_Reduce(&w0);
      if (0 < ML_DSA_65_Polyveck_Chknorm(&w0, (sint32)((uint32)((uint32)GAMMA2_ML_DSA_65 - BETA_ML_DSA_65))))
      {
        continue;
      }

      /* Compute hints for w1 */
      ML_DSA_65_Polyveck_PointwisePolyMontgomery(&h, &cp, &t0);
      ML_DSA_65_Polyveck_InvnttTomont(&h);
      ML_DSA_65_Polyveck_Reduce(&h);
      if (0 < ML_DSA_65_Polyveck_Chknorm(&h, (sint32)GAMMA2_ML_DSA_65))
      {
        continue;
      }

      ML_DSA_65_Polyveck_Add(&w0, &w0, &h);
      n = ML_DSA_65_Polyveck_MakeHint(&h, &w0, &w1);
      if (n > OMEGA_ML_DSA_65)
      {
        continue;
      }

      loop = FALSE;
    }

    /* Write signature */
    ML_DSA_65_PackSig(sig, sig, &z, &h);
    *siglen = FSMSW_ML_DSA_65_CRYPTO_BYTES;
  }
  return retVal;
}

/*====================================================================================================================*/
/**
* \brief Compute signed message with context.
*
* \param[out] uint8        *sm  : pointer to output signed message (allocated
*                                     array with FSMSW_ML_DSA_65_CRYPTO_BYTES + mlen bytes), can be equal to m
* \param[out] uint32   *smlen   : pointer to output length of signed message
* \param[in]  const  uint8 *m   : pointer to message to be signed
* \param[in]  uint32     mlen   : length of message
* \param[in]  const uint8 *ctx  : pointer to context string
* \param[in]  uint32    ctxlen  : length of context string
* \param[in]  const uint8 *sk   : pointer to bit-packed secret key
* \returns ERR_OK if success ERR_NOT_OK otherwise (context string too long)
*/
/* polyspace +4 CERT-C:DCL19-C [Justified:]"see below" */
/* polyspace +3 CERT-C:DCL15-C [Justified:]"see below" */
/* polyspace +2 MISRA2012:8.7 [Justified:]"The interface is a needed to support optional context information. 
                                           The same interfaces is used in PQClean" */
uint8 ML_DSA_65_Crypto_Sign_Ctx(uint8 *const sm, uint32 *const smlen, const uint8 *const m, uint32 mlen,
                                       const uint8 *ctx, uint32 ctxlen, const uint8 *const sk)
{
  uint32 i     = 0u;
  uint8 retVal = ERR_OK;

  for (i = 0u; i < mlen; ++i)
  {
    sm[FSMSW_ML_DSA_65_CRYPTO_BYTES + mlen - 1u - i] = m[mlen - 1u - i];
  }
  retVal =
      ML_DSA_65_Crypto_Sign_Signature_Ctx(sm, smlen, &sm[FSMSW_ML_DSA_65_CRYPTO_BYTES], mlen, ctx, ctxlen, sk);
  *smlen += mlen;
  return retVal;
}

/*====================================================================================================================*/
/**
* \brief Verifies signature with context.
*
* \param[in] uint8        *m    : pointer to input signature
* \param[in] uint32   siglen    : length of signature
* \param[in] const uint8  *m    : pointer to message
* \param[in] uint32     mlen    : length of message
* \param[in]  const uint8 *ctx  : pointer to context string
* \param[in]  uint32    ctxlen  : length of context string
* \param[in] const uint8 *pk    : pointer to bit-packed public key
* \returns ERR_OK if signature could be verified correctly and ERR_NOT_OK otherwise
*/
/* polyspace +4 CERT-C:DCL19-C [Justified:]"see below" */
/* polyspace +3 CERT-C:DCL15-C [Justified:]"see below" */
/* polyspace +2 MISRA2012:8.7 [Justified:]"The interface is a needed to support optional context information. 
                                           The same interfaces is used in PQClean" */
uint8 ML_DSA_65_Crypto_Sign_Verify_Ctx(const uint8 *const sig, uint32 siglen, const uint8 *const m, uint32 mlen,
                                              const uint8 *ctx, uint32 ctxlen, const uint8 *const pk)
{
  uint16 i                                                = 0u;
  uint8 buf[K_ML_DSA_65 * POLYW1_PACKEDBYTES_ML_DSA_65] = {0u};
  uint8 rho[SEEDBYTES_ML_DSA]                          = {0u};
  uint8 mu[CRHBYTES_ML_DSA]                            = {0u};
  uint8 c[CTILDEBYTES_ML_DSA_65]                         = {0u};
  uint8 c2[CTILDEBYTES_ML_DSA_65]                        = {0u};
  poly_D3 cp                                              = {{0}};
  polyvecl_D3 mat[K_ML_DSA_65]                           = {{{{{0}}}}};
  polyvecl_D3 z                                           = {{{{0}}}};
  polyveck_D3 t1                                          = {{{{0}}}};
  polyveck_D3 w1                                          = {{{{0}}}};
  polyveck_D3 h                                           = {{{{0}}}};
  shake256incctx state                                    = {{0u}};
  uint8 retVal                                            = ERR_OK;

  if ((ctxlen > 255) || (siglen != FSMSW_ML_DSA_65_CRYPTO_BYTES))
  {
    retVal = ERR_NOT_OK;
  }

  ML_DSA_65_UnpackPk(rho, &t1, pk);
  if (0 < ML_DSA_65_UnpackSig(c, &z, &h, sig))
  {
    retVal = ERR_NOT_OK;
  }
  if (0 < ML_DSA_65_Polyvecl_Chknorm(&z, (sint32)(GAMMA1_ML_DSA_65 - BETA_ML_DSA_65)))
  {
    retVal = ERR_NOT_OK;
  }

  /* Compute CRH(H(rho, t1), msg) */
  FsmSw_Fips202_Shake256(mu, TRBYTES_ML_DSA, pk, FSMSW_ML_DSA_65_CRYPTO_PUBLICKEYBYTES);
  FsmSw_Fips202_Shake256_IncInit(&state);
  FsmSw_Fips202_Shake256_IncAbsorb(&state, mu, TRBYTES_ML_DSA);
  mu[0] = 0;
  mu[1] = (uint8)ctxlen;
  FsmSw_Fips202_Shake256_IncAbsorb(&state, mu, 2);
  FsmSw_Fips202_Shake256_IncAbsorb(&state, ctx, ctxlen);
  FsmSw_Fips202_Shake256_IncAbsorb(&state, m, mlen);
  FsmSw_Fips202_Shake256_IncFinalize(&state);
  FsmSw_Fips202_Shake256_IncSqueeze(mu, CRHBYTES_ML_DSA, &state);

  /* Matrix-vector multiplication; compute Az - c2^dt1 */
  ML_DSA_65_Poly_Challenge(&cp, c);
  ML_DSA_65_Polyvec_MatrixExpand(mat, rho);

  ML_DSA_65_Polyvecl_Ntt(&z);
  ML_DSA_65_Polyvec_MatrixPointwiseMontgomery(&w1, mat, &z);

  ML_DSA_65_Poly_Ntt(&cp);
  ML_DSA_65_Polyveck_Shiftl(&t1);
  ML_DSA_65_Polyveck_Ntt(&t1);
  ML_DSA_65_Polyveck_PointwisePolyMontgomery(&t1, &cp, &t1);

  ML_DSA_65_Polyveck_Sub(&w1, &w1, &t1);
  ML_DSA_65_Polyveck_Reduce(&w1);
  ML_DSA_65_Polyveck_InvnttTomont(&w1);

  /* Reconstruct w1 */
  ML_DSA_65_Polyveck_CAddQ(&w1);
  ML_DSA_65_Polyveck_UseHint(&w1, &w1, &h);
  ML_DSA_65_Polyveck_PackW1(buf, &w1);

  /* Call random oracle and verify Challenge */
  FsmSw_Fips202_Shake256_IncInit(&state);
  FsmSw_Fips202_Shake256_IncAbsorb(&state, mu, CRHBYTES_ML_DSA);
  FsmSw_Fips202_Shake256_IncAbsorb(&state, buf, K_ML_DSA_65 * POLYW1_PACKEDBYTES_ML_DSA_65);
  FsmSw_Fips202_Shake256_IncFinalize(&state);
  FsmSw_Fips202_Shake256_IncSqueeze(c2, CTILDEBYTES_ML_DSA_65, &state);
  for (i = 0u; i < CTILDEBYTES_ML_DSA_65; ++i)
  {
    if (c[i] != c2[i])
    {
      retVal = ERR_NOT_OK;
    }
  }
  return retVal;
}
/*====================================================================================================================*/
/**
* \brief Verify signed message with context
*
* \param[out] uint8        *m   : pointer to output message (allocated array with smlen bytes), can be equal to sm
* \param[out] uint32    *mlen   : pointer to output length of message
* \param[in]  const uint8 *sm   : pointer to signed message
* \param[in]  uint32    smlen   : length of signed message 
* \param[in]  const uint8 *ctx  : pointer to context string
* \param[in]  uint32    ctxlen  : length of context string
* \param[in]  const uint8 *pk   : pointer to bit-packed public key
*
* \returns ERR_OK if signed message could be verified correctly and ERR_NOT_OK otherwise
*/
/* polyspace +4 CERT-C:DCL19-C [Justified:]"see below" */
/* polyspace +3 CERT-C:DCL15-C [Justified:]"see below" */
/* polyspace +2 MISRA2012:8.7 [Justified:]"The interface is a needed to support optional context information. 
                                           The same interfaces is used in PQClean" */
uint8 ML_DSA_65_Crypto_Sign_Open_Ctx(uint8 *const m, uint32 *const mlen, const uint8 *const sm, uint32 smlen,
                                            const uint8 *ctx, uint32 ctxlen, const uint8 *const pk)
{
  uint32 i     = 0u;
  uint8 retVal = ERR_NOT_OK;

  if (smlen >= FSMSW_ML_DSA_65_CRYPTO_BYTES)
  {
    *mlen = smlen - FSMSW_ML_DSA_65_CRYPTO_BYTES;
    if (0u == ML_DSA_65_Crypto_Sign_Verify_Ctx(sm, FSMSW_ML_DSA_65_CRYPTO_BYTES,
                                                      &sm[FSMSW_ML_DSA_65_CRYPTO_BYTES], *mlen, ctx, ctxlen, pk))
    {
      /* All good, copy msg, return 0u */
      for (i = 0u; i < *mlen; ++i)
      {
        m[i] = sm[FSMSW_ML_DSA_65_CRYPTO_BYTES + i];
      }
      retVal = ERR_OK;
    }
  }

  if (ERR_OK != retVal)
  {
    /* Signature verification failed */
    *mlen = UINT32_MAXVAL;
    for (i = 0u; i < smlen; ++i)
    {
      m[i] = 0u;
    }
  }

  return retVal;
}
/*====================================================================================================================*/
/**
* \brief Generates public and private key.
*
* \param[out] uint8 *pk : pointer to output public key
*                           (allocated array of FSMSW_ML_DSA_65_CRYPTO_PUBLICKEYBYTES bytes)
* \param[out] uint8 *sk : pointer to output private key (allocated
*                           array of FSMSW_ML_DSA_65_CRYPTO_SECRETKEYBYTES bytes)
*/
void ML_DSA_65_Crypto_Sign_KeyPair(uint8 *const pk, uint8 *const sk)
{
  uint8 seedbuf[(2u * SEEDBYTES_ML_DSA) + CRHBYTES_ML_DSA] = {0u};
  uint8 tr[TRBYTES_ML_DSA]                                    = {0u};
  const uint8 *rho                                               = (uint8 *)NULL_PTR;
  const uint8 *rhoprime                                          = (uint8 *)NULL_PTR;
  const uint8 *key                                               = (uint8 *)NULL_PTR;
  polyvecl_D3 mat[K_ML_DSA_65]                                  = {{{{{0}}}}};
  polyvecl_D3 s1                                                 = {{{{0}}}};
  polyvecl_D3 s1hat                                              = {{{{0}}}};
  polyveck_D3 s2                                                 = {{{{0}}}};
  polyveck_D3 t1                                                 = {{{{0}}}};
  polyveck_D3 t0                                                 = {{{{0}}}};

  /* Get randomness for rho, rhoprime and key */
  (void)FsmSw_CommonLib_RandomBytes(seedbuf, SEEDBYTES_ML_DSA);
  seedbuf[SEEDBYTES_ML_DSA]     = K_ML_DSA_65;
  seedbuf[SEEDBYTES_ML_DSA + 1] = L_ML_DSA_65;
  FsmSw_Fips202_Shake256(seedbuf, (2u * SEEDBYTES_ML_DSA) + CRHBYTES_ML_DSA, seedbuf, SEEDBYTES_ML_DSA + 2);
  rho      = seedbuf;
  rhoprime = &rho[SEEDBYTES_ML_DSA];
  key      = &rhoprime[CRHBYTES_ML_DSA];

  /* Expand matrix */
  ML_DSA_65_Polyvec_MatrixExpand(mat, rho);

  /* Sample short vectors s1 and s2 */
  ML_DSA_65_Polyvecl_UniformEta(&s1, rhoprime, 0u);
  ML_DSA_65_Polyveck_UniformEta(&s2, rhoprime, L_ML_DSA_65);

  /* Matrix-vector multiplication */
  s1hat = s1;
  ML_DSA_65_Polyvecl_Ntt(&s1hat);
  ML_DSA_65_Polyvec_MatrixPointwiseMontgomery(&t1, mat, &s1hat);
  ML_DSA_65_Polyveck_Reduce(&t1);
  ML_DSA_65_Polyveck_InvnttTomont(&t1);

  /* Add error vector s2 */
  ML_DSA_65_Polyveck_Add(&t1, &t1, &s2);

  /* Extract t1 and write public key */
  ML_DSA_65_Polyveck_CAddQ(&t1);
  ML_DSA_65_Polyveck_Power2Round(&t1, &t0, &t1);
  ML_DSA_65_PackPk(pk, rho, &t1);

  /* Compute H(rho, t1) and write secret key */
  FsmSw_Fips202_Shake256(tr, TRBYTES_ML_DSA, pk, FSMSW_ML_DSA_65_CRYPTO_PUBLICKEYBYTES);
  ML_DSA_65_PackSk(sk, rho, tr, key, &t0, &s1, &s2);

  return;
} // end: ML_DSA_65_Crypto_Sign_KeyPair
/*====================================================================================================================*/
/**
* \brief Computes signature.
*
* \param[out] uint8     *sig : pointer to output signature (of length FSMSW_ML_DSA_65_CRYPTO_BYTES)
* \param[out] uint32 *siglen : pointer to output length of signature
* \param[in]  uint8       *m : pointer to message to be signed
* \param[in]  uint32    mlen : length of message
* \param[in]  uint8      *sk : pointer to bit-packed secret key
* \returns ERR_OK if success ERR_NOT_OK otherwise (context string too long)
*/
/* polyspace +4 ISO-17961:funcdecl [Justified:]"see MISRA2012:5.1" */
/* polyspace +3 CERT-C:DCL23-C [Justified:]"see MISRA2012:5.1" */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The interface is a comfort function without context. 
                                           The same interfaces is used in PQClean" */
uint8 ML_DSA_65_Crypto_Sign_Signature(uint8 *const sig, uint32 *const siglen, const uint8 *const m, uint32 mlen,
                                             const uint8 *const sk)
{
  return ML_DSA_65_Crypto_Sign_Signature_Ctx(sig, siglen, m, mlen, NULL_PTR, 0, sk);
} // end: ML_DSA_65_Crypto_Sign_Signature
/*====================================================================================================================*/
/**
* \brief Compute signed message.
*
* \param[out] uint8        *sm : pointer to output signed message (allocated
*                                     array with FSMSW_ML_DSA_65_CRYPTO_BYTES + mlen bytes), can be equal to m
* \param[out] uint32   *smlen : pointer to output length of signed message
* \param[in]  const  uint8 *m : pointer to message to be signed
* \param[in]  uint32     mlen : length of message
* \param[in]  const uint8 *sk : pointer to bit-packed secret key
* \returns ERR_OK if success ERR_NOT_OK otherwise (context string too long)
*/
uint8 ML_DSA_65_Crypto_Sign(uint8 *const sm, uint32 *const smlen, const uint8 *const m, uint32 mlen,
                                   const uint8 *const sk)
{
  return ML_DSA_65_Crypto_Sign_Ctx(sm, smlen, m, mlen, NULL_PTR, 0, sk);
} // end: ML_DSA_65_Crypto_Sign
/*====================================================================================================================*/
/**
* \brief Verifies signature.
*
* \param[in] uint8        *m : pointer to input signature
* \param[in] uint32   siglen : length of signature
* \param[in] const uint8  *m : pointer to message
* \param[in] uint32     mlen : length of message
* \param[in] const uint8 *pk : pointer to bit-packed public key
* \returns ERR_OK if signature could be verified correctly and ERR_NOT_OK otherwise
*/
uint8 ML_DSA_65_Crypto_Sign_Verify(const uint8 *const sig, uint32 siglen, const uint8 *const m, uint32 mlen,
                                          const uint8 *const pk)
{
  return ML_DSA_65_Crypto_Sign_Verify_Ctx(sig, siglen, m, mlen, NULL_PTR, 0, pk);
} // end: ML_DSA_65_Crypto_Sign_Verify
/*====================================================================================================================*/
/**
* \brief Verify signed message.
*
* \param[out] uint8        *m : pointer to output message (allocated array with smlen bytes), can be equal to sm
* \param[out] uint32    *mlen : pointer to output length of message
* \param[in]  const uint8 *sm : pointer to signed message
* \param[in]  uint32    smlen : length of signed message
* \param[in]  const uint8 *pk : pointer to bit-packed public key
*
* \returns ERR_OK if signed message could be verified correctly and ERR_NOT_OK otherwise
*/
uint8 ML_DSA_65_Crypto_Sign_Open(uint8 *const m, uint32 *const mlen, const uint8 *const sm, uint32 smlen,
                                        const uint8 *const pk)
{
  return ML_DSA_65_Crypto_Sign_Open_Ctx(m, mlen, sm, smlen, NULL_PTR, 0, pk);
} // end: ML_DSA_65_Crypto_Sign_Open

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
