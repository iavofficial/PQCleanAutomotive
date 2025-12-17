/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Hqc192
*    includes the modules for Hqc192
 ** @{ */
/** \addtogroup FsmSw_Hqc192_kem
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Hqc192_kem.c
* \brief  Implementation of kem.h
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
#include "FsmSw_Hqc192_domains.h"
#include "FsmSw_Hqc192_hqc.h"
#include "FsmSw_Hqc192_parameters.h"
#include "FsmSw_Hqc192_parsing.h"
#include "FsmSw_Hqc192_shake_ds.h"
#include "FsmSw_Hqc192_vector.h"
#include "Platform_Types.h"

#include "FsmSw_Hqc192_kem.h"

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
* \param[out]   pk String containing the public key
* \param[out]   sk String containing the secret key
* \returns      0 if keygen is successful
*
*/
sint8 FsmSw_Hqc192_Crypto_Kem_KeyPair(uint8 *const pk, uint8 *const sk)
{

  FsmSw_Hqc192_Pke_Keygen(pk, sk);
  return 0;
} // end: FsmSw_Hqc192_Crypto_Kem_KeyPair

/*====================================================================================================================*/
/**
* \brief Keygen of the HQC_KEM IND_CAA2 scheme
*
* The public key is composed of the syndrome <b>s</b> as well as the seed used to generate the vector <b>h</b>.
*
* The secret key is composed of the seed used to generate vectors <b>x</b> and <b>y</b>.
* As a technicality, the public key is appended to the secret key in order to respect NIST API.
*
* \param[out]   ct String containing the ciphertext
* \param[out]   ss String containing the shared secret
* \param[in]    pk String containing the public key
* \returns      0 if encapsulation is successful
*
*/
sint8 FsmSw_Hqc192_Crypto_Kem_Enc(uint8 *const ct, uint8 *const ss, const uint8 *const pk)
{

  uint8 theta[HQC192_SHAKE256_512_BYTES]                                                   = {0};
  uint64 u[HQC192_VEC_N_SIZE_64]                                                           = {0};
  uint64 v[HQC192_VEC_N1N2_SIZE_64]                                                        = {0};
  uint8 mc[HQC192_VEC_K_SIZE_BYTES + HQC192_VEC_N_SIZE_BYTES + HQC192_VEC_N1N2_SIZE_BYTES] = {0};
  uint8 tmp[HQC192_VEC_K_SIZE_BYTES + HQC192_PUBLIC_KEY_BYTES + HQC192_SALT_SIZE_BYTES]    = {0};
  uint8 *const m                                                                           = tmp;
  uint8 *const salt = &tmp[HQC192_VEC_K_SIZE_BYTES + HQC192_PUBLIC_KEY_BYTES];
  shake256incctx shake256state;

  // Computing m
  (void)FsmSw_CommonLib_RandomBytes(m, HQC192_VEC_K_SIZE_BYTES);

  // Computing theta
  (void)FsmSw_CommonLib_RandomBytes(salt, HQC192_SALT_SIZE_BYTES);
  FsmSw_CommonLib_MemCpy(&tmp[HQC192_VEC_K_SIZE_BYTES], pk, HQC192_PUBLIC_KEY_BYTES);
  FsmSw_Hqc192_Shake256_512_Ds(&shake256state, theta, tmp,
                               HQC192_VEC_K_SIZE_BYTES + HQC192_PUBLIC_KEY_BYTES + HQC192_SALT_SIZE_BYTES,
                               HQC192_G_FCT_DOMAIN);

  // Encrypting m
  FsmSw_Hqc192_Pke_Encrypt(u, v, m, theta, pk);

  // Computing shared secret
  FsmSw_CommonLib_MemCpy(mc, m, HQC192_VEC_K_SIZE_BYTES);
  FsmSw_Hqc192_Store8_Arr(&mc[HQC192_VEC_K_SIZE_BYTES], HQC192_VEC_N_SIZE_BYTES, u, HQC192_VEC_N_SIZE_64);
  FsmSw_Hqc192_Store8_Arr(&mc[HQC192_VEC_K_SIZE_BYTES + HQC192_VEC_N_SIZE_BYTES], HQC192_VEC_N1N2_SIZE_BYTES, v,
                          HQC192_VEC_N1N2_SIZE_64);
  FsmSw_Hqc192_Shake256_512_Ds(&shake256state, ss, mc,
                               HQC192_VEC_K_SIZE_BYTES + HQC192_VEC_N_SIZE_BYTES + HQC192_VEC_N1N2_SIZE_BYTES,
                               HQC192_K_FCT_DOMAIN);

  // Computing ciphertext
  FsmSw_Hqc192_Ciphertext_To_String(ct, u, v, salt);

  return 0;
} // end: FsmSw_Hqc192_Crypto_Kem_Enc

/*====================================================================================================================*/
/**
* \brief Decapsulation of the HQC_KEM IND_CAA2 scheme
*
* \param[out]   ss String containing the shared secret
* \param[in]    ct String containing the cipĥertext
* \param[in]    sk String containing the secret key
* \returns      0 if decapsulation is successful, -1 otherwise
*
*/
sint8 FsmSw_Hqc192_Crypto_Kem_Dec(uint8 *const ss, const uint8 *const ct, const uint8 *const sk)
{

  uint8 result;
  uint64 u[HQC192_VEC_N_SIZE_64]         = {0};
  uint64 v[HQC192_VEC_N1N2_SIZE_64]      = {0};
  const uint8 *const pk                  = &sk[HQC192_SEED_BYTES + HQC192_VEC_K_SIZE_BYTES];
  uint8 sigma[HQC192_VEC_K_SIZE_BYTES]   = {0};
  uint8 theta[HQC192_SHAKE256_512_BYTES] = {0};
  uint64 u2[HQC192_VEC_N_SIZE_64]        = {0};
  uint64 v2[HQC192_VEC_N1N2_SIZE_64]     = {0};
  uint8 mc[HQC192_VEC_K_SIZE_BYTES + HQC192_VEC_N_SIZE_BYTES + HQC192_VEC_N1N2_SIZE_BYTES] = {0};
  uint8 tmp[HQC192_VEC_K_SIZE_BYTES + HQC192_PUBLIC_KEY_BYTES + HQC192_SALT_SIZE_BYTES]    = {0};
  uint8 *const m                                                                           = tmp;
  uint8 *const salt = &tmp[HQC192_VEC_K_SIZE_BYTES + HQC192_PUBLIC_KEY_BYTES];
  shake256incctx shake256state;

  // Retrieving u, v and d from ciphertext
  FsmSw_Hqc192_Ciphertext_From_String(u, v, salt, ct);

  // Decrypting
  result = FsmSw_Hqc192_Pke_Decrypt(m, sigma, u, v, sk);

  // Computing theta
  FsmSw_CommonLib_MemCpy(&tmp[HQC192_VEC_K_SIZE_BYTES], pk, HQC192_PUBLIC_KEY_BYTES);
  FsmSw_Hqc192_Shake256_512_Ds(&shake256state, theta, tmp,
                               HQC192_VEC_K_SIZE_BYTES + HQC192_PUBLIC_KEY_BYTES + HQC192_SALT_SIZE_BYTES,
                               HQC192_G_FCT_DOMAIN);

  // Encrypting m'
  FsmSw_Hqc192_Pke_Encrypt(u2, v2, m, theta, pk);

  // Check if c != c'
  result |= FsmSw_Hqc192_Vect_Compare((uint8 *)u, (uint8 *)u2, HQC192_VEC_N_SIZE_BYTES);
  result |= FsmSw_Hqc192_Vect_Compare((uint8 *)v, (uint8 *)v2, HQC192_VEC_N1N2_SIZE_BYTES);

  result -= 1;

  for (uint8 i = 0; i < HQC192_VEC_K_SIZE_BYTES; ++i)
  {
    mc[i] = (m[i] & result) ^ (sigma[i] & ~result);
  }

  // Computing shared secret
  FsmSw_Hqc192_Store8_Arr(&mc[HQC192_VEC_K_SIZE_BYTES], HQC192_VEC_N_SIZE_BYTES, u, HQC192_VEC_N_SIZE_64);
  FsmSw_Hqc192_Store8_Arr(&mc[HQC192_VEC_K_SIZE_BYTES + HQC192_VEC_N_SIZE_BYTES], HQC192_VEC_N1N2_SIZE_BYTES, v,
                          HQC192_VEC_N1N2_SIZE_64);
  FsmSw_Hqc192_Shake256_512_Ds(&shake256state, ss, mc,
                               HQC192_VEC_K_SIZE_BYTES + HQC192_VEC_N_SIZE_BYTES + HQC192_VEC_N1N2_SIZE_BYTES,
                               HQC192_K_FCT_DOMAIN);

  return ((result & 1U) != 0) ? (sint8)0 : (sint8)(-1);
} // end: FsmSw_Hqc192_Crypto_Kem_Dec

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */