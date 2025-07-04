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
#ifndef FSMSW_KYBER768_POLYVEC_H
#define FSMSW_KYBER768_POLYVEC_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Kyber768_params.h"
#include "FsmSw_Kyber768_poly.h"
#include "FsmSw_Types.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
/* polyspace +3 MISRA2012:D4.8 [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
typedef struct
{
  poly vec[KYBER768_K];
} polyvec768;

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_Kyber768_Polyvec_Compress(uint8 r[KYBER768_POLYVECCOMPRESSEDBYTES], const polyvec768 *a);
void FsmSw_Kyber768_Polyvec_Decompress(polyvec768 *r, const uint8 a[KYBER768_POLYVECCOMPRESSEDBYTES]);

void FsmSw_Kyber768_Polyvec_ToBytes(uint8 r[KYBER768_POLYVECBYTES], const polyvec768 *a);
void FsmSw_Kyber768_Polyvec_FromBytes(polyvec768 *r, const uint8 a[KYBER768_POLYVECBYTES]);

void FsmSw_Kyber768_Polyvec_Ntt(polyvec768 *r);
void FsmSw_Kyber768_Polyvec_InvnttTomont(polyvec768 *r);

void FsmSw_Kyber768_Polyvec_BasemulAccMontgomery(poly *r, const polyvec768 *a, const polyvec768 *b);

void FsmSw_Kyber768_Polyvec_Reduce(polyvec768 *r);

void FsmSw_Kyber768_Polyvec_Add(polyvec768 *r, const polyvec768 *a, const polyvec768 *b);

#endif /* FSMSW_KYBER768_POLYVEC_H */
