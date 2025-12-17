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
/** \addtogroup FsmSw_Hqc192_reed_muller
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Hqc192_reed_muller.c
* \brief  Constant time implementation of Reed-Muller code RM(1,7)
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
#include "FsmSw_Hqc192_parameters.h"
#include "Platform_Types.h"

#include "FsmSw_Hqc192_reed_muller.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

// number of repeated code words
#define MULTIPLICITY CEIL_DIVIDE(HQC192_PARAM_N2, 128)

/* polyspace +5 MISRA2012:D4.9 [Justified:]"No refactoring of macros, as converting to, for example, 
inline functions would not provide significant benefits." */
/* polyspace +3 CERT-C:PRE00-C [Justified:]"No refactoring of macros, as converting to, for example, 
inline functions would not provide significant benefits." */
// copy bit 0 into all bits of a 32 bit value
#define BIT0MASK(x) (uint32)((((x) & 1U) == 0) ? 0x00000000U : 0xFFFFFFFFU)
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
* \brief Encode a single byte into a single codeword using RM(1,7)
*
* Encoding matrix of this code:
* bit pattern (note that bits are numbered big endian)
* 0   aaaaaaaa aaaaaaaa aaaaaaaa aaaaaaaa
* 1   cccccccc cccccccc cccccccc cccccccc
* 2   f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
* 3   ff00ff00 ff00ff00 ff00ff00 ff00ff00
* 4   ffff0000 ffff0000 ffff0000 ffff0000
* 5   ffffffff 00000000 ffffffff 00000000
* 6   ffffffff ffffffff 00000000 00000000
* 7   ffffffff ffffffff ffffffff ffffffff
*
* \param[out]   word An RM(1,7) codeword
* \param[in]    message A message
*
*/
static void encode_192(uint64 *const cword, uint8 message)
{
  uint32 first_word;
  // bit 7 flips all the bits, do that first to save work
  first_word = BIT0MASK(message >> 7);
  // bits 0, 1, 2, 3, 4 are the same for all four longs
  // (Warning: in the bit matrix above, low bits are at the left!)
  first_word ^= BIT0MASK(message >> 0) & 0xaaaaaaaaU;
  first_word ^= BIT0MASK(message >> 1) & 0xccccccccU;
  first_word ^= BIT0MASK(message >> 2) & 0xf0f0f0f0U;
  first_word ^= BIT0MASK(message >> 3) & 0xff00ff00U;
  first_word ^= BIT0MASK(message >> 4) & 0xffff0000U;
  // we can store this in the first quarter
  cword[0] = first_word;
  // bit 5 flips entries 1 and 3. Bit 6 flips 2 and 3
  first_word ^= BIT0MASK(message >> 5);
  cword[0] |= (uint64)first_word << 32;
  first_word ^= BIT0MASK(message >> 6);
  cword[1] = (uint64)first_word << 32;
  first_word ^= BIT0MASK(message >> 5);
  cword[1] |= first_word;
} // end: encode

/*====================================================================================================================*/
/**
* \brief Hadamard transform
*
* Perform hadamard transform of src and store result in dst
* src is overwritten
*
* \param[out]   src Structure that contain the expanded codeword
* \param[out]   dst Structure that contain the expanded codeword
*
*/
static void hqc192_hadamard(uint16 src[128], uint16 dst[128])
{
  // the passes move data:
  // src -> dst -> src -> dst -> src -> dst -> src -> dst
  // using p1 and p2 alternately
  uint16 *p1 = src;
  uint16 *p2 = dst;
  uint16 *p3;
  for (uint8 pass = 0; pass < 7; ++pass)
  {
    for (uint8 i = 0; i < 64; ++i)
    {
      p2[i]      = p1[2 * i] + p1[(2 * i) + 1];
      p2[i + 64] = p1[2 * i] - p1[(2 * i) + 1];
    }
    // swap p1, p2 for next round
    p3 = p1;
    p1 = p2;
    p2 = p3;
  }
} // end: hadamard

/*====================================================================================================================*/
/**
* \brief Add multiple codewords into expanded codeword
*
* Accesses memory in order
* Note: this does not write the codewords as -1 or +1 as the green machine does
* instead, just 0 and 1 is used.
* The resulting hadamard transform has:
* all values are halved
* the first entry is 64 too high
*
* \param[out]   dest Structure that contain the expanded codeword
* \param[in]    src Structure that contain the codeword
*
*/
static void hqc192_expand_and_sum(uint16 dest[128], const uint64 src[2 * MULTIPLICITY])
{
  // start with the first copy
  for (uint8 part = 0; part < 2; ++part)
  {
    for (uint8 bit = 0; bit < 64; ++bit)
    {
      dest[(part * 64) + bit] = (uint16)(((src[part] >> bit) & 1U) & 0xFFFFU);
    }
  }
  // sum the rest of the copies
  for (uint16 copy = 1; copy < MULTIPLICITY; ++copy)
  {
    for (uint8 part = 0; part < 2; ++part)
    {
      for (uint8 bit = 0; bit < 64; ++bit)
      {
        dest[(part * 64) + bit] += (uint16)((src[(2 * copy) + part] >> bit) & 1U);
      }
    }
  }
} // end: expand_and_sum

/*====================================================================================================================*/
/**
* \brief Finding the location of the highest value
*
* This is the final step of the green machine: find the location of the highest value,
* and add 128 if the peak is positive
* if there are two identical peaks, the peak with smallest value
* in the lowest 7 bits it taken
*
* \param[in]    transform Structure that contain the expanded codeword
*
*/
static uint8 find_peaks_192(const uint16 transform[128])
{
  uint16 peak_abs = 0;
  uint16 peak     = 0;
  uint16 pos      = 0;
  uint16 t, abs_value, mask, tmp;
  for (uint16 i = 0; i < 128; ++i)
  {
    t         = transform[i];
    tmp       = ((t >> 15) == 1) ? 0xFFFFU : 0x0000U;
    abs_value = t ^ (tmp & (t ^ ((uint16)(~t) + 1U))); // t = abs(t)
    mask      = ((((uint16)(peak_abs - abs_value)) >> 15) == 1) ? 0xFFFFU : 0x0000U;
    peak ^= mask & (peak ^ t);
    pos ^= mask & (pos ^ i);
    peak_abs ^= mask & (peak_abs ^ abs_value);
  }
  // set bit 7
  tmp = ((peak >> 15) == 0) ? 0xFFFFU : 0x0000U;
  pos |= 128U & tmp;
  return (uint8)pos;
} // end: find_peaks

/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
* \brief Encodes the received word
*
* The message consists of N1 bytes each byte is encoded into PARAM_N2 bits,
* or MULTIPLICITY repeats of 128 bits
*
* \param[out]   cdw Array of size HQC192_VEC_N1N2_SIZE_64 receiving the encoded message
* \param[in]    msg Array of size HQC192_VEC_N1_SIZE_64 storing the message
*
*/
void FsmSw_Hqc192_Reed_Muller_Encode(uint64 *const cdw, const uint8 *const msg)
{
  for (uint8 i = 0; i < HQC192_VEC_N1_SIZE_BYTES; ++i)
  {
    // encode first word
    encode_192(&cdw[2 * i * MULTIPLICITY], msg[i]);
    // copy to other identical codewords
    for (uint8 copy = 1; copy < MULTIPLICITY; ++copy)
    {
      FsmSw_CommonLib_MemCpy(&cdw[(2 * i * MULTIPLICITY) + (2 * copy)], &cdw[2 * i * MULTIPLICITY], 16);
    }
  }
} // end: FsmSw_Hqc192_Reed_Muller_Encode

/*====================================================================================================================*/
/**
* \brief Decodes the received word
*
* Decoding uses fast hadamard transform, for a more complete picture on Reed-Muller decoding, see MacWilliams, Florence Jessie, and Neil James Alexander Sloane.
* The theory of error-correcting codes codes @cite macwilliams1977theory
*
* \param[out]   msg Array of size HQC192_VEC_N1_SIZE_64 receiving the decoded message
* \param[in]    cdw Array of size HQC192_VEC_N1N2_SIZE_64 storing the received word
*
*/
void FsmSw_Hqc192_Reed_Muller_Decode(uint8 *const msg, const uint64 *const cdw)
{
  uint16 expanded[128];
  uint16 transform[128];
  for (uint8 i = 0; i < HQC192_VEC_N1_SIZE_BYTES; ++i)
  {
    // collect the codewords
    hqc192_expand_and_sum(expanded, &cdw[2 * i * MULTIPLICITY]);
    // apply hadamard transform
    hqc192_hadamard(expanded, transform);
    // fix the first entry to get the half Hadamard transform
    transform[0] -= 64 * MULTIPLICITY;
    // finish the decoding
    msg[i] = find_peaks_192(transform);
  }
} // end: FsmSw_Hqc192_Reed_Muller_Decode

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */