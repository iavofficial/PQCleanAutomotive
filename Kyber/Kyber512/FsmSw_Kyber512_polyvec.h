/***********************************************************************************************************************
*
*                                          IAV GmbH
*                          All rights reserved - Alle Rechte vorbehalten
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
#ifndef FSMSW_KYBER512_POLYVEC_H
#define FSMSW_KYBER512_POLYVEC_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_Kyber512_params.h"
#include "FsmSw_Kyber512_poly.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct {
    poly512 vec[KYBER512_K];
} polyvec512;

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_Kyber512_polyvec_compress(uint8 r[KYBER512_POLYVECCOMPRESSEDBYTES], const polyvec512 *a);
void FsmSw_Kyber512_polyvec_decompress(polyvec512 *r, const uint8 a[KYBER512_POLYVECCOMPRESSEDBYTES]);

void FsmSw_Kyber512_polyvec_tobytes(uint8 r[KYBER512_POLYVECBYTES], const polyvec512 *a);
void FsmSw_Kyber512_polyvec_frombytes(polyvec512 *r, const uint8 a[KYBER512_POLYVECBYTES]);

void FsmSw_Kyber512_polyvec_ntt(polyvec512 *r);
void FsmSw_Kyber512_polyvec_invntt_tomont(polyvec512 *r);

void FsmSw_Kyber512_polyvec_basemul_acc_montgomery(poly512 *r, const polyvec512 *a, const polyvec512 *b);

void FsmSw_Kyber512_polyvec_reduce(polyvec512 *r);

void FsmSw_Kyber512_polyvec_add(polyvec512 *r, const polyvec512 *a, const polyvec512 *b);

#endif /* FSMSW_KYBER512_POLYVEC_H */