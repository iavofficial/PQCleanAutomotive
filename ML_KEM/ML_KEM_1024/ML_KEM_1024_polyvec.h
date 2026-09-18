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
/** \addtogroup ML_KEM_1024
*    includes the modules for ML_KEM_1024
 ** @{ */
/** \addtogroup ML_KEM_1024_polyvec
 ** @{ */

/*====================================================================================================================*/
/** \file ML_KEM_1024_polyvec.h
* \brief  Declarations for the modul ML_KEM_1024_polyvec.c
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
#ifndef ML_KEM_1024_POLYVEC_H
#define ML_KEM_1024_POLYVEC_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "ML_KEM_1024_params.h"
#include "ML_KEM_1024_poly.h"
#include "Std_Types.h"
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
  poly vec[ML_KEM_1024_K];
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
void ML_KEM_1024_Polyvec_Compress(uint8 r[ML_KEM_1024_POLYVECCOMPRESSEDBYTES], const polyvec1024 *const a);
void ML_KEM_1024_Polyvec_Decompress(polyvec1024 *const r, const uint8 a[ML_KEM_1024_POLYVECCOMPRESSEDBYTES]);

void ML_KEM_1024_Polyvec_ToBytes(uint8 r[ML_KEM_1024_POLYVECBYTES], const polyvec1024 *const a);
void ML_KEM_1024_Polyvec_FromBytes(polyvec1024 *r, const uint8 a[ML_KEM_1024_POLYVECBYTES]);

void ML_KEM_1024_Polyvec_Ntt(polyvec1024 *r);
void ML_KEM_1024_Polyvec_InvnttTomont(polyvec1024 *r);

void ML_KEM_1024_Polyvec_BasemulAccMontgomery(poly *const r, const polyvec1024 *const a,
                                                  const polyvec1024 *const b);

void ML_KEM_1024_Polyvec_Reduce(polyvec1024 *r);

void ML_KEM_1024_Polyvec_Add(polyvec1024 *r, const polyvec1024 *const a, const polyvec1024 *const b);

#endif /* ML_KEM_1024_POLYVEC_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */