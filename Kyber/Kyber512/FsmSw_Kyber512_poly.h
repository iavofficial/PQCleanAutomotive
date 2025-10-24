/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Kyber512
*    includes the modules for Kyber512
 ** @{ */
/** \addtogroup Kyber512_poly
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Kyber512_poly.h
* \brief  Declarations for the modul FsmSw_Kyber512_poly.c
*
* \details
*
*
*/
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
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL CONSTANTS                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_Kyber512_Poly_Compress(uint8 r[KYBER512_POLYCOMPRESSEDBYTES], const poly *const a);
void FsmSw_Kyber512_Poly_Decompress(poly *const r, const uint8 a[KYBER512_POLYCOMPRESSEDBYTES]);

void FsmSw_Kyber512_Poly_FromMsg(poly *const r, const uint8 msg[KYBER512_INDCPA_MSGBYTES]);
void FsmSw_Kyber512_Poly_ToMsg(uint8 msg[KYBER512_INDCPA_MSGBYTES], const poly *const a);

void FsmSw_Kyber512_Poly_GetNoiseEta1(poly *const r, const uint8 seed[KYBER_SYMBYTES], uint8 nonce);

/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_Kyber512_Poly_GetNoiseEta2(poly *const r, const uint8 seed[KYBER_SYMBYTES], uint8 nonce);

#endif /* FSMSW_KYBER512_POLY_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */