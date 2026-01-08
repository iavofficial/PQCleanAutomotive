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
/** \addtogroup FsmSw_Hqc256_parsing
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Hqc256_parsing.c
* \brief Functions to parse secret key, public key and ciphertext of the HQC scheme
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
#include "FsmSw_Hqc256_parameters.h"
#include "FsmSw_Hqc256_vector.h"
#include "Platform_Types.h"

#include "FsmSw_Hqc256_parsing.h"
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

/*====================================================================================================================*/
static uint64 load8_256(const uint8 *const in)
{
  uint64 ret = in[7];

  for (sint8 i = 6; i >= 0; --i)
  {
    ret <<= 8;
    ret |= in[i];
  }

  return ret;
} // end: load8_256

/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/

/*====================================================================================================================*/
void FsmSw_Hqc256_Load8_Arr(uint64 *const out64, uint32 outlen, const uint8 *const in8, uint32 inlen)
{
  uint32 index_in  = 0;
  uint32 index_out = 0;

  // first copy by 8 bytes
  if ((inlen >= 8) && (outlen >= 1))
  {
    while ((index_out < outlen) && ((index_in + 8) <= inlen))
    {
      out64[index_out] = load8_256(&in8[index_in]);

      index_in += 8;
      index_out += 1;
    }
  }

  // we now need to do the last 7 bytes if necessary
  if (!((index_in >= inlen) || (index_out >= outlen)))
  {
    out64[index_out] = in8[inlen - 1];
    for (uint32 i = inlen - index_in; i >= 2; --i)
    {
      out64[index_out] <<= 8;
      out64[index_out] |= in8[index_in + i - 2U];
    }
  }
} // end: FsmSw_Hqc256_Load8_Arr

/*====================================================================================================================*/
void FsmSw_Hqc256_Store8_Arr(uint8 *const out8, uint32 outlen, const uint64 *const in64, uint32 inlen)
{
  uint32 index_in = 0;
  for (uint32 index_out = 0; (index_out < outlen) && (index_in < inlen); ++index_out)
  {
    out8[index_out] = (uint8)((in64[index_in] >> ((index_out % 8) * 8)) & 0xFFU);
    if ((index_out % 8) == 7)
    {
      ++index_in;
    }
  }
} // end: FsmSw_Hqc256_Store8_Arr

/*====================================================================================================================*/
/**
 * \brief Parse a secret key into a string
 *
 * The secret key is composed of the seed used to generate vectors <b>x</b> and <b>y</b>.
 * As technicality, the public key is appended to the secret key in order to respect NIST API.
 *
 * \param[out] sk String containing the secret key
 * \param[in] sk_seed Seed used to generate the secret key
 * \param[in] sigma String used in HHK transform
 * \param[in] pk String containing the public key
 */
void FsmSw_Hqc256_Secret_Key_To_String(uint8 *const sk, const uint8 *const sk_seed, const uint8 *const sigma,
                                       const uint8 *const pk)
{
  FsmSw_CommonLib_MemCpy(sk, sk_seed, HQC256_SEED_BYTES);
  FsmSw_CommonLib_MemCpy(&sk[HQC256_SEED_BYTES], sigma, HQC256_VEC_K_SIZE_BYTES);
  FsmSw_CommonLib_MemCpy(&sk[HQC256_SEED_BYTES + HQC256_VEC_K_SIZE_BYTES], pk, HQC256_PUBLIC_KEY_BYTES);
} // end: FsmSw_Hqc256_Secret_Key_To_String

/*====================================================================================================================*/
/**
 * \brief Parse a secret key from a string
 *
 * The secret key is composed of the seed used to generate vectors <b>x</b> and <b>y</b>.
 * As technicality, the public key is appended to the secret key in order to respect NIST API.
 *
 * \param[out] x uint64 representation of vector x
 * \param[out] y uint64 representation of vector y
 * \param[out] pk String containing the public key
 * \param[in] sk String containing the secret key
 */
void FsmSw_Hqc256_Secret_Key_From_String(uint64 *const x, uint64 *const y, uint8 *const sigma, uint8 *const pk,
                                         const uint8 *const sk)
{
  hqc256_seedexpander_state sk_seedexpander;

  FsmSw_CommonLib_MemCpy(sigma, &sk[HQC256_SEED_BYTES], HQC256_VEC_K_SIZE_BYTES);
  FsmSw_Hqc256_SeedExpander_Init(&sk_seedexpander, sk, HQC256_SEED_BYTES);

  FsmSw_Hqc256_Vect_Set_Random_Fixed_Weight(&sk_seedexpander, x, HQC256_PARAM_OMEGA);
  FsmSw_Hqc256_Vect_Set_Random_Fixed_Weight(&sk_seedexpander, y, HQC256_PARAM_OMEGA);
  FsmSw_CommonLib_MemCpy(pk, &sk[HQC256_SEED_BYTES + HQC256_VEC_K_SIZE_BYTES], HQC256_PUBLIC_KEY_BYTES);
} // end: FsmSw_Hqc256_Secret_Key_From_String

/*====================================================================================================================*/
/**
 * \brief Parse a public key into a string
 *
 * The public key is composed of the syndrome <b>s</b> as well as the seed used to generate the vector <b>h</b>
 *
 * \param[out] pk String containing the public key
 * \param[in] pk_seed Seed used to generate the public key
 * \param[in] s uint64 representation of vector s
 */
void FsmSw_Hqc256_Public_Key_To_String(uint8 *const pk, const uint8 *const pk_seed, const uint64 *const s)
{
  FsmSw_CommonLib_MemCpy(pk, pk_seed, HQC256_SEED_BYTES);
  FsmSw_Hqc256_Store8_Arr(&pk[HQC256_SEED_BYTES], HQC256_VEC_N_SIZE_BYTES, s, HQC256_VEC_N_SIZE_64);
} // end: FsmSw_Hqc256_Public_Key_To_String

/*====================================================================================================================*/
/**
 * \brief Parse a public key from a string
 *
 * The public key is composed of the syndrome <b>s</b> as well as the seed used to generate the vector <b>h</b>
 *
 * \param[out] h uint64 representation of vector h
 * \param[out] s uint64 representation of vector s
 * \param[in] pk String containing the public key
 */
void FsmSw_Hqc256_Public_Key_From_String(uint64 *const h, uint64 *const s, const uint8 *const pk)
{
  hqc256_seedexpander_state pk_seedexpander;

  FsmSw_Hqc256_SeedExpander_Init(&pk_seedexpander, pk, HQC256_SEED_BYTES);
  FsmSw_Hqc256_Vect_Set_Random(&pk_seedexpander, h);

  FsmSw_Hqc256_Load8_Arr(s, HQC256_VEC_N_SIZE_64, &pk[HQC256_SEED_BYTES], HQC256_VEC_N_SIZE_BYTES);
} // end: FsmSw_Hqc256_Public_Key_From_String

/*====================================================================================================================*/
/**
 * \brief Parse a ciphertext into a string
 *
 * The ciphertext is composed of vectors <b>u</b>, <b>v</b> and salt.
 *
 * \param[out] ct String containing the ciphertext
 * \param[in] u uint64 representation of vector u
 * \param[in] v uint64 representation of vector v
 * \param[in] salt String containing a salt
 */
void FsmSw_Hqc256_Ciphertext_To_String(uint8 *const ct, const uint64 *const u, const uint64 *const v,
                                       const uint8 *const salt)
{
  FsmSw_Hqc256_Store8_Arr(ct, HQC256_VEC_N_SIZE_BYTES, u, HQC256_VEC_N_SIZE_64);
  FsmSw_Hqc256_Store8_Arr(&ct[HQC256_VEC_N_SIZE_BYTES], HQC256_VEC_N1N2_SIZE_BYTES, v, HQC256_VEC_N1N2_SIZE_64);
  FsmSw_CommonLib_MemCpy(&ct[HQC256_VEC_N_SIZE_BYTES + HQC256_VEC_N1N2_SIZE_BYTES], salt, HQC256_SALT_SIZE_BYTES);
} // end: FsmSw_Hqc256_Ciphertext_To_String

/*====================================================================================================================*/
/**
 * \brief Parse a ciphertext from a string
 *
 * The ciphertext is composed of vectors <b>u</b>, <b>v</b> and salt.
 *
 * \param[out] u uint64 representation of vector u
 * \param[out] v uint64 representation of vector v
 * \param[out] d String containing the hash d
 * \param[in] ct String containing the ciphertext
 */
void FsmSw_Hqc256_Ciphertext_From_String(uint64 *const u, uint64 *const v, uint8 *const salt, const uint8 *const ct)
{
  FsmSw_Hqc256_Load8_Arr(u, HQC256_VEC_N_SIZE_64, ct, HQC256_VEC_N_SIZE_BYTES);
  FsmSw_Hqc256_Load8_Arr(v, HQC256_VEC_N1N2_SIZE_64, &ct[HQC256_VEC_N_SIZE_BYTES], HQC256_VEC_N1N2_SIZE_BYTES);
  FsmSw_CommonLib_MemCpy(salt, &ct[HQC256_VEC_N_SIZE_BYTES + HQC256_VEC_N1N2_SIZE_BYTES], HQC256_SALT_SIZE_BYTES);
} // end: FsmSw_Hqc256_Ciphertext_From_String

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */