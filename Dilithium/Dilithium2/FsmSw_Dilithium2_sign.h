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
#ifndef FSMSW_DILITHIUM2_SIGN_H
#define FSMSW_DILITHIUM2_SIGN_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_Dilithium2_params.h"
#include "FsmSw_Dilithium2_poly.h"
#include "FsmSw_Dilithium2_polyvec.h"

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
void FsmSw_Dilithium2_challenge(poly_D2 *c, const uint8 seed[SEEDBYTES_DILITHIUM]);
sint8 FsmSw_Dilithium2_crypto_sign_keypair(uint8 *pk, uint8 *sk);
sint8 FsmSw_Dilithium2_crypto_sign_signature(uint8 *sig, uint32 *siglen, const uint8 *m, uint32 mlen, const uint8 *sk);
sint8 FsmSw_Dilithium2_crypto_sign(uint8 *sm, uint32 *smlen, const uint8 *m, uint32 mlen, const uint8 *sk);
sint8 FsmSw_Dilithium2_crypto_sign_verify(const uint8 *sig, uint32 siglen, const uint8 *m, uint32 mlen, const uint8 *pk);
sint8 FsmSw_Dilithium2_crypto_sign_open(uint8 *m, uint32 *mlen, const uint8 *sm, uint32 smlen, const uint8 *pk);

#endif /* FSMSW_DILITHIUM2_SIGN_H */
