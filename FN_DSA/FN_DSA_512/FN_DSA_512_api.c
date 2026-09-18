/***********************************************************************************************************************
 *
 * Original implementation: PQClean, FN_DSA
 *
 * Copyright (c) 2017-2019 FN_DSA Project
 * Copyright 2026 IAV GmbH
 *
 * Original portions are licensed under the MIT License.
 * IAV modifications are licensed under the Apache License, Version 2.0.
 *
 * SPDX-License-Identifier: MIT AND Apache-2.0
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FN_DSA
*    includes the modules for SwC FN_DSA
 ** @{ */
/** \addtogroup FN_DSA_512
*    includes the modules for FN_DSA_512
 ** @{ */
/** \addtogroup FN_DSA_512_api
 ** @{ */

/*====================================================================================================================*/
/** \file FN_DSA_512_api.c
* \brief  description of FN_DSA_512_api.c
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
/* Wrapper for implementing the PQClean API. */
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_CommonLib.h"
#include "FN_DSA_codec.h"
#include "FN_DSA_common.h"
#include "FN_DSA_fpr.h"
#include "FN_DSA_keygen.h"
#include "FN_DSA_sign.h"
#include "FN_DSA_vrfy.h"

#include "FN_DSA_512_api.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define FN_DSA_512_NONCELEN                40u
#define FN_DSA_512_BUFFER_SIZE             512
#define FN_DSA_512_SEED_BUFFER_SIZE        48
#define FN_DSA_UINT32_MAX_VALUE                  0xFFFFFFFFu
#define FN_DSA_512_TMP1_STRUCT_BUFFER_SIZE 36864
#define FN_DSA_512_TMP3_STRUCT_BUFFER_SIZE 1024
#define FN_DSA_512_LOGN                    9u
/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct
{
  uint8 b[FN_DSA_KEYGEN_TEMP_9];
} tmp2_512_struct;

typedef struct
{
  uint8 b[FN_DSA_512_TMP1_STRUCT_BUFFER_SIZE];
} tmp1_512_struct;

typedef struct
{
  uint8 b[FN_DSA_512_TMP3_STRUCT_BUFFER_SIZE];
} tmp3_512_struct;

typedef struct
{
  sint16 sig[FN_DSA_512_BUFFER_SIZE];
  uint16 hm[FN_DSA_512_BUFFER_SIZE];
} r_512_struct;
/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/
static r_512_struct r_512;
/**********************************************************************************************************************/
/* GLOBAL CONSTANTS                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/
static sint32 fn_dsa512_DoSign(uint8 *const nonce, uint8 *const sigbuf, uint32 *const sigbuflen,
                                     const uint8 *const m, uint32 mlen, const uint8 *const sk);
static sint32 fn_dsa512_DoVerify(const uint8 *const nonce, const uint8 *const sigbuf, uint32 sigbuflen,
                                       const uint8 *const m, uint32 mlen, const uint8 *const pk);
/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
 * \brief If a signature could be computed but not encoded because it would exceed the output buffer size, then a
 *        new signature is computed. If the provided buffer size is too low, this could loop indefinitely, so the
 *        caller must provide a size that can accommodate signatures with a large enough probability.
 *
 * \param[out] uint8      *nonce : receives the nonce and must have length FN_DSA_512_NONCELEN bytes
 * \param[out] uint8     *sigbuf : receives the signature value (without nonce or header byte)
 * \param[out] uint32 *sigbuflen : providing the maximum value length and receiving the actual value length
 * \param[in]  const uint8    *m : t.b.d.
 * \param[in]  uint32       mlen : t.b.d.
 * \param[in]  const uint8   *sk : t.b.d.
 *
 * \returns 0 on success, -1 on error.
 *
 */
static sint32 fn_dsa512_DoSign(uint8 *const nonce, uint8 *const sigbuf, uint32 *const sigbuflen,
                                     const uint8 *const m, uint32 mlen, const uint8 *const sk)
{
  sint8 f[FN_DSA_512_BUFFER_SIZE]         = {0};
  sint8 g[FN_DSA_512_BUFFER_SIZE]         = {0};
  sint8 F[FN_DSA_512_BUFFER_SIZE]         = {0};
  sint8 G[FN_DSA_512_BUFFER_SIZE]         = {0};
  uint8 seed[FN_DSA_512_SEED_BUFFER_SIZE] = {0};
  inner_shake256_context sc                    = {{0}};
  uint32 u                                     = 0;
  uint32 v                                     = 0;
  boolean bStopFunc                            = FALSE;
  sint8 retVal                                 = -1;
  tmp1_512_struct tmp1_512                     = {{0}};

  /* Decode the private key. */
  if (sk[0] != (0x50u + FN_DSA_512_LOGN))
  {
    bStopFunc = TRUE;
  }

  u = 1u;
  v = FN_DSA_TrimI8Decode(f, FN_DSA_512_LOGN, FN_DSA_max_small_fg_bits[FN_DSA_512_LOGN], &sk[u],
                                FN_DSA_512_CRYPTO_SECRETKEYBYTES - u);

  if (v == 0u)
  {
    bStopFunc = TRUE;
  }

  u += v;
  v = FN_DSA_TrimI8Decode(g, FN_DSA_512_LOGN, FN_DSA_max_small_fg_bits[FN_DSA_512_LOGN], &sk[u],
                                FN_DSA_512_CRYPTO_SECRETKEYBYTES - u);

  if (v == 0u)
  {
    bStopFunc = TRUE;
  }

  u += v;

  v = FN_DSA_TrimI8Decode(F, FN_DSA_512_LOGN, FN_DSA_max_big_FG_bits[FN_DSA_512_LOGN], &sk[u],
                                FN_DSA_512_CRYPTO_SECRETKEYBYTES - u);

  if (v == 0u)
  {
    bStopFunc = TRUE;
  }

  u += v;

  if (u != FN_DSA_512_CRYPTO_SECRETKEYBYTES)
  {
    bStopFunc = TRUE;
  }

  if (0 == FN_DSA_CompletePrivate(G, f, g, F, FN_DSA_512_LOGN, tmp1_512.b))
  {
    bStopFunc = TRUE;
  }

  if (bStopFunc == FALSE)
  {
    /* Create a random nonce (40 bytes). */
    (void)FsmSw_CommonLib_RandomBytes(nonce, FN_DSA_512_NONCELEN);

    /* Hash message nonce + message into a vector. */
    FsmSw_Fips202_Shake256_IncInit(&sc);
    FsmSw_Fips202_Shake256_IncAbsorb(&sc, nonce, FN_DSA_512_NONCELEN);
    FsmSw_Fips202_Shake256_IncAbsorb(&sc, m, mlen);
    FsmSw_Fips202_Shake256_IncFinalize(&sc);
    FN_DSA_HashToPointCt(&sc, r_512.hm, FN_DSA_512_LOGN, tmp1_512.b);

    /* Initialize a RNG. */
    (void)FsmSw_CommonLib_RandomBytes(seed, sizeof(seed));
    FsmSw_Fips202_Shake256_IncInit(&sc);
    FsmSw_Fips202_Shake256_IncAbsorb(&sc, seed, sizeof(seed));
    FsmSw_Fips202_Shake256_IncFinalize(&sc);

    /* Compute and return the signature. This loops until a signature value is found that fits in the provided
     * buffer. */
    for (uint32 i = 0; i < FN_DSA_UINT32_MAX_VALUE; i++)
    {
      FN_DSA_Sign_Dyn(r_512.sig, &sc, f, g, F, G, r_512.hm, FN_DSA_512_LOGN, tmp1_512.b);
      v = FN_DSA_CompEncode(sigbuf, *sigbuflen, r_512.sig, FN_DSA_512_LOGN);

      if (v != 0u)
      {
        *sigbuflen = v;
        retVal     = 0;
        break;
      }
    }
  }

  return retVal;
} // end: fn_dsa512_DoSign

/*====================================================================================================================*/
/**
 * \brief Verify a signature.
 *
 * \param[out] uint8     *nonce : The nonce has size FN_DSA_512_NONCELEN bytes
 * \param[out] uint8    *sigbuf : contains the signature value
 * \param[in]  uint32 sigbuflen : providing the maximum value length
 * \param[in]  const uint8   *m : t.b.d.
 * \param[in]  uint32      mlen : t.b.d.
 * \param[in]  const uint8  *pk : t.b.d.
 *
 * \returns 0 on success, -1 on error.
 *
 */
static sint32 fn_dsa512_DoVerify(const uint8 *const nonce, const uint8 *const sigbuf, uint32 sigbuflen,
                                       const uint8 *const m, uint32 mlen, const uint8 *const pk)
{
  uint16 h[FN_DSA_512_BUFFER_SIZE]   = {0};
  uint16 hm[FN_DSA_512_BUFFER_SIZE]  = {0};
  sint16 sig[FN_DSA_512_BUFFER_SIZE] = {0};
  inner_shake256_context sc               = {{0}};
  sint8 retVal                            = 0;
  tmp3_512_struct tmp3_512                = {{0}};

  /* Decode public key. */
  if (pk[0] != (FN_DSA_512_LOGN))
  {
    retVal = -1;
  }
  else if (FN_DSA_ModqDecode(h, FN_DSA_512_LOGN, &pk[1], FN_DSA_512_CRYPTO_PUBLICKEYBYTES - 1u) !=
           FN_DSA_512_CRYPTO_PUBLICKEYBYTES - 1u)
  {
    retVal = -1;
  }
  else
  {
    FN_DSA_ToNttMonty(h, FN_DSA_512_LOGN);

    /* Decode signature. */
    if (sigbuflen == 0u)
    {
      retVal = -1;
    }
    else if (FN_DSA_CompDecode(sig, FN_DSA_512_LOGN, sigbuf, sigbuflen) != sigbuflen)
    {
      retVal = -1;
    }
    else
    {
      /* Hash nonce + message into a vector. */
      FsmSw_Fips202_Shake256_IncInit(&sc);
      FsmSw_Fips202_Shake256_IncAbsorb(&sc, nonce, FN_DSA_512_NONCELEN);
      FsmSw_Fips202_Shake256_IncAbsorb(&sc, m, mlen);
      FsmSw_Fips202_Shake256_IncFinalize(&sc);
      FN_DSA_HashToPointCt(&sc, hm, FN_DSA_512_LOGN, tmp3_512.b);

      /* Verify signature. */
      if (0 == FN_DSA_VerifyRaw(hm, sig, h, FN_DSA_512_LOGN, tmp3_512.b))
      {
        retVal = -1;
      }
    }
  }
  return retVal;
} // end: fn_dsa512_DoVerify

/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

/* Encoding formats (nnnn = log of degree, 9 for FN_DSA-512, 10 for FN_DSA-1024)
 *
 *   private key:
 *      header byte: 0101nnnn
 *      private f  (6 or 5 bits by element, depending on degree)
 *      private g  (6 or 5 bits by element, depending on degree)
 *      private F  (8 bits by element)
 *
 *   public key:
 *      header byte: 0000nnnn
 *      public h   (14 bits by element)
 *
 *   signature:
 *      header byte: 0011nnnn
 *      nonce     40 bytes
 *      value     (12 bits by element)
 *
 *   message + signature:
 *      signature length   (2 bytes, big-endian)
 *      nonce              40 bytes
 *      message
 *      header byte:       0010nnnn
 *      value              (12 bits by element)
 *      (signature length is 1+len(value), not counting the nonce) */

/*====================================================================================================================*/
/**
 * \brief Generate a new key pair. Public key goes into pk[], private key in sk[]. Key sizes are exact (in bytes).
 *        public (pk):  FN_DSA_512_CRYPTO_PUBLICKEYBYTES
 *        private (sk): FN_DSA_512_CRYPTO_SECRETKEYBYTES
 *
 * \param[out] uint8 *pk : Key sizes is FN_DSA_512_CRYPTO_PUBLICKEYBYTES
 * \param[out] uint8 *sk : Key sizes is FN_DSA_512_CRYPTO_SECRETKEYBYTES
 *
 * \returns ERR_OK on success, ERR_NOT_OK on error.
 *
 */
uint8 FN_DSA_512_Crypto_Sign_KeyPair(uint8 *const pk, uint8 *const sk)
{
  sint8 f[FN_DSA_512_BUFFER_SIZE]         = {0};
  sint8 g[FN_DSA_512_BUFFER_SIZE]         = {0};
  sint8 F[FN_DSA_512_BUFFER_SIZE]         = {0};
  uint16 h[FN_DSA_512_BUFFER_SIZE]        = {0};
  uint8 seed[FN_DSA_512_SEED_BUFFER_SIZE] = {0};
  inner_shake256_context rng                   = {{0}};
  uint32 u                                     = 0;
  uint32 v                                     = 0;
  uint8 retVal                                 = ERR_OK;
  boolean bStopFunc                            = FALSE;
  tmp2_512_struct tmp2_512                     = {{0}};

  /* Generate key pair. */
  (void)FsmSw_CommonLib_RandomBytes(seed, sizeof(seed));
  FsmSw_Fips202_Shake256_IncInit(&rng);
  FsmSw_Fips202_Shake256_IncAbsorb(&rng, seed, sizeof(seed));
  FsmSw_Fips202_Shake256_IncFinalize(&rng);
  FN_DSA_Keygen(&rng, f, g, F, (sint8 *)NULL_PTR, h, FN_DSA_512_LOGN, tmp2_512.b);

  /* Encode private key. */
  sk[0] = 0x50u + FN_DSA_512_LOGN;
  u     = 1u;
  v     = FN_DSA_TrimI8Encode(&sk[u], FN_DSA_512_CRYPTO_SECRETKEYBYTES - u, f, FN_DSA_512_LOGN,
                                    FN_DSA_max_small_fg_bits[FN_DSA_512_LOGN]);

  if (v == 0u)
  {
    retVal    = ERR_NOT_OK;
    bStopFunc = TRUE;
  }

  u += v;
  v = FN_DSA_TrimI8Encode(&sk[u], FN_DSA_512_CRYPTO_SECRETKEYBYTES - u, g, FN_DSA_512_LOGN,
                                FN_DSA_max_small_fg_bits[FN_DSA_512_LOGN]);

  if (v == 0u)
  {
    retVal    = ERR_NOT_OK;
    bStopFunc = TRUE;
  }

  u += v;
  v = FN_DSA_TrimI8Encode(&sk[u], FN_DSA_512_CRYPTO_SECRETKEYBYTES - u, F, FN_DSA_512_LOGN,
                                (uint32)FN_DSA_max_big_FG_bits[FN_DSA_512_LOGN]);

  if (v == 0u)
  {
    retVal    = ERR_NOT_OK;
    bStopFunc = TRUE;
  }
  u += v;
  if (u != FN_DSA_512_CRYPTO_SECRETKEYBYTES)
  {
    retVal    = ERR_NOT_OK;
    bStopFunc = TRUE;
  }

  if (FALSE == bStopFunc)
  {
    /* Encode public key. */
    pk[0] = FN_DSA_512_LOGN;
    v     = FN_DSA_ModqEncode(&pk[1], FN_DSA_512_CRYPTO_PUBLICKEYBYTES - 1u, h, FN_DSA_512_LOGN);

    if (v != FN_DSA_512_CRYPTO_PUBLICKEYBYTES - 1u)
    {
      retVal = ERR_NOT_OK;
    }
  }

  return retVal;
} // end: FN_DSA_512_Crypto_Sign_KeyPair

/*====================================================================================================================*/
/**
 * \brief Compute a signature on a provided message (m, mlen), with a given private key (sk). Signature is written
 *        in sig[], with length written into *siglen. Signature length is variable; maximum signature length
 *        (in bytes) is FN_DSA_512_CRYPTO_BYTES. sig[], m[] and sk[] may overlap each other arbitrarily.
 *
 * \param[out] uint8      *sig : t.b.d.
 * \param[out] uint32  *siglen : t.b.d.
 * \param[in]  const uint8  *m : t.b.d.
 * \param[in]  uint32     mlen : t.b.d.
 * \param[in]  const uint8 *sk : t.b.d.
 *
 * \returns ERR_OK on success, ERR_NOT_OK on error.
 *
 */
uint8 FN_DSA_512_Crypto_Sign_Signature(uint8 *const sig, uint32 *const siglen, const uint8 *const m, uint32 mlen,
                                            const uint8 *const sk)
{
  /*The FN_DSA_512_CRYPTO_BYTES constant is used for the signed message object (as produced by crypto_sign())
   * and includes a two-byte length value, so we take care here to only generate signatures that are two bytes shorter
   * than the maximum. This is done to ensure that crypto_sign() and crypto_sign_signature() produce the exact same
   * signature value, if used on the same message, with the same private key, and using the same output from
   * FsmSw_CommonLib_RandomBytes() (this is for reproducibility of tests). */
  uint32 vlen  = 0;
  uint8 retVal = ERR_OK;

  vlen = FN_DSA_512_CRYPTO_BYTES - FN_DSA_512_NONCELEN - 3u;

  if (fn_dsa512_DoSign(&sig[1], &sig[1u + FN_DSA_512_NONCELEN], &vlen, m, mlen, sk) < 0)
  {
    retVal = ERR_NOT_OK;
  }
  else
  {
    sig[0]  = 0x30 + FN_DSA_512_LOGN;
    *siglen = 1u + FN_DSA_512_NONCELEN + vlen;
  }

  return retVal;
} // end: FN_DSA_512_Crypto_Sign_Signature

/*====================================================================================================================*/
/**
 * \brief Verify a signature (sig, siglen) on a message (m, mlen) with a given public key (pk).
 *        sig[], m[] and sk[] may overlap each other arbitrarily.
 *
 * \param[in]  const uint8 *sig : t.b.d.
 * \param[out] uint32   *siglen : t.b.d.
 * \param[in]  const uint8   *m : t.b.d.
 * \param[in]  uint32      mlen : t.b.d.
 * \param[in]  const uint8  *pk : t.b.d.
 *
 * \returns ERR_OK on success, ERR_NOT_OK on error.
 *
 */
uint8 FN_DSA_512_Crypto_Sign_Verify(const uint8 *const sig, uint32 siglen, const uint8 *const m, uint32 mlen,
                                         const uint8 *const pk)
{
  uint8 retVal = ERR_OK;

  if (0 != fn_dsa512_DoVerify(&sig[1], &sig[1u + FN_DSA_512_NONCELEN],
                                    siglen - 1u - FN_DSA_512_NONCELEN, m, mlen, pk))
  {
    retVal = ERR_NOT_OK;
  }

  if (siglen < (1u + FN_DSA_512_NONCELEN))
  {
    retVal = ERR_NOT_OK;
  }

  if (sig[0] != (0x30u + FN_DSA_512_LOGN))
  {
    retVal = ERR_NOT_OK;
  }

  return retVal;
} // end: FN_DSA_512_Crypto_Sign_Verify

/*====================================================================================================================*/
/**
 * \brief Compute a signature on a message and pack the signature and message into a single object, written into
 *        sm[]. The length of that output is written in *smlen; that length may be larger than the message length
 *        (mlen) by up to FN_DSA_512_CRYPTO_BYTES. sm[] and m[] may overlap each other arbitrarily; however,
 *        sm[] shall not overlap with sk[].
 *
 * \param[in]  const uint8 *sig : t.b.d.
 * \param[out] uint32   *siglen : t.b.d.
 * \param[in]  const uint8   *m : t.b.d.
 * \param[in]  uint32      mlen : t.b.d.
 * \param[in]  const uint8  *pk : t.b.d.
 *
 * \returns ERR_OK on success, ERR_NOT_OK on error.
 *
 */
uint8 FN_DSA_512_Crypto_Sign(uint8 *const sm, uint32 *const smlen, const uint8 *const m, uint32 mlen,
                                  const uint8 *const sk)
{
  uint8 *pm        = (uint8 *)NULL_PTR;
  uint8 *sigbuf    = (uint8 *)NULL_PTR;
  uint32 sigbuflen = 0;
  uint8 retVal     = ERR_OK;

  /* Move the message to its final location; this is a FsmSw_CommonLib_MemMove() so it handles overlaps properly. */
  FsmSw_CommonLib_MemMove(&sm[2u + FN_DSA_512_NONCELEN], m, mlen);
  pm        = &sm[2u + FN_DSA_512_NONCELEN];
  sigbuf    = &pm[1u + mlen];
  sigbuflen = FN_DSA_512_CRYPTO_BYTES - FN_DSA_512_NONCELEN - 3u;

  if (fn_dsa512_DoSign(&sm[2u], sigbuf, &sigbuflen, pm, mlen, sk) < 0)
  {
    retVal = ERR_NOT_OK;
  }
  else
  {
    pm[mlen] = 0x20 + FN_DSA_512_LOGN;
    sigbuflen++;
    sm[0]  = (uint8)(sigbuflen >> 8);
    sm[1]  = (uint8)sigbuflen;
    *smlen = mlen + 2u + FN_DSA_512_NONCELEN + sigbuflen;
  }

  return retVal;
} // end: FN_DSA_512_Crypto_Sign

/*====================================================================================================================*/
/**
 * \brief Open a signed message object (sm, smlen) and verify the signature; on success, the message itself is
 *        written into m[] and its length into *mlen. The message is shorter than the signed message object,
 *        but the size difference depends on the signature value; the difference may range up to
 *        FN_DSA_512_CRYPTO_BYTES.
 *        m[], sm[] and pk[] may overlap each other arbitrarily.
 *
 * \param[out] uint8        *m : t.b.d.
 * \param[out] uint32    *mlen : t.b.d.
 * \param[in]  const uint8 *sm : t.b.d.
 * \param[out] uint32   *smlen : t.b.d.
 * \param[in]  const uint8 *pk : t.b.d.
 *
 * \returns ERR_OK on success, ERR_NOT_OK on error.
 *
 */
uint8 FN_DSA_512_Crypto_Sign_Open(uint8 *const m, uint32 *const mlen, const uint8 *const sm, uint32 smlen,
                                       const uint8 *const pk)
{
  const uint8 *sigbuf = (uint8 *)NULL_PTR;
  uint32 pmlen        = 0;
  uint32 sigbuflen    = 0;
  uint8 retVal        = ERR_OK;
  boolean bStopFunc   = FALSE;

  if (smlen < (3u + FN_DSA_512_NONCELEN))
  {
    retVal    = ERR_NOT_OK;
    bStopFunc = TRUE;
  }
  sigbuflen = ((uint32)sm[0] << 8) | (uint32)sm[1];
  if (bStopFunc == FALSE)
  {
    if ((sigbuflen < 2u) || (sigbuflen > (smlen - FN_DSA_512_NONCELEN - 2u)))
    {
      retVal    = ERR_NOT_OK;
      bStopFunc = TRUE;
    }
    sigbuflen--;
    pmlen = smlen - FN_DSA_512_NONCELEN - 3u - sigbuflen;

    if ((bStopFunc == FALSE) && (sm[2u + FN_DSA_512_NONCELEN + pmlen] != (0x20u + FN_DSA_512_LOGN)))
    {
      retVal    = ERR_NOT_OK;
      bStopFunc = TRUE;
    }

    sigbuf = &sm[2u + FN_DSA_512_NONCELEN + pmlen + 1u];

    /* The 2-byte length header and the one-byte signature header have been verified. Nonce is at sm+2, followed by the
      * message itself. Message length is in pmlen. sigbuf/sigbuflen point to the signature value (excluding the header
      * byte). */
    /* polyspace +2 MISRA2012:13.5 [Justified:]"DoVerify function returns 0 on success and -1 on error, which is
    equivalent to works/doesn't work" */
    if ((bStopFunc == FALSE) &&
        (fn_dsa512_DoVerify(&sm[2], sigbuf, sigbuflen, &sm[2u + FN_DSA_512_NONCELEN], pmlen, pk) < 0))
    {
      retVal = ERR_NOT_OK;
    }
    else
    {
      /* Signature is correct, we just have to copy/move the message to its final destination.
      * The FsmSw_CommonLib_MemMove() properly handles overlaps. */
      FsmSw_CommonLib_MemMove(m, &sm[2u + FN_DSA_512_NONCELEN], pmlen);
      *mlen = pmlen;
    }
  }
  return retVal;
} // end: FN_DSA_512_Crypto_Sign_Open

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
