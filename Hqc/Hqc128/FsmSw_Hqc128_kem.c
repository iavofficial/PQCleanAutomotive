/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Hqc128
*    includes the modules for Hqc128
 ** @{ */
/** \addtogroup FsmSw_Hqc128_kem
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Hqc128_kem.c
* \brief Implementation of kem.h
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
#include "FsmSw_Fips202.h"
#include "FsmSw_Hqc128_domains.h"
#include "FsmSw_Hqc128_hqc.h"
#include "FsmSw_Hqc128_parameters.h"
#include "FsmSw_Hqc128_parsing.h"
#include "FsmSw_Hqc128_shake_ds.h"
#include "FsmSw_Hqc128_vector.h"
#include "Platform_Types.h"

#include "FsmSw_Hqc128_kem.h"
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
 * \brief Keygen of the HQC_KEM IND_CAA2 scheme
 *
 * The public key is composed of the syndrome <b>s</b> as well as the seed used to generate the vector <b>h</b>.
 *
 * The secret key is composed of the seed used to generate vectors <b>x</b> and <b>y</b>.
 * As a technicality, the public key is appended to the secret key in order to respect NIST API.
 *
 * \param[out] pk String containing the public key
 * \param[out] sk String containing the secret key
 * \returns 0 if keygen is successful
 */
sint8 FsmSw_Hqc128_Crypto_Kem_KeyPair(uint8 *const pk, uint8 *const sk)
{

  FsmSw_Hqc128_Pke_Keygen(pk, sk);
  return 0;
} // end: FsmSw_Hqc128_Crypto_Kem_KeyPair

/*====================================================================================================================*/
/**
 * \brief Encapsulation of the HQC_KEM IND_CAA2 scheme
 *
 * \param[out] ct String containing the ciphertext
 * \param[out] ss String containing the shared secret
 * \param[in] pk String containing the public key
 * \returns 0 if encapsulation is successful
 */
sint8 FsmSw_Hqc128_Crypto_Kem_Enc(uint8 *const ct, uint8 *const ss, const uint8 *const pk)
{

  uint8 theta[HQC128_SHAKE256_512_BYTES]                                                   = {0};
  uint64 u[HQC128_VEC_N_SIZE_64]                                                           = {0};
  uint64 v[HQC128_VEC_N1N2_SIZE_64]                                                        = {0};
  uint8 mc[HQC128_VEC_K_SIZE_BYTES + HQC128_VEC_N_SIZE_BYTES + HQC128_VEC_N1N2_SIZE_BYTES] = {0};
  uint8 tmp[HQC128_VEC_K_SIZE_BYTES + HQC128_PUBLIC_KEY_BYTES + HQC128_SALT_SIZE_BYTES]    = {0};
  uint8 *const m                                                                           = tmp;
  uint8 *const salt = &tmp[HQC128_VEC_K_SIZE_BYTES + HQC128_PUBLIC_KEY_BYTES];
  shake256incctx shake256state;

  // Computing m
  (void)FsmSw_CommonLib_RandomBytes(m, HQC128_VEC_K_SIZE_BYTES);

  // Computing theta
  (void)FsmSw_CommonLib_RandomBytes(salt, HQC128_SALT_SIZE_BYTES);
  FsmSw_CommonLib_MemCpy(&tmp[HQC128_VEC_K_SIZE_BYTES], pk, HQC128_PUBLIC_KEY_BYTES);
  FsmSw_Hqc128_Shake256_512_Ds(&shake256state, theta, tmp,
                               HQC128_VEC_K_SIZE_BYTES + HQC128_PUBLIC_KEY_BYTES + HQC128_SALT_SIZE_BYTES,
                               HQC128_G_FCT_DOMAIN);

  // Encrypting m
  FsmSw_Hqc128_Pke_Encrypt(u, v, m, theta, pk);

  // Computing shared secret
  FsmSw_CommonLib_MemCpy(mc, m, HQC128_VEC_K_SIZE_BYTES);
  FsmSw_Hqc128_Store8_Arr(&mc[HQC128_VEC_K_SIZE_BYTES], HQC128_VEC_N_SIZE_BYTES, u, HQC128_VEC_N_SIZE_64);
  FsmSw_Hqc128_Store8_Arr(&mc[HQC128_VEC_K_SIZE_BYTES + HQC128_VEC_N_SIZE_BYTES], HQC128_VEC_N1N2_SIZE_BYTES, v,
                          HQC128_VEC_N1N2_SIZE_64);
  FsmSw_Hqc128_Shake256_512_Ds(&shake256state, ss, mc,
                               HQC128_VEC_K_SIZE_BYTES + HQC128_VEC_N_SIZE_BYTES + HQC128_VEC_N1N2_SIZE_BYTES,
                               HQC128_K_FCT_DOMAIN);

  // Computing ciphertext
  FsmSw_Hqc128_Ciphertext_To_String(ct, u, v, salt);

  return 0;
} // end: FsmSw_Hqc128_Crypto_Kem_Enc

/*====================================================================================================================*/
/**
 * \brief Decapsulation of the HQC_KEM IND_CAA2 scheme
 *
 * \param[out] ss String containing the shared secret
 * \param[in] ct String containing the cipĥertext
 * \param[in] sk String containing the secret key
 * \returns 0 if decapsulation is successful, -1 otherwise
 */
sint8 FsmSw_Hqc128_Crypto_Kem_Dec(uint8 *const ss, const uint8 *const ct, const uint8 *const sk)
{

  uint8 result;
  uint64 u[HQC128_VEC_N_SIZE_64]         = {0};
  uint64 v[HQC128_VEC_N1N2_SIZE_64]      = {0};
  const uint8 *const pk                  = &sk[HQC128_SEED_BYTES + HQC128_VEC_K_SIZE_BYTES];
  uint8 sigma[HQC128_VEC_K_SIZE_BYTES]   = {0};
  uint8 theta[HQC128_SHAKE256_512_BYTES] = {0};
  uint64 u2[HQC128_VEC_N_SIZE_64]        = {0};
  uint64 v2[HQC128_VEC_N1N2_SIZE_64]     = {0};
  uint8 mc[HQC128_VEC_K_SIZE_BYTES + HQC128_VEC_N_SIZE_BYTES + HQC128_VEC_N1N2_SIZE_BYTES] = {0};
  uint8 tmp[HQC128_VEC_K_SIZE_BYTES + HQC128_PUBLIC_KEY_BYTES + HQC128_SALT_SIZE_BYTES]    = {0};
  uint8 *const m                                                                           = tmp;
  uint8 *const salt = &tmp[HQC128_VEC_K_SIZE_BYTES + HQC128_PUBLIC_KEY_BYTES];
  shake256incctx shake256state;

  // Retrieving u, v and d from ciphertext
  FsmSw_Hqc128_Ciphertext_From_String(u, v, salt, ct);

  // Decrypting
  result = FsmSw_Hqc128_Pke_Decrypt(m, sigma, u, v, sk);

  // Computing theta
  FsmSw_CommonLib_MemCpy(&tmp[HQC128_VEC_K_SIZE_BYTES], pk, HQC128_PUBLIC_KEY_BYTES);
  FsmSw_Hqc128_Shake256_512_Ds(&shake256state, theta, tmp,
                               HQC128_VEC_K_SIZE_BYTES + HQC128_PUBLIC_KEY_BYTES + HQC128_SALT_SIZE_BYTES,
                               HQC128_G_FCT_DOMAIN);

  // Encrypting m'
  FsmSw_Hqc128_Pke_Encrypt(u2, v2, m, theta, pk);

  // Check if c != c'
  result |= FsmSw_Hqc128_Vect_Compare((uint8 *)u, (uint8 *)u2, HQC128_VEC_N_SIZE_BYTES);
  result |= FsmSw_Hqc128_Vect_Compare((uint8 *)v, (uint8 *)v2, HQC128_VEC_N1N2_SIZE_BYTES);

  result -= 1;

  for (uint8 i = 0; i < HQC128_VEC_K_SIZE_BYTES; ++i)
  {
    mc[i] = (m[i] & result) ^ (sigma[i] & ~result);
  }

  // Computing shared secret
  FsmSw_Hqc128_Store8_Arr(&mc[HQC128_VEC_K_SIZE_BYTES], HQC128_VEC_N_SIZE_BYTES, u, HQC128_VEC_N_SIZE_64);
  FsmSw_Hqc128_Store8_Arr(&mc[HQC128_VEC_K_SIZE_BYTES + HQC128_VEC_N_SIZE_BYTES], HQC128_VEC_N1N2_SIZE_BYTES, v,
                          HQC128_VEC_N1N2_SIZE_64);
  FsmSw_Hqc128_Shake256_512_Ds(&shake256state, ss, mc,
                               HQC128_VEC_K_SIZE_BYTES + HQC128_VEC_N_SIZE_BYTES + HQC128_VEC_N1N2_SIZE_BYTES,
                               HQC128_K_FCT_DOMAIN);

  return ((result & 1U) != 0) ? (sint8)0 : (sint8)(-1);
} // end: FsmSw_Hqc128_Crypto_Kem_Dec

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */