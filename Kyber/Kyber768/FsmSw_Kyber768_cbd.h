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
#ifndef FSMSW_KYBER768_CBD_H
#define FSMSW_KYBER768_CBD_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_Kyber768_params.h"
#include "FsmSw_Kyber768_poly.h"

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
void FsmSw_Kyber768_poly_cbd_eta1(poly768 *r, const uint8 buf[KYBER768_ETA1 * KYBER_N / 4u]);

void FsmSw_Kyber768_poly_cbd_eta2(poly768 *r, const uint8 buf[KYBER768_ETA2 * KYBER_N / 4u]);

#endif /* FSMSW_KYBER768_CBD_H */