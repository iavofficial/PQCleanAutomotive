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
/** \addtogroup FsmSw_Hqc256_gf2x
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Hqc256_gf2x.c
* \brief Implementation of multiplication of two polynomials
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
#include "Platform_Types.h"

#include "FsmSw_Hqc256_gf2x.h"
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
/**
 * \brief Caryless multiplication of two words of 64 bits
 *
 * polyspace +1 MISRA2012:3.1 [Justified:]"The comment is a link and therefore contains a slash" 
 * Implemntation of the algorithm mul1 in https://hal.inria.fr/inria-00188261v4/document.
 * With w = 64 and s = 4
 *
 * \param[out] c The result c = a * b
 * \param[in] a The first value a
 * \param[in] b The second value b
 */
static void hqc256_base_mul(uint64 *const c, uint64 a, uint64 b)
{
  uint64 h = 0;
  uint64 l = 0;
  uint64 g;
  uint64 u[16]       = {0};
  uint64 mask_tab[4] = {0};
  uint64 tmp1, tmp2;

  // Step 1
  u[0]  = 0;
  u[1]  = b & (((uint64)1 << (64 - 4)) - 1);
  u[2]  = u[1] << 1;
  u[3]  = u[2] ^ u[1];
  u[4]  = u[2] << 1;
  u[5]  = u[4] ^ u[1];
  u[6]  = u[3] << 1;
  u[7]  = u[6] ^ u[1];
  u[8]  = u[4] << 1;
  u[9]  = u[8] ^ u[1];
  u[10] = u[5] << 1;
  u[11] = u[10] ^ u[1];
  u[12] = u[6] << 1;
  u[13] = u[12] ^ u[1];
  u[14] = u[7] << 1;
  u[15] = u[14] ^ u[1];

  g    = 0;
  tmp1 = a & FsmSw_Convert_u8_to_u64(0x0f);

  for (uint8 i = 0; i < 16; ++i)
  {
    tmp2 = tmp1 - i;
    /* polyspace +3 CERT-C:INT14-C [Justified:]"The current implementation has been carefully reviewed and 
    determined to be safe and reliable in this specific context. Modifying the code solely to conform to 
    the rule would provide no additional benefit and could compromise the stability of the system" */
    g ^= (u[i] & (uint64)(0 - (1 - ((uint64)(tmp2 | (0 - tmp2)) >> 63))));
  }

  l = g;
  h = 0;

  // Step 2
  for (uint8 i = 4; i < 64; i += 4)
  {
    g    = 0;
    tmp1 = (a >> i) & FsmSw_Convert_u8_to_u64(0x0f);
    for (uint8 j = 0; j < 16; ++j)
    {
      tmp2 = tmp1 - j;
      /* polyspace +3 CERT-C:INT14-C [Justified:]"The current implementation has been carefully reviewed and 
      determined to be safe and reliable in this specific context. Modifying the code solely to conform to 
      the rule would provide no additional benefit and could compromise the stability of the system" */
      g ^= (u[j] & (uint64)(0 - (1 - ((uint64)(tmp2 | (0 - tmp2)) >> 63))));
    }

    l ^= g << i;
    h ^= g >> (64 - i);
  }

  // Step 3
  mask_tab[0] = 0 - ((b >> 60) & 1U);
  mask_tab[1] = 0 - ((b >> 61) & 1U);
  mask_tab[2] = 0 - ((b >> 62) & 1U);
  mask_tab[3] = 0 - ((b >> 63) & 1U);

  l ^= ((a << 60) & mask_tab[0]);
  h ^= ((a >> 4) & mask_tab[0]);

  l ^= ((a << 61) & mask_tab[1]);
  h ^= ((a >> 3) & mask_tab[1]);

  l ^= ((a << 62) & mask_tab[2]);
  h ^= ((a >> 2) & mask_tab[2]);

  l ^= ((a << 63) & mask_tab[3]);
  h ^= ((a >> 1) & mask_tab[3]);

  c[0] = l;
  c[1] = h;
} // end: base_mul

/*====================================================================================================================*/
static void hqc256_karatsuba_add1(uint64 *const alh, uint64 *const blh, const uint64 *const a, const uint64 *const b,
                                  uint32 size_l, uint32 size_h)
{
  for (uint32 i = 0; i < size_h; ++i)
  {
    alh[i] = a[i] ^ a[i + size_l];
    blh[i] = b[i] ^ b[i + size_l];
  }

  if (size_h < size_l)
  {
    alh[size_h] = a[size_h];
    blh[size_h] = b[size_h];
  }
} // end: karatsuba_add1

/*====================================================================================================================*/
static void hqc256_karatsuba_add2(uint64 *const o, uint64 *const tmp1, const uint64 *const tmp2, uint32 size_l,
                                  uint32 size_h)
{
  for (uint32 i = 0; i < (2 * size_l); ++i)
  {
    tmp1[i] = tmp1[i] ^ o[i];
  }

  for (uint32 i = 0; i < (2 * size_h); ++i)
  {
    tmp1[i] = tmp1[i] ^ tmp2[i];
  }

  for (uint32 i = 0; i < (2 * size_l); ++i)
  {
    o[i + size_l] = o[i + size_l] ^ tmp1[i];
  }
} // end: karatsuba_add2

/*====================================================================================================================*/
/**
 * hqc256_karatsuba multiplication of a and b, Implementation inspired from the NTL library.
 *
 * \param[out] o Polynomial
 * \param[in] a Polynomial
 * \param[in] b Polynomial
 * \param[in] size Length of polynomial
 * \param[in] stack Length of polynomial
 */
static void hqc256_karatsuba(uint64 *const o, const uint64 *const a, const uint64 *const b, uint32 size, uint64 *stack)
{
  uint32 size_l, size_h;
  const uint64 *ah, *bh;

  if (size == 1)
  {
    hqc256_base_mul(o, a[0], b[0]);
  }
  else
  {
    size_h = size / 2;
    size_l = (size + 1) / 2;

    uint64 *const alh  = stack;
    uint64 *const blh  = &alh[size_l];
    uint64 *const tmp1 = &blh[size_l];
    uint64 *const tmp2 = &o[2 * size_l];
    uint64 *stack_tmp  = &stack[4 * size_l];

    ah = &a[size_l];
    bh = &b[size_l];
    /* polyspace +2 MISRA2012:17.2 [Justified:]"Without in-depth knowledge, this violation cannot be resolved." */
    /* polyspace +1 CERT-C:MEM05-C [Justified:]"Without in-depth knowledge, this violation cannot be resolved." */
    hqc256_karatsuba(o, a, b, size_l, stack_tmp);
    /* polyspace +2 MISRA2012:17.2 [Justified:]"Without in-depth knowledge, this violation cannot be resolved." */
    /* polyspace +1 CERT-C:MEM05-C [Justified:]"Without in-depth knowledge, this violation cannot be resolved." */
    hqc256_karatsuba(tmp2, ah, bh, size_h, stack_tmp);

    hqc256_karatsuba_add1(alh, blh, a, b, size_l, size_h);
    /* polyspace +2 MISRA2012:17.2 [Justified:]"Without in-depth knowledge, this violation cannot be resolved." */
    /* polyspace +1 CERT-C:MEM05-C [Justified:]"Without in-depth knowledge, this violation cannot be resolved." */
    hqc256_karatsuba(tmp1, alh, blh, size_l, stack_tmp);

    hqc256_karatsuba_add2(o, tmp1, tmp2, size_l, size_h);
  }
} // end: karatsuba

/*====================================================================================================================*/
/**
 * \brief Compute o(x) = a(x) mod \f$ X^n - 1\f$
 *
 * This function computes the modular reduction of the polynomial a(x)
 *
 * \param[in] a Pointer to the polynomial a(x)
 * \param[out] o Pointer to the result
 */
static void hqc256_gf2x_gf_reduce(uint64 *const o, const uint64 *const a)
{
  uint64 r;
  uint64 carry;

  for (uint16 i = 0; i < HQC256_VEC_N_SIZE_64; ++i)
  {
    r     = a[i + HQC256_VEC_N_SIZE_64 - 1] >> (((uint16)HQC256_PARAM_N) & FsmSw_Convert_u8_to_u16(0x3F));
    carry = a[i + HQC256_VEC_N_SIZE_64] << (64 - (((uint16)HQC256_PARAM_N) & FsmSw_Convert_u8_to_u16(0x3F)));
    o[i]  = a[i] ^ r ^ carry;
  }

  o[HQC256_VEC_N_SIZE_64 - 1] &= (uint64)HQC256_RED_MASK;
} // end: reduce

/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
 * \brief Multiply two polynomials modulo \f$ X^n - 1\f$.
 *
 * This functions multiplies polynomials <b>v1</b> and <b>v2</b>.
 * The multiplication is done modulo \f$ X^n - 1\f$.
 *
 * \param[out] o Product of <b>v1</b> and <b>v2</b>
 * \param[in] v1 Pointer to the first polynomial
 * \param[in] v2 Pointer to the second polynomial
 */
void FsmSw_Hqc256_Vect_Mul(uint64 *const o, const uint64 *const v1, const uint64 *const v2)
{
  uint64 stack[(uint32)HQC256_VEC_N_SIZE_64 << 3]   = {0};
  uint64 o_karat[(uint32)HQC256_VEC_N_SIZE_64 << 1] = {0};

  hqc256_karatsuba(o_karat, v1, v2, HQC256_VEC_N_SIZE_64, stack);
  hqc256_gf2x_gf_reduce(o, o_karat);
} // end: FsmSw_Hqc256_Vect_Mul

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */