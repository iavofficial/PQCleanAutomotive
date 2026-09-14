/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Hqc256
*    includes the modules for Hqc256
 ** @{ */
/** \addtogroup FsmSw_Hqc256_hqc
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Hqc256_hqc.c
* \brief  Implementation of FsmSw_Hqc256_hqc.h
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
#include "FsmSw_Hqc256_code.h"
#include "FsmSw_Hqc256_gf2x.h"
#include "FsmSw_Hqc256_parameters.h"
#include "FsmSw_Hqc256_parsing.h"
#include "FsmSw_Hqc256_shake_prng.h"
#include "FsmSw_Hqc256_vector.h"
#include "Platform_Types.h"

#include "FsmSw_Hqc256_hqc.h"
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
 * \param[out] pk String containing the public key
 * \param[out] sk String containing the secret key
 */
void FsmSw_Hqc256_Pke_Keygen(uint8 *const pk, uint8 *const sk)
{
  hqc256_seedexpander_state sk_seedexpander;
  hqc256_seedexpander_state pk_seedexpander;
  uint8 sk_seed[HQC256_SEED_BYTES]     = {0};
  uint8 sigma[HQC256_VEC_K_SIZE_BYTES] = {0};
  uint8 pk_seed[HQC256_SEED_BYTES]     = {0};
  uint64 x[HQC256_VEC_N_SIZE_64]       = {0};
  uint64 y[HQC256_VEC_N_SIZE_64]       = {0};
  uint64 h[HQC256_VEC_N_SIZE_64]       = {0};
  uint64 s[HQC256_VEC_N_SIZE_64]       = {0};

  // Create seed_expanders for public key and secret key
  (void)FsmSw_CommonLib_RandomBytes(sk_seed, HQC256_SEED_BYTES);
  (void)FsmSw_CommonLib_RandomBytes(sigma, HQC256_VEC_K_SIZE_BYTES);
  FsmSw_Hqc256_SeedExpander_Init(&sk_seedexpander, sk_seed, HQC256_SEED_BYTES);

  (void)FsmSw_CommonLib_RandomBytes(pk_seed, HQC256_SEED_BYTES);
  FsmSw_Hqc256_SeedExpander_Init(&pk_seedexpander, pk_seed, HQC256_SEED_BYTES);

  // Compute secret key
  FsmSw_Hqc256_Vect_Set_Random_Fixed_Weight(&sk_seedexpander, x, HQC256_PARAM_OMEGA);
  FsmSw_Hqc256_Vect_Set_Random_Fixed_Weight(&sk_seedexpander, y, HQC256_PARAM_OMEGA);

  // Compute public key
  FsmSw_Hqc256_Vect_Set_Random(&pk_seedexpander, h);
  FsmSw_Hqc256_Vect_Mul(s, y, h);
  FsmSw_Hqc256_Vect_Add(s, x, s, HQC256_VEC_N_SIZE_64);

  // Parse keys to string
  FsmSw_Hqc256_Public_Key_To_String(pk, pk_seed, s);
  FsmSw_Hqc256_Secret_Key_To_String(sk, sk_seed, sigma, pk);
} // end: FsmSw_Hqc256_Pke_Keygen

/*====================================================================================================================*/
/**
 * \brief Encryption of the HQC_PKE IND_CPA scheme
 *
 * The cihertext is composed of vectors <b>u</b> and <b>v</b>.
 *
 * \param[out] u Vector u (first part of the ciphertext)
 * \param[out] v Vector v (second part of the ciphertext)
 * \param[in] m Vector representing the message to encrypt
 * \param[in] theta Seed used to derive randomness required for encryption
 * \param[in] pk String containing the public key
 */
void FsmSw_Hqc256_Pke_Encrypt(uint64 *const u, uint64 *const v, const uint8 *const m, const uint8 *const theta,
                              const uint8 *const pk)
{
  hqc256_seedexpander_state vec_seedexpander;
  uint64 h[HQC256_VEC_N_SIZE_64]    = {0};
  uint64 s[HQC256_VEC_N_SIZE_64]    = {0};
  uint64 r1[HQC256_VEC_N_SIZE_64]   = {0};
  uint64 r2[HQC256_VEC_N_SIZE_64]   = {0};
  uint64 e[HQC256_VEC_N_SIZE_64]    = {0};
  uint64 tmp1[HQC256_VEC_N_SIZE_64] = {0};
  uint64 tmp2[HQC256_VEC_N_SIZE_64] = {0};

  // Create seed_expander from theta
  FsmSw_Hqc256_SeedExpander_Init(&vec_seedexpander, theta, HQC256_SEED_BYTES);

  // Retrieve h and s from public key
  FsmSw_Hqc256_Public_Key_From_String(h, s, pk);

  // Generate r1, r2 and e
  FsmSw_Hqc256_Vect_Set_Random_Fixed_Weight(&vec_seedexpander, r1, HQC256_PARAM_OMEGA_R);
  FsmSw_Hqc256_Vect_Set_Random_Fixed_Weight(&vec_seedexpander, r2, HQC256_PARAM_OMEGA_R);
  FsmSw_Hqc256_Vect_Set_Random_Fixed_Weight(&vec_seedexpander, e, HQC256_PARAM_OMEGA_E);

  // Compute u = r1 + r2.h
  FsmSw_Hqc256_Vect_Mul(u, r2, h);
  FsmSw_Hqc256_Vect_Add(u, r1, u, HQC256_VEC_N_SIZE_64);

  // Compute v = m.G by encoding the message
  FsmSw_Hqc256_Code_Encode(v, m);
  FsmSw_Hqc256_Vect_Resize(tmp1, HQC256_PARAM_N, v, HQC256_PARAM_N1N2);

  // Compute v = m.G + s.r2 + e
  FsmSw_Hqc256_Vect_Mul(tmp2, r2, s);
  FsmSw_Hqc256_Vect_Add(tmp2, e, tmp2, HQC256_VEC_N_SIZE_64);
  FsmSw_Hqc256_Vect_Add(tmp2, tmp1, tmp2, HQC256_VEC_N_SIZE_64);
  FsmSw_Hqc256_Vect_Resize(v, HQC256_PARAM_N1N2, tmp2, HQC256_PARAM_N);
} // end: FsmSw_Hqc256_Pke_Encrypt

/*====================================================================================================================*/
/**
 * \brief Decryption of the HQC_PKE IND_CPA scheme
 *
 * \param[out] m Vector representing the decrypted message
 * \param[in] u Vector u (first part of the ciphertext)
 * \param[in] v Vector v (second part of the ciphertext)
 * \param[in] sk String containing the secret key
 * \returns 0
 */
uint8 FsmSw_Hqc256_Pke_Decrypt(uint8 *const m, uint8 *const sigma, const uint64 *const u, const uint64 *const v,
                               const uint8 *const sk)
{
  uint64 x[HQC256_VEC_N_SIZE_64]    = {0};
  uint64 y[HQC256_VEC_N_SIZE_64]    = {0};
  uint8 pk[HQC256_PUBLIC_KEY_BYTES] = {0};
  uint64 tmp1[HQC256_VEC_N_SIZE_64] = {0};
  uint64 tmp2[HQC256_VEC_N_SIZE_64] = {0};

  // Retrieve x, y, pk from secret key
  FsmSw_Hqc256_Secret_Key_From_String(x, y, sigma, pk, sk);

  // Compute v - u.y
  FsmSw_Hqc256_Vect_Resize(tmp1, HQC256_PARAM_N, v, HQC256_PARAM_N1N2);
  FsmSw_Hqc256_Vect_Mul(tmp2, y, u);
  FsmSw_Hqc256_Vect_Add(tmp2, tmp1, tmp2, HQC256_VEC_N_SIZE_64);

  // Compute m by decoding v - u.y
  FsmSw_Hqc256_Code_Decode(m, tmp2);

  return 0;
} // end: FsmSw_Hqc256_Pke_Decrypt

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
