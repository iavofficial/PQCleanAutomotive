/***********************************************************************************************************************
 *
 * Original implementation: PQClean, ML-KEM (formerly CRYSTALS-Kyber)
 *
 * Copyright 2026 IAV GmbH
 *
 * Original portions are marked as Public Domain by PQClean.
 * See the NOTICE file in the repository root for the upstream
 * license reference and attribution information.
 * IAV modifications are licensed under the Apache License, Version 2.0.
 *
 * SPDX-License-Identifier: LicenseRef-PQClean-Public-Domain AND Apache-2.0
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup ML_KEM_512
*    includes the modules for ML_KEM_512
 ** @{ */
/** \addtogroup ML_KEM_512_polyvec
 ** @{ */

/*====================================================================================================================*/
/** \file ML_KEM_512_polyvec.h
* \brief  Declarations for the modul ML_KEM_512_polyvec.c
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
#ifndef ML_KEM_512_POLYVEC_H
#define ML_KEM_512_POLYVEC_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "ML_KEM_512_params.h"
#include "ML_KEM_512_poly.h"
#include "Std_Types.h"
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
  poly vec[ML_KEM_512_K];
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
void ML_KEM_512_Polyvec_Compress(uint8 r[ML_KEM_512_POLYVECCOMPRESSEDBYTES], const polyvec512 *const a);
void ML_KEM_512_Polyvec_Decompress(polyvec512 *const r, const uint8 a[ML_KEM_512_POLYVECCOMPRESSEDBYTES]);

void ML_KEM_512_Polyvec_ToBytes(uint8 r[ML_KEM_512_POLYVECBYTES], const polyvec512 *const a);
void ML_KEM_512_Polyvec_FromBytes(polyvec512 *r, const uint8 a[ML_KEM_512_POLYVECBYTES]);

void ML_KEM_512_Polyvec_Ntt(polyvec512 *r);
void ML_KEM_512_Polyvec_InvnttTomont(polyvec512 *r);

void ML_KEM_512_Polyvec_BasemulAccMontgomery(poly *const r, const polyvec512 *const a, const polyvec512 *const b);

void ML_KEM_512_Polyvec_Reduce(polyvec512 *r);

void ML_KEM_512_Polyvec_Add(polyvec512 *r, const polyvec512 *const a, const polyvec512 *const b);

#endif /* ML_KEM_512_POLYVEC_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */