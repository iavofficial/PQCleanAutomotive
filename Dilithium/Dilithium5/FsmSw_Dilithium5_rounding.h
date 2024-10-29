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
#ifndef FSMSW_DILITHIUM5_ROUNDING_H
#define FSMSW_DILITHIUM5_ROUNDING_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Types.h"
#include "FsmSw_Dilithium5_params.h"

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
sint32 FsmSw_Dilithium5_power2round(sint32 *a0, sint32 a);
sint32 FsmSw_Dilithium5_decompose(sint32 *a0, sint32 a);
uint8 FsmSw_Dilithium5_make_hint(sint32 a0, sint32 a1);
sint32 FsmSw_Dilithium5_use_hint(sint32 a, uint32 hint);

#endif /* FSMSW_DILITHIUM5_ROUNDING_H */
