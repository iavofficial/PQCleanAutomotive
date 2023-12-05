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
#ifndef FSMSW_KYBER768_KEM_H
#define FSMSW_KYBER768_KEM_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_Kyber768_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define FSMSW_KYBER768_CRYPTO_SECRETKEYBYTES  KYBER768_SECRETKEYBYTES
#define FSMSW_KYBER768_CRYPTO_PUBLICKEYBYTES  KYBER768_PUBLICKEYBYTES
#define FSMSW_KYBER768_CRYPTO_CIPHERTEXTBYTES KYBER768_CIPHERTEXTBYTES
#define FSMSW_KYBER768_CRYPTO_BYTES           KYBER_SSBYTES

#define FSMSW_KYBER768_CRYPTO_ALGNAME "Kyber768"

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
uint8 FsmSw_Kyber768_crypto_kem_keypair(uint8 *pk, uint8 *sk);
uint8 FsmSw_Kyber768_crypto_kem_enc(uint8 *ct, uint8 *ss, const uint8 *pk);
uint8 FsmSw_Kyber768_crypto_kem_dec(uint8 *ss, const uint8 *ct, const uint8 *sk);

#endif /* FSMSW_KYBER768_KEM_H */
