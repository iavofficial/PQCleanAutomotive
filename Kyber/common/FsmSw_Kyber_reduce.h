/***********************************************************************************************************************
*
*                                          IAV GmbH
*                          All rights reserved - Alle Rechte vorbehalten
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
#ifndef FSMSW_KYBER_REDUCE_H
#define FSMSW_KYBER_REDUCE_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_Kyber_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define MONT (-1044) // 2^16 mod q
#define QINV (-3327) // q^-1 mod 2^16

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
sint16 FsmSw_Kyber_montgomery_reduce(sint32 a);
sint16 FsmSw_Kyber_barrett_reduce(sint16 a);

#endif /* FSMSW_KYBER_REDUCE_H */