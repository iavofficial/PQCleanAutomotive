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
#ifndef FSMSW_DILITHIUM2_ROUNDING_H
#define FSMSW_DILITHIUM2_ROUNDING_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_Dilithium2_params.h"

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
sint32 FsmSw_Dilithium2_power2round(sint32 *a0, sint32 a);
sint32 FsmSw_Dilithium2_decompose(sint32 *a0, sint32 a);
uint8 FsmSw_Dilithium2_make_hint(sint32 a0, sint32 a1);
sint32 FsmSw_Dilithium2_use_hint(sint32 a, uint32 hint);

#endif /* FSMSW_DILITHIUM2_ROUNDING_H */
