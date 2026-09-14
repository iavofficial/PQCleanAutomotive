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
/** \addtogroup ML_KEM_768
*    includes the modules for ML_KEM_768
 ** @{ */
/** \addtogroup ML_KEM_768_polyvec
 ** @{ */

/*====================================================================================================================*/
/** \file ML_KEM_768_polyvec.h
* \brief  Declarations for the modul ML_KEM_768_polyvec.c
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
#ifndef ML_KEM_768_POLYVEC_H
#define ML_KEM_768_POLYVEC_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "ML_KEM_768_params.h"
#include "ML_KEM_768_poly.h"
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
  poly vec[ML_KEM_768_K];
} polyvec768;

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
void ML_KEM_768_Polyvec_Compress(uint8 r[ML_KEM_768_POLYVECCOMPRESSEDBYTES], const polyvec768 *const a);
void ML_KEM_768_Polyvec_Decompress(polyvec768 *const r, const uint8 a[ML_KEM_768_POLYVECCOMPRESSEDBYTES]);

void ML_KEM_768_Polyvec_ToBytes(uint8 r[ML_KEM_768_POLYVECBYTES], const polyvec768 *const a);
void ML_KEM_768_Polyvec_FromBytes(polyvec768 *r, const uint8 a[ML_KEM_768_POLYVECBYTES]);

void ML_KEM_768_Polyvec_Ntt(polyvec768 *r);
void ML_KEM_768_Polyvec_InvnttTomont(polyvec768 *r);

void ML_KEM_768_Polyvec_BasemulAccMontgomery(poly *const r, const polyvec768 *const a, const polyvec768 *const b);

void ML_KEM_768_Polyvec_Reduce(polyvec768 *r);

void ML_KEM_768_Polyvec_Add(polyvec768 *r, const polyvec768 *const a, const polyvec768 *const b);

#endif /* ML_KEM_768_POLYVEC_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */