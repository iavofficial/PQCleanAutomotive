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
/** \addtogroup CommonLibs
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_CommonLib.h
* \brief  Declarations for the modul FsmSw_CommonLib.c
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
#ifndef FSMSW_COMMONLIB_H
#define FSMSW_COMMONLIB_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Std_Types.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define UINT32_MAXVAL ((uint32)4294967295u) /* This value is 2^32 - 1 */
#define NULL_PTR      ((void *)0)

#define ERR_OK     0u
#define ERR_NOT_OK 1u
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
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_CommonLib_MemCpy(void *const dest, const void *const src, const uint32 n);
void FsmSw_CommonLib_MemSet(void *const dest, const uint8 value, const uint32 n);
uint8 FsmSw_CommonLib_MemCmp(void *const dest, const void *const src, const uint32 n);
void FsmSw_CommonLib_MemMove(void *const dest, const void *const src, const uint32 n);
uint8 FsmSw_CommonLib_RandomBytes(uint8 *const output, const uint32 n);
void FsmSw_CommonLib_SRand(const uint32 seed);
static inline uint64 FsmSw_Convert_u32_to_u64(uint32 value);
static inline uint64 FsmSw_Convert_u16_to_u64(uint16 value);
static inline uint64 FsmSw_Convert_u8_to_u64(uint8 value);
static inline uint32 FsmSw_Convert_u8_to_u32(uint8 value);
static inline uint32 FsmSw_Convert_u64_to_u32(uint64 value);
static inline uint32 FsmSw_Convert_u16_to_u32(uint16 value);
static inline uint16 FsmSw_Convert_u8_to_u16(uint8 value);
static inline uint8 FsmSw_Convert_u16_to_u8(uint16 value);
static inline uint16 FsmSw_Convert_u32_to_u16(uint32 value);
static inline uint16 FsmSw_GetSignBit_i16(sint16 value);

/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/*====================================================================================================================*/
/**
* \brief Returns value of uint16 type from initial uint8
*
* \param[in] value : returned with uint16 type
* 
*/
static inline uint16 FsmSw_Convert_u8_to_u16(uint8 value)
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
static inline uint32 FsmSw_Convert_u8_to_u32(uint8 value)
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
static inline uint32 FsmSw_Convert_u16_to_u32(uint16 value)
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
static inline uint64 FsmSw_Convert_u32_to_u64(uint32 value)
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
static inline uint64 FsmSw_Convert_u16_to_u64(uint16 value)
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
static inline uint64 FsmSw_Convert_u8_to_u64(uint8 value)
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
static inline uint32 FsmSw_Convert_u64_to_u32(uint64 value)
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
static inline uint8 FsmSw_Convert_u16_to_u8(uint16 value)
{
  return (uint8)(value & 0xFFU);
} // end: FsmSw_Convert_u64_to_u32

/*====================================================================================================================*/
/**
* \brief Returns value of uint8 type from initial uint16
*
* \param[in] value : returned with uint8 type
* 
*/
static inline uint16 FsmSw_Convert_u32_to_u16(uint32 value)
{
  return (uint16)(value & 0xFFFFU);
} // end: FsmSw_Convert_u32_to_u16

/**
 * \brief Get the sign bit of a signed 16-bit integer
 * 
 * \param[in] value : a signed integer
 * 
 * \retval 1 - if the value is negative
 * \retval 0 - if the value is positive
 */
static inline uint16 FsmSw_GetSignBit_i16(sint16 value)
{

  /* polyspace +2 CERT-C:INT31-C [Justified:] "Wrap-around is the intended behavior as we operate on the bit
  representation of the signed integer to avoid branching" */
  return ((uint16)value >> 15);
} // end: FsmSw_GetSignBit_i16

#endif /* FSMSW_COMMONLIB_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */