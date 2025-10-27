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
/** \addtogroup FsmSw_Dilithium2_sign
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Dilithium_sign.c
* \brief  description of FsmSw_Dilithium_sign.c
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
#include "FsmSw_Dilithium2_packing.h"
#include "FsmSw_Dilithium2_params.h"
#include "FsmSw_Dilithium2_poly.h"
#include "FsmSw_Dilithium2_polyvec.h"
#include "FsmSw_Dilithium_symmetric.h"
#include "FsmSw_Fips202.h"

#include "FsmSw_Dilithium2_sign.h"
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
* \brief Generates public and private key.
*
* \param[out] uint8 *pk : pointer to output public key
*                           (allocated array of FSMSW_DILITHIUM2_CRYPTO_PUBLICKEYBYTES bytes)
* \param[out] uint8 *sk : pointer to output private key (allocated
*                           array of FSMSW_DILITHIUM2_CRYPTO_SECRETKEYBYTES bytes)
*/
void FsmSw_Dilithium2_Crypto_Sign_KeyPair(uint8 *pk, uint8 *sk)
{
  uint8 seedbuf[(2u * SEEDBYTES_DILITHIUM) + CRHBYTES_DILITHIUM] = {0u};
  uint8 tr[TRBYTES_DILITHIUM]                                    = {0u};
  const uint8 *rho                                               = (uint8 *)NULL_PTR;
  const uint8 *rhoprime                                          = (uint8 *)NULL_PTR;
  const uint8 *key                                               = (uint8 *)NULL_PTR;
  polyvecl_D2 mat[K_DILITHIUM2]                                  = {{{{{0}}}}};
  polyvecl_D2 s1                                                 = {{{{0}}}};
  polyvecl_D2 s1hat                                              = {{{{0}}}};
  polyveck_D2 s2                                                 = {{{{0}}}};
  polyveck_D2 t1                                                 = {{{{0}}}};
  polyveck_D2 t0                                                 = {{{{0}}}};

  /* Get randomness for rho, rhoprime and key */
  (void)FsmSw_CommonLib_RandomBytes(seedbuf, SEEDBYTES_DILITHIUM);
  FsmSw_Fips202_Shake256(seedbuf, (2u * SEEDBYTES_DILITHIUM) + CRHBYTES_DILITHIUM, seedbuf, SEEDBYTES_DILITHIUM);
  rho      = seedbuf;
  rhoprime = &rho[SEEDBYTES_DILITHIUM];
  key      = &rhoprime[CRHBYTES_DILITHIUM];

  /* Expand matrix */
  FsmSw_Dilithium2_Polyvec_MatrixExpand(mat, rho);

  /* Sample short vectors s1 and s2 */
  FsmSw_Dilithium2_Polyvecl_UniformEta(&s1, rhoprime, 0u);
  FsmSw_Dilithium2_Polyveck_UniformEta(&s2, rhoprime, L_DILITHIUM2);

  /* Matrix-vector multiplication */
  s1hat = s1;
  FsmSw_Dilithium2_Polyvecl_Ntt(&s1hat);
  FsmSw_Dilithium2_Polyvec_MatrixPointwiseMontgomery(&t1, mat, &s1hat);
  FsmSw_Dilithium2_Polyveck_Reduce(&t1);
  FsmSw_Dilithium2_Polyveck_InvnttTomont(&t1);

  /* Add error vector s2 */
  FsmSw_Dilithium2_Polyveck_Add(&t1, &t1, &s2);

  /* Extract t1 and write public key */
  FsmSw_Dilithium2_Polyveck_CAddQ(&t1);
  FsmSw_Dilithium2_Polyveck_Power2Round(&t1, &t0, &t1);
  FsmSw_Dilithium2_Pack_Pk(pk, rho, &t1);

  /* Compute H(rho, t1) and write secret key */
  FsmSw_Fips202_Shake256(tr, TRBYTES_DILITHIUM, pk, FSMSW_DILITHIUM2_CRYPTO_PUBLICKEYBYTES);
  FsmSw_Dilithium2_Pack_Sk(sk, rho, tr, key, &t0, &s1, &s2);

  return;
} // end: FsmSw_Dilithium2_Crypto_Sign_KeyPair
/*====================================================================================================================*/
/**
* \brief Computes signature.
*
* \param[out] uint8     *sig : pointer to output signature (of length FSMSW_DILITHIUM2_CRYPTO_BYTES)
* \param[out] uint32 *siglen : pointer to output length of signature
* \param[in]  uint8       *m : pointer to message to be signed
* \param[in]  uint32    mlen : length of message
* \param[in]  uint8      *sk : pointer to bit-packed secret key
*/
/* polyspace +6 CERT-C:DCL15-C [Justified:]"This is an interface function 
designed for use by other systems that aim to integrate the Dilithium." */
/* polyspace +4 CERT-C:DCL19-C [Justified:]"This is an interface function 
designed for use by other systems that aim to integrate the Dilithium." */
/* polyspace +2 MISRA2012:8.7 [Justified:]"This is an interface function 
designed for use by other systems that aim to integrate the Dilithium." */
void FsmSw_Dilithium2_Crypto_Sign_Signature(uint8 *sig, uint32 *siglen, const uint8 *m, uint32 mlen, const uint8 *sk)
{
  uint32 n                                                                                                       = 0u;
  uint8 seedbuf[(2u * SEEDBYTES_DILITHIUM) + TRBYTES_DILITHIUM + RNDBYTES_DILITHIUM + (2u * CRHBYTES_DILITHIUM)] = {0u};
  uint8 *rho                    = (uint8 *)NULL_PTR;
  uint8 *tr                     = (uint8 *)NULL_PTR;
  uint8 *key                    = (uint8 *)NULL_PTR;
  uint8 *mu                     = (uint8 *)NULL_PTR;
  uint8 *rhoprime               = (uint8 *)NULL_PTR;
  uint8 *rnd                    = (uint8 *)NULL_PTR;
  uint16 nonce                  = 0u;
  polyvecl_D2 mat[K_DILITHIUM2] = {{{{{0}}}}};
  polyvecl_D2 s1                = {{{{0}}}};
  polyvecl_D2 y                 = {{{{0}}}};
  polyvecl_D2 z                 = {{{{0}}}};
  polyveck_D2 t0                = {{{{0}}}};
  polyveck_D2 s2                = {{{{0}}}};
  polyveck_D2 w1                = {{{{0}}}};
  polyveck_D2 w0                = {{{{0}}}};
  polyveck_D2 h                 = {{{{0}}}};
  poly_D2 cp                    = {{0}};
  shake256incctx state          = {{0u}};
  boolean loop                  = TRUE;

  rho      = seedbuf;
  tr       = &rho[SEEDBYTES_DILITHIUM];
  key      = &tr[TRBYTES_DILITHIUM];
  rnd      = &key[SEEDBYTES_DILITHIUM];
  mu       = &rnd[RNDBYTES_DILITHIUM];
  rhoprime = &mu[CRHBYTES_DILITHIUM];
  FsmSw_Dilithium2_Unpack_Sk(rho, tr, key, &t0, &s1, &s2, sk);

  /* Compute mu = CRH(tr, msg) */
  FsmSw_Fips202_Shake256_IncInit(&state);
  FsmSw_Fips202_Shake256_IncAbsorb(&state, tr, TRBYTES_DILITHIUM);
  FsmSw_Fips202_Shake256_IncAbsorb(&state, m, mlen);
  FsmSw_Fips202_Shake256_IncFinalize(&state);
  FsmSw_Fips202_Shake256_IncSqueeze(mu, CRHBYTES_DILITHIUM, &state);
  for (n = 0u; n < RNDBYTES_DILITHIUM; n++)
  {
    rnd[n] = 0u;
  }
  FsmSw_Fips202_Shake256(rhoprime, CRHBYTES_DILITHIUM, key,
                         SEEDBYTES_DILITHIUM + RNDBYTES_DILITHIUM + CRHBYTES_DILITHIUM);

  /* Expand matrix and transform vectors */
  FsmSw_Dilithium2_Polyvec_MatrixExpand(mat, rho);
  FsmSw_Dilithium2_Polyvecl_Ntt(&s1);
  FsmSw_Dilithium2_Polyveck_Ntt(&s2);
  FsmSw_Dilithium2_Polyveck_Ntt(&t0);

  while (TRUE == loop)
  {
    /* Sample intermediate vector y */
    FsmSw_Dilithium2_Polyvecl_UniformGamma1(&y, rhoprime, nonce);
    nonce++;

    /* Matrix-vector multiplication */
    z = y;
    FsmSw_Dilithium2_Polyvecl_Ntt(&z);
    FsmSw_Dilithium2_Polyvec_MatrixPointwiseMontgomery(&w1, mat, &z);
    FsmSw_Dilithium2_Polyveck_Reduce(&w1);
    FsmSw_Dilithium2_Polyveck_InvnttTomont(&w1);

    /* Decompose w and call the random oracle */
    FsmSw_Dilithium2_Polyveck_CAddQ(&w1);
    FsmSw_Dilithium2_Polyveck_Decompose(&w1, &w0, &w1);
    FsmSw_Dilithium2_Polyveck_PackW1(sig, &w1);

    FsmSw_Fips202_Shake256_IncInit(&state);
    FsmSw_Fips202_Shake256_IncAbsorb(&state, mu, CRHBYTES_DILITHIUM);
    FsmSw_Fips202_Shake256_IncAbsorb(&state, sig, K_DILITHIUM2 * POLYW1_PACKEDBYTES_DILITHIUM2);
    FsmSw_Fips202_Shake256_IncFinalize(&state);
    FsmSw_Fips202_Shake256_IncSqueeze(sig, CTILDEBYTES_DILITHIUM2, &state);
    FsmSw_Dilithium2_Poly_Challenge(&cp, sig); /* uses only the first SEEDBYTES bytes of sig */
    FsmSw_Dilithium2_Poly_Ntt(&cp);

    /* Compute z, reject if it reveals secret */
    FsmSw_Dilithium2_Polyvecl_PointwisePolyMontgomery(&z, &cp, &s1);
    FsmSw_Dilithium2_Polyvecl_InvnttTomont(&z);
    FsmSw_Dilithium2_Polyvecl_Add(&z, &z, &y);
    FsmSw_Dilithium2_Polyvecl_Reduce(&z);
    if (0 < FsmSw_Dilithium2_Polyvecl_Chknorm(&z, (sint32)(GAMMA1_DILITHIUM2 - BETA_DILITHIUM2)))
    {
      continue;
    }

    /* Check that subtracting cs2 does not change high bits of w and low bits
         * do not reveal secret information */
    FsmSw_Dilithium2_Polyveck_PointwisePolyMontgomery(&h, &cp, &s2);
    FsmSw_Dilithium2_Polyveck_InvnttTomont(&h);
    FsmSw_Dilithium2_Polyveck_Sub(&w0, &w0, &h);
    FsmSw_Dilithium2_Polyveck_Reduce(&w0);
    if (0 < FsmSw_Dilithium2_Polyveck_Chknorm(&w0, (sint32)((uint32)((uint32)GAMMA2_DILITHIUM2 - BETA_DILITHIUM2))))
    {
      continue;
    }

    /* Compute hints for w1 */
    FsmSw_Dilithium2_Polyveck_PointwisePolyMontgomery(&h, &cp, &t0);
    FsmSw_Dilithium2_Polyveck_InvnttTomont(&h);
    FsmSw_Dilithium2_Polyveck_Reduce(&h);
    if (0 < FsmSw_Dilithium2_Polyveck_Chknorm(&h, (sint32)GAMMA2_DILITHIUM2))
    {
      continue;
    }

    FsmSw_Dilithium2_Polyveck_Add(&w0, &w0, &h);
    n = FsmSw_Dilithium2_Polyveck_MakeHint(&h, &w0, &w1);
    if (n > OMEGA_DILITHIUM2)
    {
      continue;
    }

    loop = FALSE;
  }
  /* Write signature */
  FsmSw_Dilithium2_Pack_Sig(sig, sig, &z, &h);
  *siglen = FSMSW_DILITHIUM2_CRYPTO_BYTES;
  return;
} // end: FsmSw_Dilithium2_Crypto_Sign_Signature
/*====================================================================================================================*/
/**
* \brief Compute signed message.
*
* \param[out] uint8       *sm :    pointer to output signed message (allocated
*                                     array with FSMSW_DILITHIUM2_CRYPTO_BYTES + mlen bytes), can be equal to m
* \param[out] uint32   *smlen : pointer to output length of signed message
* \param[in]  const uint8  *m : pointer to message to be signed
* \param[in]  uint32     mlen : length of message
* \param[in]  const uint8 *sk : pointer to bit-packed secret key
*/
void FsmSw_Dilithium2_Crypto_Sign(uint8 *sm, uint32 *smlen, const uint8 *m, uint32 mlen, const uint8 *sk)
{
  uint32 i = 0u;

  for (i = 0u; i < mlen; ++i)
  {
    sm[FSMSW_DILITHIUM2_CRYPTO_BYTES + mlen - 1u - i] = m[mlen - 1u - i];
  }
  (void)FsmSw_Dilithium2_Crypto_Sign_Signature(sm, smlen, &sm[FSMSW_DILITHIUM2_CRYPTO_BYTES], mlen, sk);
  *smlen += mlen;
  return;
} // end: FsmSw_Dilithium2_Crypto_Sign
/*====================================================================================================================*/
/**
* \brief Verifies signature.
*
* \param[in] uint8        *m : pointer to input signature
* \param[in] uint32   siglen : length of signature
* \param[in] const uint8  *m : pointer to message
* \param[in] uint32     mlen : length of message
* \param[in] const uint8 *pk : pointer to bit-packed public key
*
* \returns ERR_OK if signature could be verified correctly and ERR_NOT_OK otherwise
*/
/* polyspace +6 CERT-C:DCL15-C [Justified:]"This is an interface function 
designed for use by other systems that aim to integrate the Dilithium." */
/* polyspace +4 CERT-C:DCL19-C [Justified:]"This is an interface function 
designed for use by other systems that aim to integrate the Dilithium." */
/* polyspace +2 MISRA2012:8.7 [Justified:]"This is an interface function 
designed for use by other systems that aim to integrate the Dilithium." */
uint8 FsmSw_Dilithium2_Crypto_Sign_Verify(const uint8 *sig, uint32 siglen, const uint8 *m, uint32 mlen, const uint8 *pk)
{
  uint16 i                                                = 0u;
  uint8 buf[K_DILITHIUM2 * POLYW1_PACKEDBYTES_DILITHIUM2] = {0u};
  uint8 rho[SEEDBYTES_DILITHIUM]                          = {0u};
  uint8 mu[CRHBYTES_DILITHIUM]                            = {0u};
  uint8 c[CTILDEBYTES_DILITHIUM2]                         = {0u};
  uint8 c2[CTILDEBYTES_DILITHIUM2]                        = {0u};
  poly_D2 cp                                              = {{0}};
  polyvecl_D2 mat[K_DILITHIUM2]                           = {{{{{0}}}}};
  polyvecl_D2 z                                           = {{{{0}}}};
  polyveck_D2 t1                                          = {{{{0}}}};
  polyveck_D2 w1                                          = {{{{0}}}};
  polyveck_D2 h                                           = {{{{0}}}};
  shake256incctx state                                    = {{0u}};

  uint8 retVal = ERR_OK;

  if (siglen != FSMSW_DILITHIUM2_CRYPTO_BYTES)
  {
    retVal = ERR_NOT_OK;
  }

  FsmSw_Dilithium2_Unpack_Pk(rho, &t1, pk);
  if (0 < FsmSw_Dilithium2_Unpack_Sig(c, &z, &h, sig))
  {
    retVal = ERR_NOT_OK;
  }
  if (0 < FsmSw_Dilithium2_Polyvecl_Chknorm(&z, (sint32)(GAMMA1_DILITHIUM2 - BETA_DILITHIUM2)))
  {
    retVal = ERR_NOT_OK;
  }

  /* Compute CRH(H(rho, t1), msg) */
  FsmSw_Fips202_Shake256(mu, CRHBYTES_DILITHIUM, pk, FSMSW_DILITHIUM2_CRYPTO_PUBLICKEYBYTES);
  FsmSw_Fips202_Shake256_IncInit(&state);
  FsmSw_Fips202_Shake256_IncAbsorb(&state, mu, CRHBYTES_DILITHIUM);
  FsmSw_Fips202_Shake256_IncAbsorb(&state, m, mlen);
  FsmSw_Fips202_Shake256_IncFinalize(&state);
  FsmSw_Fips202_Shake256_IncSqueeze(mu, CRHBYTES_DILITHIUM, &state);

  /* Matrix-vector multiplication; compute Az - c2^dt1 */
  FsmSw_Dilithium2_Poly_Challenge(&cp, c); /* uses only the first SEEDBYTES bytes of c */
  FsmSw_Dilithium2_Polyvec_MatrixExpand(mat, rho);

  FsmSw_Dilithium2_Polyvecl_Ntt(&z);
  FsmSw_Dilithium2_Polyvec_MatrixPointwiseMontgomery(&w1, mat, &z);

  FsmSw_Dilithium2_Poly_Ntt(&cp);
  FsmSw_Dilithium2_Polyveck_Shiftl(&t1);
  FsmSw_Dilithium2_Polyveck_Ntt(&t1);
  FsmSw_Dilithium2_Polyveck_PointwisePolyMontgomery(&t1, &cp, &t1);

  FsmSw_Dilithium2_Polyveck_Sub(&w1, &w1, &t1);
  FsmSw_Dilithium2_Polyveck_Reduce(&w1);
  FsmSw_Dilithium2_Polyveck_InvnttTomont(&w1);

  /* Reconstruct w1 */
  FsmSw_Dilithium2_Polyveck_CAddQ(&w1);
  FsmSw_Dilithium2_Polyveck_UseHint(&w1, &w1, &h);
  FsmSw_Dilithium2_Polyveck_PackW1(buf, &w1);

  /* Call random oracle and verify FsmSw_Dilithium2_Challenge */
  FsmSw_Fips202_Shake256_IncInit(&state);
  FsmSw_Fips202_Shake256_IncAbsorb(&state, mu, CRHBYTES_DILITHIUM);
  FsmSw_Fips202_Shake256_IncAbsorb(&state, buf, K_DILITHIUM2 * POLYW1_PACKEDBYTES_DILITHIUM2);
  FsmSw_Fips202_Shake256_IncFinalize(&state);
  FsmSw_Fips202_Shake256_IncSqueeze(c2, CTILDEBYTES_DILITHIUM2, &state);
  for (i = 0u; i < CTILDEBYTES_DILITHIUM2; ++i)
  {
    if (c[i] != c2[i])
    {
      retVal = ERR_NOT_OK;
    }
  }
  return retVal;
} // end: FsmSw_Dilithium2_Crypto_Sign_Verify
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
uint8 FsmSw_Dilithium2_Crypto_Sign_Open(uint8 *m, uint32 *mlen, const uint8 *sm, uint32 smlen, const uint8 *pk)
{
  uint32 i     = 0u;
  uint8 retVal = ERR_NOT_OK;

  if (smlen >= FSMSW_DILITHIUM2_CRYPTO_BYTES)
  {
    *mlen = smlen - FSMSW_DILITHIUM2_CRYPTO_BYTES;
    if (0u == FsmSw_Dilithium2_Crypto_Sign_Verify(sm, FSMSW_DILITHIUM2_CRYPTO_BYTES, &sm[FSMSW_DILITHIUM2_CRYPTO_BYTES],
                                                  *mlen, pk))
    {
      /* All good, copy msg, return 1 */
      for (i = 0u; i < *mlen; ++i)
      {
        m[i] = sm[FSMSW_DILITHIUM2_CRYPTO_BYTES + i];
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
} // end: FsmSw_Dilithium2_Crypto_Sign_Open

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
