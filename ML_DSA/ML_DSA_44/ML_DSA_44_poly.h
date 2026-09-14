/***********************************************************************************************************************
 *
 * Original implementation: PQClean, ML-DSA
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
/** \addtogroup ML_DSA_44
*    includes the modules for ML_DSA_44
 ** @{ */
/** \addtogroup ML_DSA_44_params
 ** @{ */

/*====================================================================================================================*/
/** \file ML_DSA_44_params.h
* \brief  Declarations for the modul ML_DSA_44_params.h
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
#ifndef FSMSW_ML_DSA_44_POLY_H
#define FSMSW_ML_DSA_44_POLY_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "ML_DSA_44_params.h"
#include "Std_Types.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct
{
  sint32 coeffs[N_ML_DSA];
} poly_D2;

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
void ML_DSA_44_Poly_Reduce(poly_D2 *const a);
void ML_DSA_44_Poly_CAddQ(poly_D2 *const a);

void ML_DSA_44_Poly_Add(poly_D2 *const c, const poly_D2 *const a, const poly_D2 *const b);
void ML_DSA_44_Poly_Sub(poly_D2 *const c, const poly_D2 *const a, const poly_D2 *const b);
void ML_DSA_44_Poly_Shiftl(poly_D2 *const a);

void ML_DSA_44_Poly_Ntt(poly_D2 *a);
void ML_DSA_44_Poly_InvnttTomont(poly_D2 *const a);
void ML_DSA_44_Poly_PointwiseMontgomery(poly_D2 *const c, const poly_D2 *const a, const poly_D2 *const b);

void ML_DSA_44_Poly_Power2Round(poly_D2 *const a1, poly_D2 *a0, const poly_D2 *const a);
void ML_DSA_44_Poly_Decompose(poly_D2 *const a1, poly_D2 *a0, const poly_D2 *const a);
uint32 ML_DSA_44_Poly_MakeHint(poly_D2 *const h, const poly_D2 *const a0, const poly_D2 *const a1);
void ML_DSA_44_Poly_UseHint(poly_D2 *const b, const poly_D2 *const a, const poly_D2 *const h);

sint8 ML_DSA_44_Poly_Chknorm(const poly_D2 *const a, sint32 B);
void ML_DSA_44_Poly_Uniform(poly_D2 *a, const uint8 seed[SEEDBYTES_ML_DSA], uint16 nonce);
void ML_DSA_44_Poly_UniformEta(poly_D2 *a, const uint8 seed[CRHBYTES_ML_DSA], uint16 nonce);
void ML_DSA_44_Poly_UniformGamma1(poly_D2 *const a, const uint8 seed[CRHBYTES_ML_DSA], uint16 nonce);
void ML_DSA_44_Poly_Challenge(poly_D2 *const c, const uint8 seed[CTILDEBYTES_ML_DSA_44]);

void ML_DSA_44_Polyeta_EtaPack(uint8 *const r, const poly_D2 *const a);
void ML_DSA_44_Polyeta_EtaUnpack(poly_D2 *const r, const uint8 *const a);

void ML_DSA_44_Poly_T1Pack(uint8 *const r, const poly_D2 *const a);
void ML_DSA_44_Poly_T1Unpack(poly_D2 *const r, const uint8 *const a);

void ML_DSA_44_Poly_T0Pack(uint8 *const r, const poly_D2 *const a);
void ML_DSA_44_Poly_T0Unpack(poly_D2 *const r, const uint8 *const a);

void ML_DSA_44_Poly_ZPack(uint8 *const r, const poly_D2 *const a);
void ML_DSA_44_Poly_ZUnpack(poly_D2 *const r, const uint8 *const a);

void ML_DSA_44_Poly_W1Pack(uint8 *const r, const poly_D2 *const a);

#endif /* FSMSW_ML_DSA_44_POLY_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */