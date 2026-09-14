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
* \brief  Description of ML_DSA_44_params.h
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
#ifndef FSMSW_ML_DSA_44_PARAMS_H
#define FSMSW_ML_DSA_44_PARAMS_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "ML_DSA_44_api.h"
#include "ML_DSA_params.h"
#include "FsmSw_StaticAssert.h"

/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define K_ML_DSA_44    4u
#define L_ML_DSA_44    4u
#define ETA_ML_DSA_44  2u
#define TAU_ML_DSA_44  39u
#define BETA_ML_DSA_44 78u
/* (1u << 17u) = 131072 */
#define GAMMA1_ML_DSA_44 131072u
/* MISRA check for GAMMA2_ML_DSA_44 is ongoing. You get an error if GAMMA2_ML_DSA_44 get a type cast. */
#define GAMMA2_ML_DSA_44      ((Q_ML_DSA - 1) / 88)
#define OMEGA_ML_DSA_44       80u
#define CTILDEBYTES_ML_DSA_44 32u

#define POLYVECH_PACKEDBYTES_ML_DSA_44 (OMEGA_ML_DSA_44 + K_ML_DSA_44)

#define POLYZ_PACKEDBYTES_ML_DSA_44   576u
#define POLYW1_PACKEDBYTES_ML_DSA_44  192u
#define POLYETA_PACKEDBYTES_ML_DSA_44 96u

FSMSW_STATIC_ASSERT(FSMSW_ML_DSA_44_CRYPTO_PUBLICKEYBYTES ==
                    (SEEDBYTES_ML_DSA + (K_ML_DSA_44 * POLYT1_PACKEDBYTES_ML_DSA)));
FSMSW_STATIC_ASSERT(FSMSW_ML_DSA_44_CRYPTO_SECRETKEYBYTES ==
                    ((2u * SEEDBYTES_ML_DSA) + TRBYTES_ML_DSA + (L_ML_DSA_44 * POLYETA_PACKEDBYTES_ML_DSA_44) +
                     (K_ML_DSA_44 * POLYETA_PACKEDBYTES_ML_DSA_44) + (K_ML_DSA_44 * POLYT0_PACKEDBYTES_ML_DSA)));
FSMSW_STATIC_ASSERT(FSMSW_ML_DSA_44_CRYPTO_BYTES ==
                    (CTILDEBYTES_ML_DSA_44 + (L_ML_DSA_44 * POLYZ_PACKEDBYTES_ML_DSA_44) +
                     POLYVECH_PACKEDBYTES_ML_DSA_44));

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

#endif /* FSMSW_ML_DSA_44_PARAMS_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */