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
/** \addtogroup FsmSw_Hqc256_vector
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Hqc256_vector.c
* \brief  Implementation of vectors sampling and some utilities for the HQC scheme
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
#include "FsmSw_Hqc256_parsing.h"
#include "Platform_Types.h"

#include "FsmSw_Hqc256_vector.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

static uint32 hqc256_m_val[149] = {
    74517, 74518, 74520, 74521, 74522, 74524, 74525, 74526, 74527, 74529, 74530, 74531, 74533, 74534, 74535,
    74536, 74538, 74539, 74540, 74542, 74543, 74544, 74545, 74547, 74548, 74549, 74551, 74552, 74553, 74555,
    74556, 74557, 74558, 74560, 74561, 74562, 74564, 74565, 74566, 74567, 74569, 74570, 74571, 74573, 74574,
    74575, 74577, 74578, 74579, 74580, 74582, 74583, 74584, 74586, 74587, 74588, 74590, 74591, 74592, 74593,
    74595, 74596, 74597, 74599, 74600, 74601, 74602, 74604, 74605, 74606, 74608, 74609, 74610, 74612, 74613,
    74614, 74615, 74617, 74618, 74619, 74621, 74622, 74623, 74625, 74626, 74627, 74628, 74630, 74631, 74632,
    74634, 74635, 74636, 74637, 74639, 74640, 74641, 74643, 74644, 74645, 74647, 74648, 74649, 74650, 74652,
    74653, 74654, 74656, 74657, 74658, 74660, 74661, 74662, 74663, 74665, 74666, 74667, 74669, 74670, 74671,
    74673, 74674, 74675, 74676, 74678, 74679, 74680, 74682, 74683, 74684, 74685, 74687, 74688, 74689, 74691,
    74692, 74693, 74695, 74696, 74697, 74698, 74700, 74701, 74702, 74704, 74705, 74706, 74708, 74709};

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
* \param[in]    v1
* \param[in]    v2
*
*/
static inline uint32 hqc256_compare_u32(uint32 v1, uint32 v2)
{
  return FsmSw_Convert_u8_to_u32(1) ^ ((uint32)((v1 - v2) | (v2 - v1)) >> 31);
} // end: compare_u32

static uint64 single_bit_mask_256(uint32 pos)
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

static inline uint32 cond_sub_256(uint32 r, uint32 n)
{
  uint32 mask;
  uint32 tmp = r - n;
  mask       = 0 - (tmp >> 31);
  return tmp + (n & mask);
} // end: cond_sub

static inline uint32 hqc256_vector_gf_reduce(uint32 a, uint8 i)
{
  uint32 q, n, r;
  q = (uint32)((((uint64)a * hqc256_m_val[i]) >> 32) & 0xFFFFFFFFU);
  n = FsmSw_Convert_u16_to_u32((uint16)HQC256_PARAM_N - FsmSw_Convert_u8_to_u16(i));
  r = a - (q * n);
  return cond_sub_256(r, n);
} // end: reduce

/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
* \brief Generates a vector of a given Hamming weight
*
* polyspace +1 MISRA2012:3.1 [Justified:]"The comment is a link and therefore contains a slash" 
* Implementation of Algorithm 5 in https://eprint.iacr.org/2021/1631.pdf
*
* \param[in]    ctx Pointer to the context of the seed expander
* \param[in]    v Pointer to an array
* \param[in]    weight Integer that is the Hamming weight
*
*/
void FsmSw_Hqc256_Vect_Set_Random_Fixed_Weight(hqc256_seedexpander_state *const ctx, uint64 *const v, uint16 weight)
{
  uint8 rand_bytes[4 * HQC256_PARAM_OMEGA_R] = {0}; // to be interpreted as HQC256_PARAM_OMEGA_R 32-bit unsigned ints
  uint32 support[HQC256_PARAM_OMEGA_R]       = {0};
  uint32 index_tab[HQC256_PARAM_OMEGA_R]     = {0};
  uint64 bit_tab[HQC256_PARAM_OMEGA_R]       = {0};
  uint32 pos, found, mask32, tmp;
  uint64 mask64, val;

  FsmSw_Hqc256_SeedExpander(ctx, rand_bytes, FsmSw_Convert_u16_to_u32(4 * weight));

  for (uint16 i = 0; i < weight; ++i)
  {
    support[i] = rand_bytes[4 * i];
    support[i] |= FsmSw_Convert_u8_to_u32(rand_bytes[(4 * i) + 1]) << 8;
    support[i] |= FsmSw_Convert_u8_to_u32(rand_bytes[(4 * i) + 2]) << 16;
    support[i] |= FsmSw_Convert_u8_to_u32(rand_bytes[(4 * i) + 3]) << 24;
    support[i] =
        (uint32)(i + hqc256_vector_gf_reduce(support[i], FsmSw_Convert_u16_to_u8(i))); // use constant-tme reduction
  }

  for (uint16 i = (weight - 1); i > 0; --i)
  {
    found = 0;

    for (uint16 j = i + 1; j < weight; ++j)
    {
      found |= hqc256_compare_u32(support[j], support[i]);
    }

    mask32     = 0 - found;
    support[i] = (mask32 & i) ^ (~mask32 & support[i]);
  }

  for (uint16 i = 0; i < weight; ++i)
  {
    index_tab[i] = support[i] >> 6;
    pos          = support[i] & FsmSw_Convert_u8_to_u32(0x3f);
    bit_tab[i]   = single_bit_mask_256(pos); // avoid secret shift
  }

  for (uint16 i = 0; i < HQC256_VEC_N_SIZE_64; ++i)
  {
    val = 0;
    for (uint16 j = 0; j < weight; ++j)
    {
      tmp = (uint32)(i - index_tab[j]);
      /* polyspace +3 CERT-C:INT14-C [Justified:]"The current implementation has been carefully reviewed and 
      determined to be safe and reliable in this specific context. Modifying the code solely to conform to 
      the rule would provide no additional benefit and could compromise the stability of the system" */
      tmp    = FsmSw_Convert_u8_to_u32(1) ^ ((uint32)(tmp | (0 - tmp)) >> 31);
      mask64 = 0 - (uint64)tmp;
      val |= (bit_tab[j] & mask64);
    }
    v[i] |= val;
  }
} // end: FsmSw_Hqc256_Vect_Set_Random_Fixed_Weight

/*====================================================================================================================*/
/**
* \brief Generates a random vector of dimension <b>PARAM_N</b>
*
* This function generates a random binary vector of dimension <b>PARAM_N</b>. It generates a random
* array of bytes using the FsmSw_Hqc256_seedexpander function, and drop the extra bits using a mask.
*
* \param[in]    v Pointer to an array
* \param[in]    ctx Pointer to the context of the seed expander
*
*/
void FsmSw_Hqc256_Vect_Set_Random(hqc256_seedexpander_state *const ctx, uint64 *const v)
{
  uint8 rand_bytes[HQC256_VEC_N_SIZE_BYTES] = {0};

  FsmSw_Hqc256_SeedExpander(ctx, rand_bytes, HQC256_VEC_N_SIZE_BYTES);

  FsmSw_Hqc256_Load8_Arr(v, HQC256_VEC_N_SIZE_64, rand_bytes, HQC256_VEC_N_SIZE_BYTES);
  v[HQC256_VEC_N_SIZE_64 - 1] &= (uint64)HQC256_RED_MASK;
} // end: FsmSw_Hqc256_Vect_Set_Random

/*====================================================================================================================*/
/**
* \brief Adds two vectors
*
* \param[out]   o Pointer to an array that is the result
* \param[in]    v1 Pointer to an array that is the first vector
* \param[in]    v2 Pointer to an array that is the second vector
* \param[in]    size Integer that is the size of the vectors
*
*/
void FsmSw_Hqc256_Vect_Add(uint64 *const o, const uint64 *const v1, const uint64 *const v2, uint32 size)
{
  for (uint32 i = 0; i < size; ++i)
  {
    o[i] = v1[i] ^ v2[i];
  }
} // end: FsmSw_Hqc256_Vect_Add

/*====================================================================================================================*/
/**
* \brief Compares two vectors
*
* \param[in]    v1 Pointer to an array that is the first vector
* \param[in]    v2 Pointer to an array that is the second vector
* \param[in]    size Integer that is the size of the vectors
* \returns      0 if the vectors are equal and 1 otherwise
*
*/
uint8 FsmSw_Hqc256_Vect_Compare(const uint8 *const v1, const uint8 *const v2, uint32 size)
{
  uint16 r = 0x0100;

  for (uint32 i = 0; i < size; i++)
  {
    r |= FsmSw_Convert_u8_to_u16(v1[i] ^ v2[i]);
  }

  return (uint8)(((r - 1) >> 8) & 0xFFU);
} // end: FsmSw_Hqc256_Vect_Compare

/*====================================================================================================================*/
/**
* \brief Resize a vector so that it contains <b>size_o</b> bits
*
* \param[out]   o Pointer to the output vector
* \param[in]    size_o Integer that is the size of the output vector in bits
* \param[in]    v Pointer to the input vector
* \param[in]    size_v Integer that is the size of the input vector in bits
*
*/
void FsmSw_Hqc256_Vect_Resize(uint64 *const o, uint32 size_o, const uint64 *const v, uint32 size_v)
{
  const uint64 mask = 0x7FFFFFFFFFFFFFFFU;
  uint8 val         = 0;
  if (size_o < size_v)
  {

    if ((size_o % 64) != 0)
    {
      val = (uint8)((64 - (size_o % 64)) & 0xFFU);
    }

    FsmSw_CommonLib_MemCpy(o, v, HQC256_VEC_N1N2_SIZE_BYTES);

    for (uint8 i = 0; i < val; ++i)
    {
      o[HQC256_VEC_N1N2_SIZE_64 - 1] &= (mask >> i);
    }
  }
  else
  {
    FsmSw_CommonLib_MemCpy(o, v, 8 * CEIL_DIVIDE(size_v, 64));
  }
} // end: FsmSw_Hqc256_Vect_Resize

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */