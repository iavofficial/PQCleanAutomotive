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
#ifndef FSMSW_KYBER512_POLY_H
#define FSMSW_KYBER512_POLY_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Kyber512_params.h"
#include "FsmSw_Kyber_CommonLib.h"
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
void FsmSw_Kyber512_Poly_Compress(uint8 r[KYBER512_POLYCOMPRESSEDBYTES], const poly *a);
void FsmSw_Kyber512_Poly_Decompress(poly *r, const uint8 a[KYBER512_POLYCOMPRESSEDBYTES]);

void FsmSw_Kyber512_Poly_FromMsg(poly *r, const uint8 msg[KYBER512_INDCPA_MSGBYTES]);
void FsmSw_Kyber512_Poly_ToMsg(uint8 msg[KYBER512_INDCPA_MSGBYTES], const poly *a);

void FsmSw_Kyber512_Poly_GetNoiseEta1(poly *r, const uint8 seed[KYBER_SYMBYTES], uint8 nonce);

/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_Kyber512_Poly_GetNoiseEta2(poly *r, const uint8 seed[KYBER_SYMBYTES], uint8 nonce);

#endif /* FSMSW_KYBER512_POLY_H */
