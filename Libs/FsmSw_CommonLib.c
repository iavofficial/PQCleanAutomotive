/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Libs
*    includes the modules for Libs
 ** @{ */
/** \addtogroup CommonLib
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_CommonLib.c
* \brief  description of FsmSw_CommonLib.c
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
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/
static uint32 next = 1;
/**********************************************************************************************************************/
/* GLOBAL CONSTANTS                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/
static uint8 iav_commonlib_rand_byte(void);
static uint32 iav_commonlib_rand(void);
/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
* \brief This function generates a 1 byte random value
*
* \returns a 1 byte random value
*
*/
static uint8 iav_commonlib_rand_byte(void)
{
  /* polyspace +2 MISRA2012:D1.1 [Justified:]"Generates a random byte. Float conversion is acceptable 
    due to randomness and planned future replacement of the function." */
  return (uint8)((float32)(256.0f * ((float32)((float32)iav_commonlib_rand() / 32768.0f))));
} // end: iav_commonlib_rand_byte

/*====================================================================================================================*/
/**
* \brief This function generates a 4 byte random value
* \returns a 4 byte random value
*
*/
static uint32 iav_commonlib_rand(void)
{
  next = (next * 1103515245u) + 12345u;
  return ((uint32)(next / 65536u) & (32768u - 1u));
} // end: iav_commonlib_rand
/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
* \brief This function copies n values from dest to src
*
* \param[out] void      *dest : pointer to output array destination
* \param[in]  const void *src : pointer to input array source
* \param[in]  const uint32  n : number of  values
*
*/
/* polyspace +2 CODE-METRICS:CALLING [Justified:]"[Value: 220]The increase to 220 is due 
to recent refactoring and code improvements." */
void FsmSw_CommonLib_MemCpy(void *const dest, const void *const src, const uint32 n)
{
  uint32 i = 0;
  /* polyspace +4 CERT-C:EXP36-C [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  uint8 *const destPtr      = (uint8 *)dest;
  const uint8 *const srcPtr = (const uint8 *)src;

  for (i = 0; i < n; i++)
  {
    destPtr[i] = srcPtr[i];
  }
} // end: FsmSw_CommonLib_MemCpy

/*====================================================================================================================*/
/**
* \brief This function sets n values from dest to value
*
* \param[out] void        *dest : pointer to output array destination
* \param[in]  const uint8 value : set dest array to this value
* \param[in]  const uint32    n : number of values
*
*/
void FsmSw_CommonLib_MemSet(void *const dest, const uint8 value, const uint32 n)
{
  uint32 i = 0;
  /* polyspace +4 CERT-C:EXP36-C [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  uint8 *const destPtr = (uint8 *)dest;

  for (i = 0; i < n; i++)
  {
    destPtr[i] = value;
  }
} // end: FsmSw_CommonLib_MemSet

/*====================================================================================================================*/
/**
* \brief This function compares dest to src
*
* \param[in] const void *dest : pointer to input array destination
* \param[in] const void  *src : pointer to input array source
* \param[in] const uint32   n : number of  values
*
* \returns 0 success, otherwise 1
*
*/
uint8 FsmSw_CommonLib_MemCmp(void *const dest, const void *const src, const uint32 n)
{
  uint32 i = 0;
  /* polyspace +4 CERT-C:EXP36-C [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  uint8 *const destPtr      = (uint8 *)dest;
  const uint8 *const srcPtr = (const uint8 *)src;
  uint8 retVal              = 0;

  for (i = 0; i < n; i++)
  {
    if (destPtr[i] != srcPtr[i])
    {
      retVal = 1;
    }
  }

  return retVal;
} // end: FsmSw_CommonLib_MemCmp

/*====================================================================================================================*/
/**
* \brief This function moves n values from dest to src
*
* \param[out] void      *dest : pointer to output array destination
* \param[in]  const void *src : pointer to input array source
* \param[in]  const uint32  n : number of  values
*
*/
void FsmSw_CommonLib_MemMove(void *const dest, const void *const src, const uint32 n)
{
  sint32 i = 0;
  /* polyspace +4 CERT-C:EXP36-C [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  uint8 *const destPtr      = (uint8 *)dest;
  const uint8 *const srcPtr = (const uint8 *)src;

  if (destPtr > srcPtr)
  { /* copy from right to left */
    for (i = (sint32)((sint32)n - (sint32)1u); i >= 0; i--)
    {
      destPtr[i] = srcPtr[i];
    }
  }
  else
  { /* copy from left to right */
    for (i = 0; i < (sint32)n; i++)
    {
      destPtr[i] = srcPtr[i];
    }
  }
} // end: FsmSw_CommonLib_MemMove

/*====================================================================================================================*/
/**
* \brief This function generates n random values
*
* \param[out] uint8  *output : pointer to output array with random values
* \param[in]  const uint32 n : number of random values
*
* \returns 0 (success)
*
*/
uint8 FsmSw_CommonLib_RandomBytes(uint8 *const output, const uint32 n)
{
  for (uint32 i = 0; i < n; i++)
  {
    output[i] = iav_commonlib_rand_byte();
  }

  return 0;
} // end: FsmSw_CommonLib_RandomBytes

/*====================================================================================================================*/
/**
* \brief Initialize the random function with a seed
*
* \param[in] const uint32 seed : start seed for generating random values
*
*/
void FsmSw_CommonLib_SRand(const uint32 seed)
{
  next = seed;
} // end: FsmSw_CommonLib_SRand

/*====================================================================================================================*/
/**
* \brief Returns value of uint16 type from initial uint8
*
* \param[in] value : returned with uint16 type
* 
*/
uint16 FsmSw_Convert_u8_to_u16(uint8 value)
{
  return (uint16)value;
} // end: FsmSw_Convert_u8_to_u16

/*====================================================================================================================*/
/**
* \brief Returns value of uint32 type from initial uint8
*
* \param[in] value : returned with uint32 type
* 
*/
uint32 FsmSw_Convert_u8_to_u32(uint8 value)
{
  return (uint32)value;
} // end: FsmSw_Convert_u8_to_u16

/*====================================================================================================================*/
/**
* \brief Returns value of uint32 type from initial uint16
*
* \param[in] value : returned with uint32 type
* 
*/
uint32 FsmSw_Convert_u16_to_u32(uint16 value)
{
  return (uint32)value;
} // end: FsmSw_Convert_u8_to_u16

/*====================================================================================================================*/
/**
* \brief Returns value of uint64 type from initial size_t
*
* \param[in] value : returned with uint64 type
* 
*/
uint64 FsmSw_Convert_u32_to_u64(uint32 value)
{
  return (uint64)value;
} // end: FsmSw_Convert_u8_to_u16

/*====================================================================================================================*/
/**
* \brief Returns value of uint64 type from initial uint16
*
* \param[in] value : returned with uint64 type
* 
*/
uint64 FsmSw_Convert_u16_to_u64(uint16 value)
{
  return (uint64)value;
} // end: FsmSw_Convert_u16_to_u64

/*====================================================================================================================*/
/**
* \brief Returns value of uint64 type from initial uint8
*
* \param[in] value : returned with uint64 type
* 
*/
uint64 FsmSw_Convert_u8_to_u64(uint8 value)
{
  return (uint64)value;
} // end: FsmSw_Convert_u8_to_u64

/*====================================================================================================================*/
/**
* \brief Returns value of uint32 type from initial uint64
*
* \param[in] value : returned with uint32 type
* 
*/
uint32 FsmSw_Convert_u64_to_u32(uint64 value)
{
  return (uint32)(value & 0xFFFFFFFFU);
} // end: FsmSw_Convert_u64_to_u32

/*====================================================================================================================*/
/**
* \brief Returns value of uint8 type from initial uint16
*
* \param[in] value : returned with uint8 type
* 
*/
uint8 FsmSw_Convert_u16_to_u8(uint16 value)
{
  return (uint8)(value & 0xFFU);
} // end: FsmSw_Convert_u64_to_u32

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */