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
#include "FsmSw_Types.h"
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
uint64 FsmSw_Convert_u32_to_u64(uint32 value);
uint64 FsmSw_Convert_u16_to_u64(uint16 value);
uint64 FsmSw_Convert_u8_to_u64(uint8 value);
uint32 FsmSw_Convert_u8_to_u32(uint8 value);
uint32 FsmSw_Convert_u64_to_u32(uint64 value);
uint32 FsmSw_Convert_u16_to_u32(uint16 value);
uint16 FsmSw_Convert_u8_to_u16(uint8 value);
uint8 FsmSw_Convert_u16_to_u8(uint16 value);

#endif /* FSMSW_COMMONLIB_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */