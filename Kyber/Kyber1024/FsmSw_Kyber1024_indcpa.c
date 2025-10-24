/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Kyber1024
*    includes the modules for Kyber1024
 ** @{ */
/** \addtogroup Kyber1024_indcpa
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Kyber1024_indcpa.c
* \brief  description of FsmSw_Kyber1024_indcpa.c
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
#include "FsmSw_Kyber1024_params.h"
#include "FsmSw_Kyber1024_poly.h"
#include "FsmSw_Kyber1024_polyvec.h"
#include "FsmSw_Kyber_ntt.h"
#include "FsmSw_Kyber_poly.h"
#include "FsmSw_Kyber_symmetric.h"
#include "FsmSw_Types.h"

#include "FsmSw_Kyber1024_indcpa.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
/* polyspace +2 MISRA2012:2.2 [Justified:]"The function is used deeper in the code for crucial calculations 
if the defines change, so it's not dead code." */
#define GEN_MATRIX_NBLOCKS (((((12u * KYBER_N) / (8u * 4096u)) / KYBER_Q) + XOF_BLOCKBYTES) / XOF_BLOCKBYTES)

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
static void fsmsw_kyber1024_PackPk(uint8 r[KYBER1024_INDCPA_PUBLICKEYBYTES], const polyvec1024 *const pk,
                                   const uint8 seed[KYBER_SYMBYTES]);
static void fsmsw_kyber1024_UnpackPk(polyvec1024 *const pk, uint8 seed[KYBER_SYMBYTES],
                                     const uint8 packedpk[KYBER1024_INDCPA_PUBLICKEYBYTES]);
static void fsmsw_kyber1024_PackSk(uint8 r[KYBER1024_INDCPA_SECRETKEYBYTES], const polyvec1024 *const sk);
static void fsmsw_kyber1024_UnpackSk(polyvec1024 *const sk, const uint8 packedsk[KYBER1024_INDCPA_SECRETKEYBYTES]);
static void fsmsw_kyber1024_PackCiphertext(uint8 r[KYBER1024_INDCPA_BYTES], const polyvec1024 *const b,
                                           const poly *const v);
static void fsmsw_kyber1024_UnpackCiphertext(polyvec1024 *const b, poly *const v,
                                             const uint8 c[KYBER1024_INDCPA_BYTES]);
static uint16 fsmsw_kyber1024_RejUniform(sint16 *const r, uint16 len, const uint8 *const buf, uint16 buflen);

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
* \brief Serialize the public key as concatenation of the
*        serialized vector of polynomials pk
*        and the public seed used to generate the matrix A.
*
* \param[out] uint8              *r : pointer to the output serialized public key
* \param[in]  const polyvec1024 *pk : pointer to the input public-key polyvec
* \param[in]  const uint8     *seed : pointer to the input public seed
*/
static void fsmsw_kyber1024_PackPk(uint8 r[KYBER1024_INDCPA_PUBLICKEYBYTES], const polyvec1024 *const pk,
                                   const uint8 seed[KYBER_SYMBYTES])
{
  uint32 i = 0;

  FsmSw_Kyber1024_Polyvec_ToBytes(r, pk);

  for (i = 0; i < KYBER_SYMBYTES; i++)
  {
    r[i + KYBER1024_POLYVECBYTES] = seed[i];
  }
} // end: fsmsw_kyber1024_PackPk

/*====================================================================================================================*/
/**
* \brief De-serialize public key from a byte array
*        approximate inverse of fsmsw_kyber1024_PackPk
*
* \param[out] polyvec1024       *pk : pointer to output public-key polynomial vector
* \param[out] uint8           *seed : pointer to output seed to generate matrix A
* \param[in]  const uint8 *packedpk : pointer to input serialized public key
*/
static void fsmsw_kyber1024_UnpackPk(polyvec1024 *const pk, uint8 seed[KYBER_SYMBYTES],
                                     const uint8 packedpk[KYBER1024_INDCPA_PUBLICKEYBYTES])
{
  uint32 i = 0;

  FsmSw_Kyber1024_Polyvec_FromBytes(pk, packedpk);

  for (i = 0; i < KYBER_SYMBYTES; i++)
  {
    seed[i] = packedpk[i + KYBER1024_POLYVECBYTES];
  }
} // end: fsmsw_kyber1024_UnpackPk

/*====================================================================================================================*/
/**
* \brief Serialize the secret key
*
* \param[out] uint8              *r : pointer to output serialized secret key
* \param[in]  const polyvec1024 *sk : pointer to input vector of polynomials (secret key)
*/
static void fsmsw_kyber1024_PackSk(uint8 r[KYBER1024_INDCPA_SECRETKEYBYTES], const polyvec1024 *const sk)
{
  FsmSw_Kyber1024_Polyvec_ToBytes(r, sk);
} // end: fsmsw_kyber1024_PackSk

/*====================================================================================================================*/
/**
* \brief De-serialize the secret key; inverse of fsmsw_kyber1024_PackSk
*
* \param[out] polyvec1024       *sk : pointer to output vector of polynomials (secret key)
* \param[in]  const uint8 *packedsk : pointer to input serialized secret key
*/
static void fsmsw_kyber1024_UnpackSk(polyvec1024 *const sk, const uint8 packedsk[KYBER1024_INDCPA_SECRETKEYBYTES])
{
  FsmSw_Kyber1024_Polyvec_FromBytes(sk, packedsk);
} // end: fsmsw_kyber1024_UnpackSk

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
static void fsmsw_kyber1024_PackCiphertext(uint8 r[KYBER1024_INDCPA_BYTES], const polyvec1024 *const b,
                                           const poly *const v)
{
  FsmSw_Kyber1024_Polyvec_Compress(r, b);
  FsmSw_Kyber1024_Poly_Compress(&r[KYBER1024_POLYVECCOMPRESSEDBYTES], v);
} // end: fsmsw_kyber1024_PackCiphertext

/*====================================================================================================================*/
/**
* \brief De-serialize and decompress ciphertext from a byte array;
*        approximate inverse of fsmsw_kyber1024_PackCiphertext
*
* \param[out] polyvec1024 *b : pointer to the output vector of polynomials b
* \param[out] poly        *v : pointer to the output polynomial v
* \param[in]  const uint8 *c : pointer to the input serialized ciphertext
*/
static void fsmsw_kyber1024_UnpackCiphertext(polyvec1024 *const b, poly *const v, const uint8 c[KYBER1024_INDCPA_BYTES])
{
  FsmSw_Kyber1024_Polyvec_Decompress(b, c);
  FsmSw_Kyber1024_Poly_Decompress(v, &c[KYBER1024_POLYVECCOMPRESSEDBYTES]);
} // end: fsmsw_kyber1024_UnpackCiphertext

/*====================================================================================================================*/
/**
* \brief Run rejection sampling on uniform random bytes to generate
*        uniform random integers mod q
*
* \param[out] sint16       *r : pointer to output buffer
* \param[in]  uint16      len : requested number of 16-bit integers (uniform mod q)
* \param[in]  const uint8 *buf: pointer to input buffer (assumed to be uniformly random bytes)
* \param[in]  uint16   buflen : length of input buffer in bytes
*
* \returns number of sampled 16-bit integers (at most len)
*/
static uint16 fsmsw_kyber1024_RejUniform(sint16 *const r, uint16 len, const uint8 *const buf, uint16 buflen)
{
  uint16 ctr  = 0;
  uint16 pos  = 0;
  uint16 val0 = 0;
  uint16 val1 = 0;

  while ((ctr < len) && ((pos + 3u) <= buflen))
  {
    val0 = (((uint16)buf[pos] >> 0u) | ((uint16)buf[pos + 1u] << 8u)) & 0xFFFu;
    val1 = (((uint16)buf[(pos + 1u)] >> 4u) | ((uint16)buf[pos + 2u] << 4u)) & 0xFFFu;
    pos  = pos + 3u;

    if (val0 < KYBER_Q)
    {
      r[ctr] = (sint16)val0;
      ctr++;
    }

    if ((ctr < len) && (val1 < KYBER_Q))
    {
      r[ctr] = (sint16)val1;
      ctr++;
    }
  }

  return ctr;
} // end: fsmsw_kyber1024_RejUniform
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
* \param[out] polyvec1024    *a : pointer to ouptput matrix A
* \param[in]  const uint8 *seed : pointer to input seed
* \param[in]  uint8  transposed : boolean deciding whether A or A^T is generated
*/
/* polyspace +3 CERT-C:DCL15-C [Justified:]"Not static for benchmarking */
/* polyspace +2 CERT-C:DCL19-C [Justified:]"Not static for benchmarking */
/* polyspace +1 MISRA2012:8.7 [Justified:]"Not static for benchmarking */
void FsmSw_Kyber1024_Indcpa_GenMatrix(polyvec1024 *a, const uint8 seed[KYBER_SYMBYTES], uint8 transposed)
{
  uint8 i       = 0;
  uint8 j       = 0;
  uint16 buflen = 0;
  uint16 off    = 0;
  uint16 ctr    = 0;
  /* polyspace +2 MISRA2012:2.2 [Justified:]"Calculation is important if defines should change 
    and therefore not dead code" */
  uint8 buf[(GEN_MATRIX_NBLOCKS * XOF_BLOCKBYTES) + 2u] = {0};
  xof_state state;

  for (i = 0; i < KYBER1024_K; i++)
  {
    for (j = 0; j < KYBER1024_K; j++)
    {
      if (transposed > 0u)
      {
        FsmSw_Kyber_Shake128_Absorb(&state, seed, i, j);
      }
      else
      {
        FsmSw_Kyber_Shake128_Absorb(&state, seed, j, i);
      }

      FsmSw_Fips202_Shake128_SqueezeBlocks(buf, GEN_MATRIX_NBLOCKS, &state);
      /* polyspace +2 MISRA2012:2.2 [Justified:]"Calculation is important if defines should change 
             and therefore not dead code" */
      buflen = GEN_MATRIX_NBLOCKS * XOF_BLOCKBYTES;
      ctr    = fsmsw_kyber1024_RejUniform(a[i].vec[j].coeffs, KYBER_N, buf, buflen);

      while (ctr < KYBER_N)
      {
        off = buflen % 3u;

        FsmSw_Fips202_Shake128_SqueezeBlocks(&buf[off], 1u, &state);
        buflen = off + XOF_BLOCKBYTES;
        ctr    = ctr + (fsmsw_kyber1024_RejUniform(&(a[i].vec[j].coeffs[ctr]), KYBER_N - ctr, buf, buflen));
      }
    }
  }
} // end: FsmSw_Kyber1024_Indcpa_GenMatrix

/*====================================================================================================================*/
/**
* \brief Generates public and private key for the CPA-secure
*        public-key encryption scheme underlying Kyber
*
* \param[out] uint8 *pk : pointer to output public key (of length KYBER1024_INDCPA_PUBLICKEYBYTES bytes)
* \param[out] uint8 *sk : pointer to output private key  (of length KYBER1024_INDCPA_SECRETKEYBYTES bytes)
*/
void FsmSw_Kyber1024_Indcpa_KeyPair(uint8 pk[KYBER1024_INDCPA_PUBLICKEYBYTES],
                                    uint8 sk[KYBER1024_INDCPA_SECRETKEYBYTES])
{
  uint8 i                        = 0;
  uint8 buf[2u * KYBER_SYMBYTES] = {0};
  const uint8 *const publicseed  = buf;
  const uint8 *const noiseseed   = &buf[KYBER_SYMBYTES];
  uint8 nonce                    = 0;

  polyvec1024 a[KYBER1024_K], e, pkpv, skpv;

  (void)FsmSw_CommonLib_RandomBytes(buf, KYBER_SYMBYTES);
  FsmSw_Fips202_Sha3_512(buf, buf, KYBER_SYMBYTES);

  FsmSw_Kyber1024_Indcpa_GenMatrix(a, publicseed, 0);

  for (i = 0; i < KYBER1024_K; i++)
  {
    FsmSw_Kyber1024_Poly_GetNoiseEta1(&skpv.vec[i], noiseseed, nonce);
    nonce++;
  }

  for (i = 0; i < KYBER1024_K; i++)
  {
    FsmSw_Kyber1024_Poly_GetNoiseEta1(&e.vec[i], noiseseed, nonce);
    nonce++;
  }

  FsmSw_Kyber1024_Polyvec_Ntt(&skpv);
  FsmSw_Kyber1024_Polyvec_Ntt(&e);

  // matrix-vector multiplication
  for (i = 0; i < KYBER1024_K; i++)
  {
    FsmSw_Kyber1024_Polyvec_BasemulAccMontgomery(&pkpv.vec[i], &a[i], &skpv);
    FsmSw_Kyber_Poly_Tomont(&pkpv.vec[i]);
  }

  FsmSw_Kyber1024_Polyvec_Add(&pkpv, &pkpv, &e);
  FsmSw_Kyber1024_Polyvec_Reduce(&pkpv);

  fsmsw_kyber1024_PackSk(sk, &skpv);
  fsmsw_kyber1024_PackPk(pk, &pkpv, publicseed);
} // end: FsmSw_Kyber1024_Indcpa_KeyPair

/*====================================================================================================================*/
/**
* \brief Encryption function of the CPA-secure
*        public-key encryption scheme underlying Kyber.
*
* \param[out] uint8           *c : pointer to output ciphertext (of length KYBER1024_INDCPA_BYTES bytes)
* \param[in]  const uint8     *m : pointer to input message (of length KYBER1024_INDCPA_MSGBYTES bytes)
* \param[in]  const uint8    *pk : pointer to input public key (of length KYBER1024_INDCPA_PUBLICKEYBYTES)
* \param[in]  const uint8 *coins : pointer to input random coins used as seed
*                                  (of length KYBER_SYMBYTES) to deterministically
*                                  generate all randomness
*/
void FsmSw_Kyber1024_Indcpa_Enc(uint8 c[KYBER1024_INDCPA_BYTES], const uint8 m[KYBER1024_INDCPA_MSGBYTES],
                                const uint8 pk[KYBER1024_INDCPA_PUBLICKEYBYTES], const uint8 coins[KYBER_SYMBYTES])
{
  uint8 i                     = 0;
  uint8 seed[KYBER_SYMBYTES]  = {0};
  uint8 nonce                 = 0;
  polyvec1024 sp              = {{{{0}}}};
  polyvec1024 pkpv            = {{{{0}}}};
  polyvec1024 ep              = {{{{0}}}};
  polyvec1024 at[KYBER1024_K] = {{{{{0}}}}};
  polyvec1024 b               = {{{{0}}}};
  poly v                      = {{0}};
  poly k                      = {{0}};
  poly epp                    = {{0}};

  fsmsw_kyber1024_UnpackPk(&pkpv, seed, pk);
  FsmSw_Kyber1024_Poly_FromMsg(&k, m);
  FsmSw_Kyber1024_Indcpa_GenMatrix(at, seed, 1);

  for (i = 0; i < KYBER1024_K; i++)
  {
    FsmSw_Kyber1024_Poly_GetNoiseEta1(&(sp.vec[i]), coins, nonce);
    nonce++;
  }

  for (i = 0; i < KYBER1024_K; i++)
  {
    FsmSw_Kyber1024_Poly_GetNoiseEta2(&(ep.vec[i]), coins, nonce);
    nonce++;
  }

  FsmSw_Kyber1024_Poly_GetNoiseEta2(&epp, coins, nonce);

  FsmSw_Kyber1024_Polyvec_Ntt(&sp);

  // matrix-vector multiplication
  for (i = 0; i < KYBER1024_K; i++)
  {
    FsmSw_Kyber1024_Polyvec_BasemulAccMontgomery(&b.vec[i], &at[i], &sp);
  }

  FsmSw_Kyber1024_Polyvec_BasemulAccMontgomery(&v, &pkpv, &sp);

  FsmSw_Kyber1024_Polyvec_InvnttTomont(&b);
  FsmSw_Kyber_Poly_InvnttTomont(&v);

  FsmSw_Kyber1024_Polyvec_Add(&b, &b, &ep);
  FsmSw_Kyber_Poly_Add(&v, &v, &epp);
  FsmSw_Kyber_Poly_Add(&v, &v, &k);
  FsmSw_Kyber1024_Polyvec_Reduce(&b);
  FsmSw_Kyber_Poly_Reduce(&v);

  fsmsw_kyber1024_PackCiphertext(c, &b, &v);
} // end: FsmSw_Kyber1024_Indcpa_Enc

/*====================================================================================================================*/
/**
* \brief Decryption function of the CPA-secure
*        public-key encryption scheme underlying Kyber.
*
* \param[out] uint8        *m :  pointer to output decrypted message (of length KYBER1024_INDCPA_MSGBYTES bytes)
* \param[in]  const uint8  *c :  pointer to input ciphertext (of length KYBER1024_INDCPA_BYTES bytes)
* \param[in]  const uint8 *sk : pointer to input secret key (of length KYBER1024_INDCPA_SECRETKEYBYTES bytes)
*/
void FsmSw_Kyber1024_Indcpa_Dec(uint8 m[KYBER1024_INDCPA_MSGBYTES], const uint8 c[KYBER1024_INDCPA_BYTES],
                                const uint8 sk[KYBER1024_INDCPA_SECRETKEYBYTES])
{
  polyvec1024 b    = {{{{0}}}};
  polyvec1024 skpv = {{{{0}}}};
  poly v           = {{0}};
  poly mp          = {{0}};

  fsmsw_kyber1024_UnpackCiphertext(&b, &v, c);
  fsmsw_kyber1024_UnpackSk(&skpv, sk);

  FsmSw_Kyber1024_Polyvec_Ntt(&b);
  FsmSw_Kyber1024_Polyvec_BasemulAccMontgomery(&mp, &skpv, &b);
  FsmSw_Kyber_Poly_InvnttTomont(&mp);

  FsmSw_Kyber_Poly_Sub(&mp, &v, &mp);
  FsmSw_Kyber_Poly_Reduce(&mp);

  FsmSw_Kyber1024_Poly_ToMsg(m, &mp);
} // end: FsmSw_Kyber1024_Indcpa_Dec

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */