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
/** \addtogroup FsmSw_Hqc128_gf
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Hqc128_gf.c
* \brief  Galois field implementation
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
#include "Platform_Types.h"

#include "FsmSw_Hqc128_gf.h"
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
 * \brief Computes the number of trailing zero bits.
 *
 * \returns The number of trailing zero bits in a.
 * \param[in] a An operand
 */
static uint16 hqc128_trailing_zero_bits_count(uint16 a)
{
  uint16 bit  = 0x0000;
  uint16 tmp  = 0;
  uint16 mask = 0xFFFF;
  for (uint8 i = 0; i < 14; ++i)
  {
    bit = (a >> i) & 0x0001U;
    tmp += ((1 - bit) & mask);
    mask &= (bit != 0) ? 0x0000U : 0xFFFFU;
  }
  return tmp;
} // end: uint16 trailing_zero_bits_count

/*====================================================================================================================*/
/**
 * Reduces polynomial x modulo primitive polynomial GF_POLY.
 * \returns x mod GF_POLY
 * \param[in] x Polynomial of degree less than 64
 * \param[in] deg_x The degree of polynomial x
 */
static uint16 hqc128_gf_reduce(uint64 x, uint8 deg_x)
{
  uint16 z1, z2, rmdr, dist;
  uint64 mod;
  uint64 x_tmp = x;

  // Deduce the number of steps of reduction
  const uint8 steps = CEIL_DIVIDE(deg_x - (HQC128_PARAM_M - 1), HQC128_PARAM_GF_POLY_M2);

  // Reduce
  for (uint8 i = 0; i < steps; ++i)
  {
    mod = x_tmp >> HQC128_PARAM_M;
    x_tmp &= (FsmSw_Convert_u8_to_u64(1U) << HQC128_PARAM_M) - 1;
    x_tmp ^= mod;

    z1   = 0;
    rmdr = (uint16)HQC128_PARAM_GF_POLY ^ 0x0001U;
    for (uint8 j = HQC128_PARAM_GF_POLY_WT - 2; j > 0; --j)
    {
      z2   = hqc128_trailing_zero_bits_count(rmdr);
      dist = z2 - z1;
      mod <<= dist;
      x_tmp ^= mod;
      rmdr ^= (uint16)1 << z2;
      z1 = z2;
    }
  }

  return (uint16)x_tmp;
} // end: uint16 gf_reduce

/*====================================================================================================================*/
/**
 * Carryless multiplication of two polynomials a and b.
 * polyspace +1 MISRA2012:3.1 [Justified:]"The comment is a link and therefore contains a slash" 
 * Implementation of the algorithm mul1 in https://hal.inria.fr/inria-00188261v4/document
 * with s = 2 and w = 8
 *
 * \param[out] The polynomial c = a * b
 * \param[in] a The first polynomial
 * \param[in] b The second polynomial
 */
static void gf_carryless_mul_128(uint8 c[2], uint8 a, uint8 b)
{
  uint16 h = 0, l = 0, g = 0, u[4];
  uint32 tmp1, tmp2;
  uint16 mask;
  u[0] = 0;
  u[1] = FsmSw_Convert_u8_to_u16((b & 0x7FU));
  u[2] = u[1] << 1;
  u[3] = u[2] ^ u[1];
  tmp1 = (uint32)a & (uint32)3;

  for (uint8 i = 0; i < 4; i++)
  {
    tmp2 = (uint32)(tmp1 - i);
    g ^= (uint16)((u[i] & (uint32)(0 - (1 - ((uint32)(tmp2 | (~tmp2 + 1U)) >> 31)))) & 0xFFFFU);
  }

  l = g;
  h = 0;

  for (uint8 i = 2; i < 8; i += 2)
  {
    g    = 0;
    tmp1 = FsmSw_Convert_u8_to_u32(((a >> i) & (uint8)3));
    for (uint8 j = 0; j < 4; ++j)
    {
      tmp2 = (uint32)(tmp1 - j);
      g ^= (uint16)((u[j] & (uint32)(0 - (1 - ((uint32)(tmp2 | (~tmp2 + 1U)) >> 31)))) & 0xFFFFU);
    }

    l ^= g << i;
    h ^= g >> (8 - i);
  }

  mask = (((b >> 7) & 0x01U) == 0) ? 0x0000U : 0xFFFFU;
  l ^= (FsmSw_Convert_u8_to_u16((a << 7)) & mask);
  h ^= (FsmSw_Convert_u8_to_u16((a >> 1)) & mask);

  c[0] = (uint8)l;
  c[1] = (uint8)h;
} // end: gf_carryless_mul

/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
 * Multiplies two elements of GF(2^GF_M).
 * \returns the product a*b
 * \param[in] a Element of GF(2^GF_M)
 * \param[in] b Element of GF(2^GF_M)
 */
uint16 FsmSw_Hqc128_Gf_Mul(uint16 a, uint16 b)
{
  uint8 c[2] = {0};
  gf_carryless_mul_128(c, (uint8)a, (uint8)b);
  const uint16 tmp = FsmSw_Convert_u8_to_u16(c[0]) ^ (FsmSw_Convert_u8_to_u16(c[1]) << 8);
  return hqc128_gf_reduce(tmp, 2 * (HQC128_PARAM_M - 1));
} //end: FsmSw_Hqc128_Gf_Mul

/*====================================================================================================================*/
/**
 * \brief Squares an element of GF(2^HQC128_PARAM_M).
 * \returns a^2
 * \param[in] a Element of GF(2^HQC128_PARAM_M)
 */
uint16 FsmSw_Hqc128_Gf_Square(uint16 a)
{
  uint32 b = a;
  uint32 s = b & (uint32)1;
  for (uint8 i = 1; i < HQC128_PARAM_M; ++i)
  {
    b <<= 1;
    s ^= b & ((uint32)1 << ((size_t)2 * i));
  }

  return hqc128_gf_reduce(s, 2 * (HQC128_PARAM_M - 1));
} // end: FsmSw_Hqc128_Gf_Square

/*====================================================================================================================*/
/**
 * \brief Computes the inverse of an element of GF(2^HQC128_PARAM_M),
 * using the addition chain 1 2 3 4 7 11 15 30 60 120 127 254
 * \returns the inverse of a if a != 0 or 0 if a = 0
 * \param[in] a Element of GF(2^HQC128_PARAM_M)
 */
uint16 FsmSw_Hqc128_Gf_Inverse(uint16 a)
{
  uint16 inv = a;
  uint16 tmp1, tmp2;

  inv  = FsmSw_Hqc128_Gf_Square(a);      /* a^2 */
  tmp1 = FsmSw_Hqc128_Gf_Mul(inv, a);    /* a^3 */
  inv  = FsmSw_Hqc128_Gf_Square(inv);    /* a^4 */
  tmp2 = FsmSw_Hqc128_Gf_Mul(inv, tmp1); /* a^7 */
  tmp1 = FsmSw_Hqc128_Gf_Mul(inv, tmp2); /* a^11 */
  inv  = FsmSw_Hqc128_Gf_Mul(tmp1, inv); /* a^15 */
  inv  = FsmSw_Hqc128_Gf_Square(inv);    /* a^30 */
  inv  = FsmSw_Hqc128_Gf_Square(inv);    /* a^60 */
  inv  = FsmSw_Hqc128_Gf_Square(inv);    /* a^120 */
  inv  = FsmSw_Hqc128_Gf_Mul(inv, tmp2); /* a^127 */
  inv  = FsmSw_Hqc128_Gf_Square(inv);    /* a^254 */
  return inv;
} // end: FsmSw_Hqc128_Gf_Inverse

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */