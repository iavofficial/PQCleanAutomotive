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
/** \addtogroup FsmSw_Hqc128_vector
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Hqc128_vector.c
* \brief Implementation of vectors sampling and some utilities for the HQC scheme
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
#include "FsmSw_Hqc128_parameters.h"
#include "FsmSw_Hqc128_parsing.h"
#include "Platform_Types.h"

#include "FsmSw_Hqc128_vector.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/
static uint32 hqc128_m_val[75] = {
    243079, 243093, 243106, 243120, 243134, 243148, 243161, 243175, 243189, 243203, 243216, 243230, 243244,
    243258, 243272, 243285, 243299, 243313, 243327, 243340, 243354, 243368, 243382, 243396, 243409, 243423,
    243437, 243451, 243465, 243478, 243492, 243506, 243520, 243534, 243547, 243561, 243575, 243589, 243603,
    243616, 243630, 243644, 243658, 243672, 243686, 243699, 243713, 243727, 243741, 243755, 243769, 243782,
    243796, 243810, 243824, 243838, 243852, 243865, 243879, 243893, 243907, 243921, 243935, 243949, 243962,
    243976, 243990, 244004, 244018, 244032, 244046, 244059, 244073, 244087, 244101};
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
/**
 * \brief Constant-time comparison of two integers v1 and v2
 *
 * Returns 1 if v1 is equal to v2 and 0 otherwise
 * polyspace +1 MISRA2012:3.1 [Justified:]"The comment is a link and therefore contains a slash" 
 * https://gist.github.com/sneves/10845247
 *
 * \param[in] v1
 * \param[in] v2
 */
static inline uint32 hqc128_compare_u32(uint32 v1, uint32 v2)
{
  return (uint32)1 ^ ((uint32)((v1 - v2) | (v2 - v1)) >> 31);
} // end: compare_u32

/*====================================================================================================================*/
static uint64 single_bit_mask_128(uint32 pos)
{
  uint64 ret  = 0;
  uint64 mask = 1;
  uint64 tmp;

  for (uint8 i = 0; i < 64; ++i)
  {
    tmp = FsmSw_Convert_u32_to_u64(pos - i);
    /* polyspace +3 CERT-C:INT14-C [Justified:]"The current implementation has been carefully reviewed and 
    determined to be safe and reliable in this specific context. Modifying the code solely to conform to 
    the rule would provide no additional benefit and could compromise the stability of the system" */
    tmp = 0 - (1 - ((uint64)(tmp | (0 - tmp)) >> 63));
    ret |= mask & tmp;
    mask <<= 1;
  }

  return ret;
} // end: single_bit_mask

/*====================================================================================================================*/
static inline uint32 cond_sub_128(uint32 r, uint32 n)
{
  uint32 mask;
  uint32 tmp = r - n;
  mask       = 0 - (tmp >> 31);
  return tmp + (n & mask);
} // end: cond_sub

/*====================================================================================================================*/
static inline uint32 hqc128_vector_gf_reduce(uint32 a, uint16 i)
{
  uint32 q, n, r;
  q = (uint32)((((uint64)a * hqc128_m_val[i]) >> 32) & 0xFFFFFFFFU);
  n = FsmSw_Convert_u16_to_u32((uint16)HQC128_PARAM_N - i);
  r = a - (q * n);
  return cond_sub_128(r, n);
} // end: reduce

/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
 * \brief Generates a vector of a given Hamming weight
 * polyspace +1 MISRA2012:3.1 [Justified:]"The comment is a link and therefore contains a slash" 
 * Implementation of Algorithm 5 in https://eprint.iacr.org/2021/1631.pdf
 *
 * \param[in] ctx Pointer to the context of the seed expander
 * \param[in] v Pointer to an array
 * \param[in] weight Integer that is the Hamming weight
 */
void FsmSw_Hqc128_Vect_Set_Random_Fixed_Weight(hqc128_seedexpander_state *const ctx, uint64 *const v, uint16 weight)
{
  uint8 rand_bytes[4 * HQC128_PARAM_OMEGA_R] = {0}; // to be interpreted as HQC128_PARAM_OMEGA_R 32-bit unsigned ints
  uint32 support[HQC128_PARAM_OMEGA_R]       = {0};
  uint32 index_tab[HQC128_PARAM_OMEGA_R]     = {0};
  uint64 bit_tab[HQC128_PARAM_OMEGA_R]       = {0};
  uint32 pos, found, mask32, tmp;
  uint64 mask64, val;

  FsmSw_Hqc128_SeedExpander(ctx, rand_bytes, FsmSw_Convert_u16_to_u32(4 * weight));

  for (uint16 i = 0; i < weight; ++i)
  {
    support[i] = rand_bytes[4 * i];
    support[i] |= FsmSw_Convert_u8_to_u32(rand_bytes[(4 * i) + 1]) << 8;
    support[i] |= FsmSw_Convert_u8_to_u32(rand_bytes[(4 * i) + 2]) << 16;
    support[i] |= FsmSw_Convert_u8_to_u32(rand_bytes[(4 * i) + 3]) << 24;
    support[i] = (uint32)(i + hqc128_vector_gf_reduce(support[i], i)); // use constant-tme reduction
  }

  for (uint16 i = (weight - 1); i > 0; --i)
  {
    found = 0;

    for (uint16 j = i + 1; j < weight; ++j)
    {
      found |= hqc128_compare_u32(support[j], support[i]);
    }

    mask32     = 0 - found;
    support[i] = (mask32 & i) ^ (~mask32 & support[i]);
  }

  for (uint16 i = 0; i < weight; ++i)
  {
    index_tab[i] = support[i] >> 6;
    pos          = support[i] & (uint32)0x3f;
    bit_tab[i]   = single_bit_mask_128(pos); // avoid secret shift
  }

  for (uint32 i = 0; i < HQC128_VEC_N_SIZE_64; ++i)
  {
    val = 0;
    for (uint16 j = 0; j < weight; ++j)
    {
      tmp = (uint32)(i - index_tab[j]);
      /* polyspace +3 CERT-C:INT14-C [Justified:]"The current implementation has been carefully reviewed and 
      determined to be safe and reliable in this specific context. Modifying the code solely to conform to 
      the rule would provide no additional benefit and could compromise the stability of the system" */
      tmp    = (uint32)1 ^ ((uint32)(tmp | (0 - tmp)) >> 31);
      mask64 = 0 - (uint64)tmp;
      val |= (bit_tab[j] & mask64);
    }
    v[i] |= val;
  }
} // end: FsmSw_Hqc128_Vect_Set_Random_Fixed_Weight

/*====================================================================================================================*/
/**
 * \brief Generates a random vector of dimension <b>PARAM_N</b>
 *
 * This function generates a random binary vector of dimension <b>PARAM_N</b>. It generates a random
 * array of bytes using the FsmSw_Hqc128_seedexpander function, and drop the extra bits using a mask.
 *
 * \param[in] v Pointer to an array
 * \param[in] ctx Pointer to the context of the seed expander
 */
void FsmSw_Hqc128_Vect_Set_Random(hqc128_seedexpander_state *const ctx, uint64 *const v)
{
  uint8 rand_bytes[HQC128_VEC_N_SIZE_BYTES] = {0};

  FsmSw_Hqc128_SeedExpander(ctx, rand_bytes, HQC128_VEC_N_SIZE_BYTES);

  FsmSw_Hqc128_Load8_Arr(v, HQC128_VEC_N_SIZE_64, rand_bytes, HQC128_VEC_N_SIZE_BYTES);
  v[HQC128_VEC_N_SIZE_64 - 1] &= (uint64)HQC128_RED_MASK;
} // end: FsmSw_Hqc128_Vect_Set_Random

/*====================================================================================================================*/
/**
 * \brief Adds two vectors
 *
 * \param[out] o Pointer to an array that is the result
 * \param[in] v1 Pointer to an array that is the first vector
 * \param[in] v2 Pointer to an array that is the second vector
 * \param[in] size Integer that is the size of the vectors
 */
void FsmSw_Hqc128_Vect_Add(uint64 *const o, const uint64 *const v1, const uint64 *const v2, uint16 size)
{
  for (uint16 i = 0; i < size; ++i)
  {
    o[i] = v1[i] ^ v2[i];
  }
} // end: FsmSw_Hqc128_Vect_Add

/*====================================================================================================================*/
/**
 * \brief Compares two vectors
 *
 * \param[in] v1 Pointer to an array that is first vector
 * \param[in] v2 Pointer to an array that is second vector
 * \param[in] size Integer that is the size of the vectors
 * \returns 0 if the vectors are equal and 1 otherwise
 */
uint8 FsmSw_Hqc128_Vect_Compare(const uint8 *const v1, const uint8 *const v2, uint16 size)
{
  uint16 r = 0x0100;

  for (uint16 i = 0; i < size; i++)
  {
    r |= FsmSw_Convert_u8_to_u16(v1[i] ^ v2[i]);
  }

  return (uint8)(((r - 1) >> 8) & 0xFFU);
} // end: FsmSw_Hqc128_Vect_Compare

/*====================================================================================================================*/
/**
 * \brief Resize a vector so that it contains <b>size_o</b> bits
 *
 * \param[out] o Pointer to the output vector
 * \param[in] size_o Integer that is the size of the output vector in bits
 * \param[in] v Pointer to the input vector
 * \param[in] size_v Integer that is the size of the input vector in bits
 */
void FsmSw_Hqc128_Vect_Resize(uint64 *const o, uint32 size_o, const uint64 *const v, uint32 size_v)
{
  const uint64 mask = 0x7FFFFFFFFFFFFFFFU;
  uint32 val        = 0;
  if (size_o < size_v)
  {

    if ((size_o % 64) != 0)
    {
      val = 64 - (size_o % 64);
    }

    FsmSw_CommonLib_MemCpy(o, v, HQC128_VEC_N1N2_SIZE_BYTES);

    for (uint32 i = 0; i < val; ++i)
    {
      o[HQC128_VEC_N1N2_SIZE_64 - 1] &= (mask >> i);
    }
  }
  else
  {
    FsmSw_CommonLib_MemCpy(o, v, 8 * CEIL_DIVIDE(size_v, 64));
  }
} // end: FsmSw_Hqc128_Vect_Resize

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */