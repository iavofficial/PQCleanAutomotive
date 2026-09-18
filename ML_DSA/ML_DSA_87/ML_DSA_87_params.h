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
/** \addtogroup ML_DSA_87
*    includes the modules for ML_DSA_87
 ** @{ */
/** \addtogroup ML_DSA_87_params
 ** @{ */

/*====================================================================================================================*/
/** \file ML_DSA_87_params.h
* \brief  Description of the ML_DSA_87_params.h
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
#ifndef FSMSW_ML_DSA_87_PARAMS_H
#define FSMSW_ML_DSA_87_PARAMS_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "ML_DSA_87_api.h"
#include "ML_DSA_params.h"
#include "FsmSw_StaticAssert.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define K_ML_DSA_87    8u
#define L_ML_DSA_87    7u
#define ETA_ML_DSA_87  2u
#define TAU_ML_DSA_87  60u
#define BETA_ML_DSA_87 120u
/* (1 << 19) = 524288u */
#define GAMMA1_ML_DSA_87 524288u
/* MISRA check for GAMMA2_ML_DSA_87 is ongoing. You get an error if GAMMA2_ML_DSA_87 get a type cast. */
#define GAMMA2_ML_DSA_87      ((Q_ML_DSA - 1) / 32)
#define OMEGA_ML_DSA_87       75u
#define CTILDEBYTES_ML_DSA_87 64u

#define POLYVECH_PACKEDBYTES_ML_DSA_87 (OMEGA_ML_DSA_87 + K_ML_DSA_87)

#define POLYZ_PACKEDBYTES_ML_DSA_87   640u
#define POLYW1_PACKEDBYTES_ML_DSA_87  128u
#define POLYETA_PACKEDBYTES_ML_DSA_87 96u

FSMSW_STATIC_ASSERT(FSMSW_ML_DSA_87_CRYPTO_PUBLICKEYBYTES ==
                    (SEEDBYTES_ML_DSA + (K_ML_DSA_87 * POLYT1_PACKEDBYTES_ML_DSA)));
FSMSW_STATIC_ASSERT(FSMSW_ML_DSA_87_CRYPTO_SECRETKEYBYTES ==
                    ((2u * SEEDBYTES_ML_DSA) + TRBYTES_ML_DSA + (L_ML_DSA_87 * POLYETA_PACKEDBYTES_ML_DSA_87) +
                     (K_ML_DSA_87 * POLYETA_PACKEDBYTES_ML_DSA_87) + (K_ML_DSA_87 * POLYT0_PACKEDBYTES_ML_DSA)));
FSMSW_STATIC_ASSERT(FSMSW_ML_DSA_87_CRYPTO_BYTES ==
                    (CTILDEBYTES_ML_DSA_87 + (L_ML_DSA_87 * POLYZ_PACKEDBYTES_ML_DSA_87) +
                     POLYVECH_PACKEDBYTES_ML_DSA_87));

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

#endif /* FSMSW_ML_DSA_87_PARAMS_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */