/***********************************************************************************************************************
 *
 * Original implementation: PQClean, HQC
 *
 * Copyright 2026 IAV GmbH
 *
 * The upstream PQClean repository identifies the original HQC
 * implementation as "Public Domain". No complete upstream license text
 * or explicit CC0 reference is provided.
 * IAV modifications are licensed under the Apache License, Version 2.0.
 *
 * SPDX-License-Identifier: LicenseRef-PQClean-HQC-Public-Domain AND Apache-2.0
 *
 **********************************************************************************************************************/

/** \addtogroup SwC Hqc
*    includes the modules for SwC Hqc
 ** @{ */
/** \addtogroup Hqc192
*    includes the modules for Hqc192
 ** @{ */
/** \addtogroup Hqc192_code
 ** @{ */

/*====================================================================================================================*/
/** \file Hqc192_hqc.c
* \brief  Implementation of hqc.h
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
#include "Hqc192_code.h"
#include "Hqc192_gf2x.h"
#include "Hqc192_parameters.h"
#include "Hqc192_parsing.h"
#include "Hqc192_shake_prng.h"
#include "Hqc192_vector.h"
#include "Platform_Types.h"

#include "Hqc192_hqc.h"

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
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
* \brief Keygen of the HQC_PKE IND_CPA scheme
*
* The public key is composed of the syndrome <b>s</b> as well as the <b>seed</b> used to generate the vector <b>h</b>.
*
* The secret key is composed of the <b>seed</b> used to generate vectors <b>x</b> and  <b>y</b>.
* As a technicality, the public key is appended to the secret key in order to respect NIST API.
*
* \param[out]   pk String containing the public key
* \param[out]   sk String containing the secret key
*
*/
void Hqc192_Pke_Keygen(uint8 *const pk, uint8 *const sk)
{
  hqc192_seedexpander_state sk_seedexpander;
  hqc192_seedexpander_state pk_seedexpander;
  uint8 sk_seed[HQC192_SEED_BYTES]     = {0};
  uint8 sigma[HQC192_VEC_K_SIZE_BYTES] = {0};
  uint8 pk_seed[HQC192_SEED_BYTES]     = {0};
  uint64 x[HQC192_VEC_N_SIZE_64]       = {0};
  uint64 y[HQC192_VEC_N_SIZE_64]       = {0};
  uint64 h[HQC192_VEC_N_SIZE_64]       = {0};
  uint64 s[HQC192_VEC_N_SIZE_64]       = {0};

  // Create seed_expanders for public key and secret key
  (void)FsmSw_CommonLib_RandomBytes(sk_seed, HQC192_SEED_BYTES);
  (void)FsmSw_CommonLib_RandomBytes(sigma, HQC192_VEC_K_SIZE_BYTES);
  Hqc192_SeedExpander_Init(&sk_seedexpander, sk_seed, HQC192_SEED_BYTES);

  (void)FsmSw_CommonLib_RandomBytes(pk_seed, HQC192_SEED_BYTES);
  Hqc192_SeedExpander_Init(&pk_seedexpander, pk_seed, HQC192_SEED_BYTES);

  // Compute secret key
  Hqc192_Vect_Set_Random_Fixed_Weight(&sk_seedexpander, x, HQC192_PARAM_OMEGA);
  Hqc192_Vect_Set_Random_Fixed_Weight(&sk_seedexpander, y, HQC192_PARAM_OMEGA);

  // Compute public key
  Hqc192_Vect_Set_Random(&pk_seedexpander, h);
  Hqc192_Vect_Mul(s, y, h);
  Hqc192_Vect_Add(s, x, s, HQC192_VEC_N_SIZE_64);

  // Parse keys to string
  Hqc192_Public_Key_To_String(pk, pk_seed, s);
  Hqc192_Secret_Key_To_String(sk, sk_seed, sigma, pk);
} // end: Hqc192_Pke_Keygen

/*====================================================================================================================*/
/**
* \brief Encryption of the HQC_PKE IND_CPA scheme
*
* The cihertext is composed of vectors <b>u</b> and <b>v</b>.
*
* \param[out]   u Vector u (first part of the ciphertext)
* \param[out]   v Vector v (second part of the ciphertext)
* \param[in]    m Vector representing the message to encrypt
* \param[in]    theta Seed used to derive randomness required for encryption
* \param[in]    pk String containing the public key
*
*/
void Hqc192_Pke_Encrypt(uint64 *const u, uint64 *const v, const uint8 *const m, const uint8 *const theta,
                              const uint8 *const pk)
{
  hqc192_seedexpander_state vec_seedexpander;
  uint64 h[HQC192_VEC_N_SIZE_64]    = {0};
  uint64 s[HQC192_VEC_N_SIZE_64]    = {0};
  uint64 r1[HQC192_VEC_N_SIZE_64]   = {0};
  uint64 r2[HQC192_VEC_N_SIZE_64]   = {0};
  uint64 e[HQC192_VEC_N_SIZE_64]    = {0};
  uint64 tmp1[HQC192_VEC_N_SIZE_64] = {0};
  uint64 tmp2[HQC192_VEC_N_SIZE_64] = {0};

  // Create seed_expander from theta
  Hqc192_SeedExpander_Init(&vec_seedexpander, theta, HQC192_SEED_BYTES);

  // Retrieve h and s from public key
  Hqc192_Public_Key_From_String(h, s, pk);

  // Generate r1, r2 and e
  Hqc192_Vect_Set_Random_Fixed_Weight(&vec_seedexpander, r1, HQC192_PARAM_OMEGA_R);
  Hqc192_Vect_Set_Random_Fixed_Weight(&vec_seedexpander, r2, HQC192_PARAM_OMEGA_R);
  Hqc192_Vect_Set_Random_Fixed_Weight(&vec_seedexpander, e, HQC192_PARAM_OMEGA_E);

  // Compute u = r1 + r2.h
  Hqc192_Vect_Mul(u, r2, h);
  Hqc192_Vect_Add(u, r1, u, HQC192_VEC_N_SIZE_64);

  // Compute v = m.G by encoding the message
  Hqc192_Code_Encode(v, m);
  Hqc192_Vect_Resize(tmp1, HQC192_PARAM_N, v, HQC192_PARAM_N1N2);

  // Compute v = m.G + s.r2 + e
  Hqc192_Vect_Mul(tmp2, r2, s);
  Hqc192_Vect_Add(tmp2, e, tmp2, HQC192_VEC_N_SIZE_64);
  Hqc192_Vect_Add(tmp2, tmp1, tmp2, HQC192_VEC_N_SIZE_64);
  Hqc192_Vect_Resize(v, HQC192_PARAM_N1N2, tmp2, HQC192_PARAM_N);
} // end: Hqc192_Pke_Encrypt

/*====================================================================================================================*/
/**
* \brief Decryption of the HQC_PKE IND_CPA scheme
*
* \param[out]   m Vector representing the decrypted message
* \param[in]    u Vector u (first part of the ciphertext)
* \param[in]    v Vector v (second part of the ciphertext)
* \param[in]    sk String containing the secret key
* \returns 0
*
*/
uint8 Hqc192_Pke_Decrypt(uint8 *const m, uint8 *const sigma, const uint64 *const u, const uint64 *const v,
                               const uint8 *const sk)
{
  uint64 x[HQC192_VEC_N_SIZE_64]    = {0};
  uint64 y[HQC192_VEC_N_SIZE_64]    = {0};
  uint8 pk[HQC192_PUBLIC_KEY_BYTES] = {0};
  uint64 tmp1[HQC192_VEC_N_SIZE_64] = {0};
  uint64 tmp2[HQC192_VEC_N_SIZE_64] = {0};

  // Retrieve x, y, pk from secret key
  Hqc192_Secret_Key_From_String(x, y, sigma, pk, sk);

  // Compute v - u.y
  Hqc192_Vect_Resize(tmp1, HQC192_PARAM_N, v, HQC192_PARAM_N1N2);
  Hqc192_Vect_Mul(tmp2, y, u);
  Hqc192_Vect_Add(tmp2, tmp1, tmp2, HQC192_VEC_N_SIZE_64);

  // Compute m by decoding v - u.y
  Hqc192_Code_Decode(m, tmp2);

  return 0;
} // end: Hqc192_Pke_Decrypt

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */