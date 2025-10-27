/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Kyber1024
*    includes the modules for Kyber1024
 ** @{ */
/** \addtogroup Kyber1024_polyvec
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Kyber1024_polyvec.h
* \brief  Declarations for the modul FsmSw_Kyber1024_polyvec.c
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
#ifndef FSMSW_KYBER1024_POLYVEC_H
#define FSMSW_KYBER1024_POLYVEC_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Kyber1024_params.h"
#include "FsmSw_Kyber1024_poly.h"
#include "FsmSw_Types.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
/* polyspace +5 CERT-C:DCL12-C [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* polyspace +3 MISRA2012:D4.8 [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
typedef struct
{
  poly vec[KYBER1024_K];
} polyvec1024;

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
void FsmSw_Kyber1024_Polyvec_Compress(uint8 r[KYBER1024_POLYVECCOMPRESSEDBYTES], const polyvec1024 *const a);
void FsmSw_Kyber1024_Polyvec_Decompress(polyvec1024 *const r, const uint8 a[KYBER1024_POLYVECCOMPRESSEDBYTES]);

void FsmSw_Kyber1024_Polyvec_ToBytes(uint8 r[KYBER1024_POLYVECBYTES], const polyvec1024 *const a);
void FsmSw_Kyber1024_Polyvec_FromBytes(polyvec1024 *r, const uint8 a[KYBER1024_POLYVECBYTES]);

void FsmSw_Kyber1024_Polyvec_Ntt(polyvec1024 *r);
void FsmSw_Kyber1024_Polyvec_InvnttTomont(polyvec1024 *r);

void FsmSw_Kyber1024_Polyvec_BasemulAccMontgomery(poly *const r, const polyvec1024 *const a,
                                                  const polyvec1024 *const b);

void FsmSw_Kyber1024_Polyvec_Reduce(polyvec1024 *r);

void FsmSw_Kyber1024_Polyvec_Add(polyvec1024 *r, const polyvec1024 *const a, const polyvec1024 *const b);

#endif /* FSMSW_KYBER1024_POLYVEC_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */