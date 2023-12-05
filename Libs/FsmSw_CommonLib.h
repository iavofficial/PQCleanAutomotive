/***********************************************************************************************************************
*
*                                          IAV GmbH
*
***********************************************************************************************************************/
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
#include "Platform_Types.h"

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
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void  FsmSw_CommonLib_memcpy(void *dest, const void * src, const uint32 n);
void  FsmSw_CommonLib_memset(void *dest, const uint8 value, const uint32 n);
uint8 FsmSw_CommonLib_memcmp(void *dest, const void * src,  const uint32 n);
void  FsmSw_CommonLib_memmove(void *dest, const void * src, const uint32 n);
uint8 FsmSw_CommonLib_randombytes(uint8 *output, const uint32 n);
void  FsmSw_CommonLib_srand(const uint32 seed);

#endif /* FSMSW_COMMONLIB_H */
