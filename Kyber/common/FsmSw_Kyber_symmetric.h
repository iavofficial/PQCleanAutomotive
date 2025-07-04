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
#ifndef FSMSW_KYBER_SYMMETRIC_H
#define FSMSW_KYBER_SYMMETRIC_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Fips202.h"
#include "FsmSw_Kyber_params.h"
#include "FsmSw_Types.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define XOF_BLOCKBYTES SHAKE128_RATE

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef shake128ctx xof_state;

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_Kyber_Shake128_Absorb(xof_state *s, const uint8 seed[KYBER_SYMBYTES], uint8 x, uint8 y);
void FsmSw_Kyber_Shake256_Prf(uint8 *out, uint32 outlen, const uint8 key[KYBER_SYMBYTES], uint8 nonce);

#endif /* FSMSW_KYBER_SYMMETRIC_H */
