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
#ifndef FSMSW_KYBER_NTT_H
#define FSMSW_KYBER_NTT_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_Kyber_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/
extern const sint16 FsmSw_Kyber_zetas[128];

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_Kyber_ntt(sint16 r[256]);
void FsmSw_Kyber_invntt(sint16 r[256]);
void FsmSw_Kyber_basemul(sint16 r[2], const sint16 a[2], const sint16 b[2], sint16 zeta);

#endif /* FSMSW_KYBER_NTT_H */
