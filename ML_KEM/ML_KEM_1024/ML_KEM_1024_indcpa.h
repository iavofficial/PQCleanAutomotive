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
/** \addtogroup ML_KEM_1024_indcpa
 ** @{ */

/*====================================================================================================================*/
/** \file ML_KEM_1024_indcpa.h
* \brief  Declarations for the modul ML_KEM_1024_indcpa.c
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
#ifndef ML_KEM_1024_INDCPA_H
#define ML_KEM_1024_INDCPA_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "ML_KEM_1024_params.h"
#include "ML_KEM_1024_polyvec.h"
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
void ML_KEM_1024_Indcpa_GenMatrix(polyvec1024 *a, const uint8 seed[ML_KEM_SYMBYTES], uint8 transposed);

void ML_KEM_1024_Indcpa_KeyPair(uint8 pk[ML_KEM_1024_INDCPA_PUBLICKEYBYTES],
                                    uint8 sk[ML_KEM_1024_INDCPA_SECRETKEYBYTES]);

void ML_KEM_1024_Indcpa_Enc(uint8 c[ML_KEM_1024_INDCPA_BYTES], const uint8 m[ML_KEM_1024_INDCPA_MSGBYTES],
                                const uint8 pk[ML_KEM_1024_INDCPA_PUBLICKEYBYTES], const uint8 coins[ML_KEM_SYMBYTES]);

void ML_KEM_1024_Indcpa_Dec(uint8 m[ML_KEM_1024_INDCPA_MSGBYTES], const uint8 c[ML_KEM_1024_INDCPA_BYTES],
                                const uint8 sk[ML_KEM_1024_INDCPA_SECRETKEYBYTES]);

#endif /* ML_KEM_1024_INDCPA_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */