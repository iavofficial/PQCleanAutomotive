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
/** \addtogroup common
*    includes the modules for common
 ** @{ */
/** \addtogroup ML_KEM_poly
 ** @{ */

/*====================================================================================================================*/
/** \file ML_KEM_poly.h
* \brief  Declarations for the modul ML_KEM_poly.c
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
#ifndef ML_KEM_POLY_H
#define ML_KEM_POLY_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
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
void ML_KEM_Poly_ToBytes(uint8 r[ML_KEM_POLYBYTES], const poly *a);
void ML_KEM_Poly_FromBytes(poly *r, const uint8 a[ML_KEM_POLYBYTES]);

void ML_KEM_Poly_Ntt(poly *r);
void ML_KEM_Poly_InvnttTomont(poly *r);
void ML_KEM_Poly_BasemulMontgomery(poly *r, const poly *a, const poly *b);
void ML_KEM_Poly_Tomont(poly *r);

void ML_KEM_Poly_Reduce(poly *r);

void ML_KEM_Poly_Add(poly *r, const poly *a, const poly *b);
void ML_KEM_Poly_Sub(poly *r, const poly *a, const poly *b);

#endif /* ML_KEM_POLY_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */