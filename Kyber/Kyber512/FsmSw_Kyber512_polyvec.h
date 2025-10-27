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
/** \addtogroup Kyber512_polyvec
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Kyber512_polyvec.h
* \brief  Declarations for the modul FsmSw_Kyber512_polyvec.c
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
#ifndef FSMSW_KYBER512_POLYVEC_H
#define FSMSW_KYBER512_POLYVEC_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Kyber512_params.h"
#include "FsmSw_Kyber512_poly.h"
#include "FsmSw_Types.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
/* polyspace +5 CERT-C:DCL12-C [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* polyspace +3 MISRA2012:D4.8 [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
typedef struct
{
  poly vec[KYBER512_K];
} polyvec512;
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
void FsmSw_Kyber512_Polyvec_Compress(uint8 r[KYBER512_POLYVECCOMPRESSEDBYTES], const polyvec512 *const a);
void FsmSw_Kyber512_Polyvec_Decompress(polyvec512 *const r, const uint8 a[KYBER512_POLYVECCOMPRESSEDBYTES]);

void FsmSw_Kyber512_Polyvec_ToBytes(uint8 r[KYBER512_POLYVECBYTES], const polyvec512 *const a);
void FsmSw_Kyber512_Polyvec_FromBytes(polyvec512 *r, const uint8 a[KYBER512_POLYVECBYTES]);

void FsmSw_Kyber512_Polyvec_Ntt(polyvec512 *r);
void FsmSw_Kyber512_Polyvec_InvnttTomont(polyvec512 *r);

void FsmSw_Kyber512_Polyvec_BasemulAccMontgomery(poly *const r, const polyvec512 *const a, const polyvec512 *const b);

void FsmSw_Kyber512_Polyvec_Reduce(polyvec512 *r);

void FsmSw_Kyber512_Polyvec_Add(polyvec512 *r, const polyvec512 *const a, const polyvec512 *const b);

#endif /* FSMSW_KYBER512_POLYVEC_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */