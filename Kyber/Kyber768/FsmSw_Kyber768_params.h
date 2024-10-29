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
#ifndef FSMSW_KYBER768_PARAMS_H
#define FSMSW_KYBER768_PARAMS_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Kyber_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define KYBER768_K             3u
#define KYBER768_POLYVECBYTES  (KYBER768_K * KYBER_POLYBYTES)

#define KYBER768_POLYCOMPRESSEDBYTES    128u
#define KYBER768_POLYVECCOMPRESSEDBYTES (KYBER768_K * 320u)

#define KYBER768_ETA1  2u
#define KYBER768_ETA2  2u

#define KYBER768_INDCPA_MSGBYTES       (KYBER_SYMBYTES)
#define KYBER768_INDCPA_PUBLICKEYBYTES (KYBER768_POLYVECBYTES + KYBER_SYMBYTES)
#define KYBER768_INDCPA_SECRETKEYBYTES (KYBER768_POLYVECBYTES)
#define KYBER768_INDCPA_BYTES          (KYBER768_POLYVECCOMPRESSEDBYTES + KYBER768_POLYCOMPRESSEDBYTES)

#define KYBER768_PUBLICKEYBYTES  (KYBER768_INDCPA_PUBLICKEYBYTES)
/* 32 bytes of additional space to save H(pk) */
#define KYBER768_SECRETKEYBYTES  (KYBER768_INDCPA_SECRETKEYBYTES + KYBER768_INDCPA_PUBLICKEYBYTES + (2u*KYBER_SYMBYTES))
#define KYBER768_CIPHERTEXTBYTES (KYBER768_INDCPA_BYTES)

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

#endif /* FSMSW_KYBER768_PARAMS_H */
