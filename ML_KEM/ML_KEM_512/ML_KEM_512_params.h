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
/** \addtogroup ML_KEM_512_params
 ** @{ */

/*====================================================================================================================*/
/** \file ML_KEM_512_params.h
* \brief  description of ML_KEM_512_params.h
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
#ifndef ML_KEM_512_PARAMS_H
#define ML_KEM_512_PARAMS_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "ML_KEM_512_api.h"
#include "ML_KEM_params.h"
#include "FsmSw_StaticAssert.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define ML_KEM_512_K            2u
#define ML_KEM_512_POLYVECBYTES (ML_KEM_512_K * ML_KEM_POLYBYTES)

#define ML_KEM_512_IMPLBYTES 512u

#define ML_KEM_512_POLYCOMPRESSEDBYTES    128u
#define ML_KEM_512_POLYVECCOMPRESSEDBYTES (ML_KEM_512_K * 320u)

#define ML_KEM_512_ETA1 3u
#define ML_KEM_512_ETA2 2u

#define ML_KEM_512_INDCPA_PUBLICKEYBYTES (ML_KEM_512_POLYVECBYTES + ML_KEM_SYMBYTES)
#define ML_KEM_512_INDCPA_SECRETKEYBYTES (ML_KEM_512_POLYVECBYTES)

FSMSW_STATIC_ASSERT(ML_KEM_512_PUBLICKEYBYTES == (ML_KEM_512_INDCPA_PUBLICKEYBYTES));
/* 32 bytes of additional space to save H(pk) */
FSMSW_STATIC_ASSERT(ML_KEM_512_SECRETKEYBYTES ==
                    (ML_KEM_512_INDCPA_SECRETKEYBYTES + ML_KEM_512_INDCPA_PUBLICKEYBYTES + (2u * ML_KEM_SYMBYTES)));
FSMSW_STATIC_ASSERT(ML_KEM_512_INDCPA_BYTES == (ML_KEM_512_POLYVECCOMPRESSEDBYTES + ML_KEM_512_POLYCOMPRESSEDBYTES));
FSMSW_STATIC_ASSERT(ML_KEM_512_CIPHERTEXTBYTES == (ML_KEM_512_INDCPA_BYTES));
FSMSW_STATIC_ASSERT(ML_KEM_512_INDCPA_MSGBYTES == (ML_KEM_SYMBYTES));

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

#endif /* ML_KEM_512_PARAMS_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */