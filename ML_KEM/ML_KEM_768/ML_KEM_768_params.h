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
/** \addtogroup ML_KEM_768_params
 ** @{ */

/*====================================================================================================================*/
/** \file ML_KEM_768_params.h
* \brief  Declarations for the modul ML_KEM_768_params.c
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
#ifndef ML_KEM_768_PARAMS_H
#define ML_KEM_768_PARAMS_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "ML_KEM_768_api.h"
#include "ML_KEM_params.h"
#include "FsmSw_StaticAssert.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define ML_KEM_768_K            3u
#define ML_KEM_768_POLYVECBYTES (ML_KEM_768_K * ML_KEM_POLYBYTES)

#define ML_KEM_768_IMPLBYTES 768u

#define ML_KEM_768_POLYCOMPRESSEDBYTES    128u
#define ML_KEM_768_POLYVECCOMPRESSEDBYTES (ML_KEM_768_K * 320u)

#define ML_KEM_768_ETA1 2u
#define ML_KEM_768_ETA2 2u

#define ML_KEM_768_INDCPA_PUBLICKEYBYTES (ML_KEM_768_POLYVECBYTES + ML_KEM_SYMBYTES)
#define ML_KEM_768_INDCPA_SECRETKEYBYTES (ML_KEM_768_POLYVECBYTES)

FSMSW_STATIC_ASSERT(ML_KEM_768_PUBLICKEYBYTES == (ML_KEM_768_INDCPA_PUBLICKEYBYTES));
/* 32 bytes of additional space to save H(pk) */
FSMSW_STATIC_ASSERT(ML_KEM_768_SECRETKEYBYTES ==
                    (ML_KEM_768_INDCPA_SECRETKEYBYTES + ML_KEM_768_INDCPA_PUBLICKEYBYTES + (2u * ML_KEM_SYMBYTES)));
FSMSW_STATIC_ASSERT(ML_KEM_768_INDCPA_BYTES == (ML_KEM_768_POLYVECCOMPRESSEDBYTES + ML_KEM_768_POLYCOMPRESSEDBYTES));
FSMSW_STATIC_ASSERT(ML_KEM_768_CIPHERTEXTBYTES == (ML_KEM_768_INDCPA_BYTES));
FSMSW_STATIC_ASSERT(ML_KEM_768_INDCPA_MSGBYTES == (ML_KEM_SYMBYTES));

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

#endif /* ML_KEM_768_PARAMS_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */