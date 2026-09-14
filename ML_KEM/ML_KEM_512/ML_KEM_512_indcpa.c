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
/** \addtogroup ML_KEM_512_indcpa
 ** @{ */

/*====================================================================================================================*/
/** \file ML_KEM_512_indcpa.c
* \brief  description of ML_KEM_512_indcpa.c
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
#include "ML_KEM_512_params.h"
#include "ML_KEM_512_poly.h"
#include "ML_KEM_512_polyvec.h"
#include "ML_KEM_ntt.h"
#include "ML_KEM_poly.h"
#include "ML_KEM_symmetric.h"
#include "Std_Types.h"

#include "ML_KEM_512_indcpa.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
/* polyspace +2 MISRA2012:2.2 [Justified:]"The function is used deeper in the code for crucial calculations 
if the defines change, so it's not dead code." */
#define GEN_MATRIX_NBLOCKS (((((12u * ML_KEM_N) / (8u * 4096u)) / ML_KEM_Q) + XOF_BLOCKBYTES) / XOF_BLOCKBYTES)
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
static void ml_kem_512_PackPk(uint8 r[ML_KEM_512_INDCPA_PUBLICKEYBYTES], const polyvec512 *const pk,
                                  const uint8 seed[ML_KEM_SYMBYTES]);
static void ml_kem_512_UnpackPk(polyvec512 *const pk, uint8 seed[ML_KEM_SYMBYTES],
                                    const uint8 packedpk[ML_KEM_512_INDCPA_PUBLICKEYBYTES]);
static void ml_kem_512_PackSk(uint8 r[ML_KEM_512_INDCPA_SECRETKEYBYTES], const polyvec512 *const sk);
static void ml_kem_512_UnpackSk(polyvec512 *const sk, const uint8 packedsk[ML_KEM_512_INDCPA_SECRETKEYBYTES]);
static void ml_kem_512_PackCiphertext(uint8 r[ML_KEM_512_INDCPA_BYTES], const polyvec512 *const b,
                                          const poly *const v);
static void ml_kem_512_UnpackCiphertext(polyvec512 *const b, poly *const v, const uint8 c[ML_KEM_512_INDCPA_BYTES]);
static uint16 ml_kem_512_RejUniform(sint16 *const r, uint16 len, const uint8 *const buf, uint16 buflen);
/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
* \brief Serialize the public key as concatenation of the
*        serialized vector of polynomials pk
*        and the public seed used to generate the matrix A.
*
* \param[out] uint8             *r : pointer to the output serialized public key
* \param[in]  const polyvec512 *pk : pointer to the input public-key polyvec
* \param[in]  const uint8    *seed : pointer to the input public seed
*/
static void ml_kem_512_PackPk(uint8 r[ML_KEM_512_INDCPA_PUBLICKEYBYTES], const polyvec512 *const pk,
                                  const uint8 seed[ML_KEM_SYMBYTES])
{
  uint32 i = 0;

  ML_KEM_512_Polyvec_ToBytes(r, pk);

  for (i = 0; i < ML_KEM_SYMBYTES; i++)
  {
    r[i + ML_KEM_512_POLYVECBYTES] = seed[i];
  }
} // end: ml_kem_512_PackPk

/*====================================================================================================================*/
/**
* \brief De-serialize public key from a byte array;
*        approximate inverse of ml_kem_512_PackPk
*
* \param[out] polyvec512        *pk : pointer to output public-key polynomial vector
* \param[out] uint8           *seed : pointer to output seed to generate matrix A
* \param[in]  const uint8 *packedpk : pointer to input serialized public key
*/
static void ml_kem_512_UnpackPk(polyvec512 *const pk, uint8 seed[ML_KEM_SYMBYTES],
                                    const uint8 packedpk[ML_KEM_512_INDCPA_PUBLICKEYBYTES])
{
  uint32 i = 0;

  ML_KEM_512_Polyvec_FromBytes(pk, packedpk);

  for (i = 0; i < ML_KEM_SYMBYTES; i++)
  {
    seed[i] = packedpk[i + ML_KEM_512_POLYVECBYTES];
  }
} // end: ml_kem_512_UnpackPk

/*====================================================================================================================*/
/**
* \brief Serialize the secret key
*
* \param[out] uint8             *r : pointer to output serialized secret key
* \param[in]  const polyvec512 *sk : pointer to input vector of polynomials (secret key)
*/
static void ml_kem_512_PackSk(uint8 r[ML_KEM_512_INDCPA_SECRETKEYBYTES], const polyvec512 *const sk)
{
  ML_KEM_512_Polyvec_ToBytes(r, sk);
} // end: ml_kem_512_PackSk

/*====================================================================================================================*/
/**
* \brief De-serialize the secret key; inverse of ml_kem_512_PackSk
*
* \param[out] polyvec512        *sk : pointer to output vector of polynomials (secret key)
* \param[in]  const uint8 *packedsk : pointer to input serialized secret key
*/
static void ml_kem_512_UnpackSk(polyvec512 *const sk, const uint8 packedsk[ML_KEM_512_INDCPA_SECRETKEYBYTES])
{
  ML_KEM_512_Polyvec_FromBytes(sk, packedsk);
} // end: ml_kem_512_UnpackSk

/*====================================================================================================================*/
/**
* \brief Serialize the ciphertext as concatenation of the
*        compressed and serialized vector of polynomials b
*        and the compressed and serialized polynomial v
*
* \param[out] uint8      *r : pointer to the output serialized ciphertext
* \param[in]  const poly *b : pointer to the input vector of polynomials b
* \param[in]  const poly *v : pointer to the input polynomial v
*/
static void ml_kem_512_PackCiphertext(uint8 r[ML_KEM_512_INDCPA_BYTES], const polyvec512 *const b,
                                          const poly *const v)
{
  ML_KEM_512_Polyvec_Compress(r, b);
  ML_KEM_512_Poly_Compress(&r[ML_KEM_512_POLYVECCOMPRESSEDBYTES], v);
} // end: ml_kem_512_PackCiphertext

/*====================================================================================================================*/
/**
* \brief De-serialize and decompress ciphertext from a byte array;
*        approximate inverse of ml_kem_512_PackCiphertext
*
* \param[out] polyvec512  *b : pointer to the output vector of polynomials b
* \param[out] poly        *v : pointer to the output polynomial v
* \param[in]  const uint8 *c : pointer to the input serialized ciphertext
*/
static void ml_kem_512_UnpackCiphertext(polyvec512 *const b, poly *const v, const uint8 c[ML_KEM_512_INDCPA_BYTES])
{
  ML_KEM_512_Polyvec_Decompress(b, c);
  ML_KEM_512_Poly_Decompress(v, &c[ML_KEM_512_POLYVECCOMPRESSEDBYTES]);
} // end: ml_kem_512_UnpackCiphertext

/*====================================================================================================================*/
/**
* \brief Run rejection sampling on uniform random bytes to generate
*        uniform random integers mod q
*
* \param[out] int16_t       *r : pointer to output buffer
* \param[in]  uint16       len : requested number of 16-bit integers (uniform mod q)
* \param[in]  const uint8 *buf : pointer to input buffer (assumed to be uniformly random bytes)
* \param[in]  uint16    buflen : length of input buffer in bytes
*
* \returns number of sampled 16-bit integers (at most len)
*/
static uint16 ml_kem_512_RejUniform(sint16 *const r, uint16 len, const uint8 *const buf, uint16 buflen)
{
  uint16 ctr  = 0;
  uint16 pos  = 0;
  uint16 val0 = 0;
  uint16 val1 = 0;

  while ((ctr < len) && ((pos + 3u) <= buflen))
  {
    val0 = (((uint16)buf[pos] >> 0u) | ((uint16)buf[pos + 1u] << 8u)) & 0xFFFu;
    val1 = (((uint16)buf[pos + 1u] >> 4u) | ((uint16)buf[pos + 2u] << 4u)) & 0xFFFu;
    pos  = pos + 3u;

    if (val0 < ML_KEM_Q)
    {
      r[ctr] = (sint16)val0;
      ctr++;
    }

    if ((ctr < len) && (val1 < ML_KEM_Q))
    {
      r[ctr] = (sint16)val1;
      ctr++;
    }
  }

  return ctr;
} // end: ml_kem_512_RejUniform
/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
* \brief Deterministically generate matrix A (or the transpose of A)
*        from a seed. Entries of the matrix are polynomials that look
*        uniformly random. Performs rejection sampling on output of
*        a XOF
*
* \param[out] polyvec512     *a : pointer to output matrix A
* \param[in]  const uint8 *seed : pointer to input seed
* \param[in]  uint8  transposed : boolean deciding whether A or A^T is generated
*/
/* polyspace +3 CERT-C:DCL15-C [Justified:]"Not static for benchmarking */
/* polyspace +2 CERT-C:DCL19-C [Justified:]"Not static for benchmarking */
/* polyspace +1 MISRA2012:8.7 [Justified:]"Not static for benchmarking */
void ML_KEM_512_Indcpa_GenMatrix(polyvec512 *a, const uint8 seed[ML_KEM_SYMBYTES], uint8 transposed)
{
  uint8 i       = 0;
  uint8 j       = 0;
  uint16 buflen = 0;
  uint16 ctr    = 0;
  /* polyspace +2 MISRA2012:2.2 [Justified:]"Calculation is important if defines should change 
    and therefore not dead code" */
  uint8 buf[(GEN_MATRIX_NBLOCKS * XOF_BLOCKBYTES) + 2u] = {0};
  xof_state state;

  for (i = 0; i < ML_KEM_512_K; i++)
  {
    for (j = 0; j < ML_KEM_512_K; j++)
    {
      if (transposed > 0u)
      {
        ML_KEM_Shake128_Absorb(&state, seed, i, j);
      }
      else
      {
        ML_KEM_Shake128_Absorb(&state, seed, j, i);
      }

      FsmSw_Fips202_Shake128_SqueezeBlocks(buf, GEN_MATRIX_NBLOCKS, &state);
      /* polyspace +2 MISRA2012:2.2 [Justified:]"Calculation is important if defines should change 
            and therefore not dead code" */
      buflen = GEN_MATRIX_NBLOCKS * XOF_BLOCKBYTES;
      ctr    = ml_kem_512_RejUniform(a[i].vec[j].coeffs, ML_KEM_N, buf, buflen);

      while (ctr < ML_KEM_N)
      {
        FsmSw_Fips202_Shake128_SqueezeBlocks(buf, 1u, &state);
        buflen = XOF_BLOCKBYTES;
        ctr    = ctr + (ml_kem_512_RejUniform(&(a[i].vec[j].coeffs[ctr]), ML_KEM_N - ctr, buf, buflen));
      }
    }
  }
} // end: ML_KEM_512_Indcpa_GenMatrix

/*====================================================================================================================*/
/**
* \brief Generates public and private key for the CPA-secure
*        public-key encryption scheme underlying ML_KEM
*
* \param[out] uint8 *pk : pointer to output public key (of length ML_KEM_512_INDCPA_PUBLICKEYBYTES bytes)
* \param[out] uint8 *sk : pointer to output private key (of length ML_KEM_512_INDCPA_SECRETKEYBYTES bytes)
*/
void ML_KEM_512_Indcpa_KeyPair(uint8 pk[ML_KEM_512_INDCPA_PUBLICKEYBYTES], uint8 sk[ML_KEM_512_INDCPA_SECRETKEYBYTES])
{
  uint8 i                        = 0;
  uint8 buf[2u * ML_KEM_SYMBYTES] = {0};
  const uint8 *const publicseed  = buf;
  const uint8 *const noiseseed   = &buf[ML_KEM_SYMBYTES];
  uint8 nonce                    = 0;

  polyvec512 a[ML_KEM_512_K], e, pkpv, skpv;

  (void)FsmSw_CommonLib_RandomBytes(buf, ML_KEM_SYMBYTES);
  // Domain separation byte
  buf[ML_KEM_SYMBYTES] = ML_KEM_512_K;
  FsmSw_Fips202_Sha3_512(buf, buf, ML_KEM_SYMBYTES + 1);

  ML_KEM_512_Indcpa_GenMatrix(a, publicseed, 0);

  for (i = 0; i < ML_KEM_512_K; i++)
  {
    ML_KEM_512_Poly_GetNoiseEta1(&skpv.vec[i], noiseseed, nonce);
    nonce++;
  }

  for (i = 0; i < ML_KEM_512_K; i++)
  {
    ML_KEM_512_Poly_GetNoiseEta1(&e.vec[i], noiseseed, nonce);
    nonce++;
  }

  ML_KEM_512_Polyvec_Ntt(&skpv);
  ML_KEM_512_Polyvec_Ntt(&e);

  // matrix-vector multiplication
  for (i = 0; i < ML_KEM_512_K; i++)
  {
    ML_KEM_512_Polyvec_BasemulAccMontgomery(&pkpv.vec[i], &a[i], &skpv);
    ML_KEM_Poly_Tomont(&pkpv.vec[i]);
  }

  ML_KEM_512_Polyvec_Add(&pkpv, &pkpv, &e);
  ML_KEM_512_Polyvec_Reduce(&pkpv);

  ml_kem_512_PackSk(sk, &skpv);
  ml_kem_512_PackPk(pk, &pkpv, publicseed);
} // end: ML_KEM_512_Indcpa_KeyPair

/*====================================================================================================================*/
/**
* \brief Encryption function of the CPA-secure
*        public-key encryption scheme underlying ML_KEM.
*
* \param[out] uint8           *c : pointer to output ciphertext (of length ML_KEM_512_INDCPA_BYTES bytes)
* \param[in]  const uint8     *m : pointer to input message (of length ML_KEM_512_INDCPA_MSGBYTES bytes)
* \param[in]  const uint8    *pk : pointer to input public key (of length ML_KEM_512_INDCPA_PUBLICKEYBYTES bytes)
* \param[in]  const uint8 *coins : pointer to input random coins used as seed
*                                  (of length ML_KEM_SYMBYTES bytes) to deterministically
*                                  generate all randomness
*/
void ML_KEM_512_Indcpa_Enc(uint8 c[ML_KEM_512_INDCPA_BYTES], const uint8 m[ML_KEM_512_INDCPA_MSGBYTES],
                               const uint8 pk[ML_KEM_512_INDCPA_PUBLICKEYBYTES], const uint8 coins[ML_KEM_SYMBYTES])
{
  uint8 i                    = 0;
  uint8 seed[ML_KEM_SYMBYTES] = {0};
  uint8 nonce                = 0;
  polyvec512 sp              = {{{{0}}}};
  polyvec512 pkpv            = {{{{0}}}};
  polyvec512 ep              = {{{{0}}}};
  polyvec512 at[ML_KEM_512_K]  = {{{{{0}}}}};
  polyvec512 b               = {{{{0}}}};
  poly v                     = {{0}};
  poly k                     = {{0}};
  poly epp                   = {{0}};

  ml_kem_512_UnpackPk(&pkpv, seed, pk);
  ML_KEM_512_Poly_FromMsg(&k, m);
  ML_KEM_512_Indcpa_GenMatrix(at, seed, 1);

  for (i = 0; i < ML_KEM_512_K; i++)
  {
    ML_KEM_512_Poly_GetNoiseEta1(&(sp.vec[i]), coins, nonce);
    nonce++;
  }

  for (i = 0; i < ML_KEM_512_K; i++)
  {
    ML_KEM_512_Poly_GetNoiseEta2(&(ep.vec[i]), coins, nonce);
    nonce++;
  }

  ML_KEM_512_Poly_GetNoiseEta2(&epp, coins, nonce);

  ML_KEM_512_Polyvec_Ntt(&sp);

  // matrix-vector multiplication
  for (i = 0; i < ML_KEM_512_K; i++)
  {
    ML_KEM_512_Polyvec_BasemulAccMontgomery(&b.vec[i], &at[i], &sp);
  }

  ML_KEM_512_Polyvec_BasemulAccMontgomery(&v, &pkpv, &sp);

  ML_KEM_512_Polyvec_InvnttTomont(&b);
  ML_KEM_Poly_InvnttTomont(&v);

  ML_KEM_512_Polyvec_Add(&b, &b, &ep);
  ML_KEM_Poly_Add(&v, &v, &epp);
  ML_KEM_Poly_Add(&v, &v, &k);
  ML_KEM_512_Polyvec_Reduce(&b);
  ML_KEM_Poly_Reduce(&v);

  ml_kem_512_PackCiphertext(c, &b, &v);
} // end: ML_KEM_512_Indcpa_Enc

/*====================================================================================================================*/
/**
* \brief Decryption function of the CPA-secure
*        public-key encryption scheme underlying ML_KEM.
*
* \param[out] uint8        *m : pointer to output decrypted message (of length ML_KEM_512_INDCPA_MSGBYTES bytes)
* \param[in]  const uint8  *c : pointer to input ciphertext (of length ML_KEM_512_INDCPA_BYTES bytes)
* \param[in]  const uint8 *sk : pointer to input secret key (of length ML_KEM_512_INDCPA_SECRETKEYBYTES bytes)
*/
void ML_KEM_512_Indcpa_Dec(uint8 m[ML_KEM_512_INDCPA_MSGBYTES], const uint8 c[ML_KEM_512_INDCPA_BYTES],
                               const uint8 sk[ML_KEM_512_INDCPA_SECRETKEYBYTES])
{
  polyvec512 b    = {{{{0}}}};
  polyvec512 skpv = {{{{0}}}};
  poly v          = {{0}};
  poly mp         = {{0}};

  ml_kem_512_UnpackCiphertext(&b, &v, c);
  ml_kem_512_UnpackSk(&skpv, sk);

  ML_KEM_512_Polyvec_Ntt(&b);
  ML_KEM_512_Polyvec_BasemulAccMontgomery(&mp, &skpv, &b);
  ML_KEM_Poly_InvnttTomont(&mp);

  ML_KEM_Poly_Sub(&mp, &v, &mp);
  ML_KEM_Poly_Reduce(&mp);

  ML_KEM_512_Poly_ToMsg(m, &mp);
} // end: ML_KEM_512_Indcpa_Dec

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */