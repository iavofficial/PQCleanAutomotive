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
#ifndef FSMSW_KYBER1024_KEM_H
#define FSMSW_KYBER1024_KEM_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Kyber1024_params.h"
#include "FsmSw_Types.h"

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
uint8 FsmSw_Kyber1024_Crypto_Kem_KeyPair(uint8 *pk, uint8 *sk);
uint8 FsmSw_Kyber1024_Crypto_Kem_Enc(uint8 *ct, uint8 *ss, const uint8 *pk);
uint8 FsmSw_Kyber1024_Crypto_Kem_Dec(uint8 *ss, const uint8 *ct, const uint8 *sk);

#endif /* FSMSW_KYBER1024_KEM_H */
