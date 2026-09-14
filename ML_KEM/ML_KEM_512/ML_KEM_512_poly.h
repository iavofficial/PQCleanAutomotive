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
/** \addtogroup ML_KEM_512_poly
 ** @{ */

/*====================================================================================================================*/
/** \file ML_KEM_512_poly.h
* \brief  Declarations for the modul ML_KEM_512_poly.c
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
#ifndef ML_KEM_512_POLY_H
#define ML_KEM_512_POLY_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "ML_KEM_512_params.h"
#include "ML_KEM_CommonLib.h"
#include "Std_Types.h"
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
void ML_KEM_512_Poly_Compress(uint8 r[ML_KEM_512_POLYCOMPRESSEDBYTES], const poly *const a);
void ML_KEM_512_Poly_Decompress(poly *const r, const uint8 a[ML_KEM_512_POLYCOMPRESSEDBYTES]);

void ML_KEM_512_Poly_FromMsg(poly *const r, const uint8 msg[ML_KEM_512_INDCPA_MSGBYTES]);
void ML_KEM_512_Poly_ToMsg(uint8 msg[ML_KEM_512_INDCPA_MSGBYTES], const poly *const a);

void ML_KEM_512_Poly_GetNoiseEta1(poly *const r, const uint8 seed[ML_KEM_SYMBYTES], uint8 nonce);

void ML_KEM_512_Poly_GetNoiseEta2(poly *const r, const uint8 seed[ML_KEM_SYMBYTES], uint8 nonce);

#endif /* ML_KEM_512_POLY_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
