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
#ifndef ML_KEM_1024_PARAMS_H
#define ML_KEM_1024_PARAMS_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "ML_KEM_1024_api.h"
#include "ML_KEM_params.h"
#include "FsmSw_StaticAssert.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define ML_KEM_1024_K            4u
#define ML_KEM_1024_POLYVECBYTES (ML_KEM_1024_K * ML_KEM_POLYBYTES)

#define ML_KEM_1024_IMPLBYTES 1024u

#define ML_KEM_1024_POLYCOMPRESSEDBYTES    160u
#define ML_KEM_1024_POLYVECCOMPRESSEDBYTES (ML_KEM_1024_K * 352u)

#define ML_KEM_1024_ETA1 2u
#define ML_KEM_1024_ETA2 2u

#define ML_KEM_1024_INDCPA_PUBLICKEYBYTES (ML_KEM_1024_POLYVECBYTES + ML_KEM_SYMBYTES)
#define ML_KEM_1024_INDCPA_SECRETKEYBYTES (ML_KEM_1024_POLYVECBYTES)

FSMSW_STATIC_ASSERT(ML_KEM_1024_PUBLICKEYBYTES == (ML_KEM_1024_INDCPA_PUBLICKEYBYTES));
/* 32 bytes of additional space to save H(pk) */
FSMSW_STATIC_ASSERT(ML_KEM_1024_SECRETKEYBYTES ==
                    (ML_KEM_1024_INDCPA_SECRETKEYBYTES + ML_KEM_1024_INDCPA_PUBLICKEYBYTES + (2u * ML_KEM_SYMBYTES)));
FSMSW_STATIC_ASSERT(ML_KEM_1024_INDCPA_BYTES == (ML_KEM_1024_POLYVECCOMPRESSEDBYTES + ML_KEM_1024_POLYCOMPRESSEDBYTES));
FSMSW_STATIC_ASSERT(ML_KEM_1024_CIPHERTEXTBYTES == (ML_KEM_1024_INDCPA_BYTES));
FSMSW_STATIC_ASSERT(ML_KEM_1024_INDCPA_MSGBYTES == (ML_KEM_SYMBYTES));

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

#endif /* ML_KEM_1024_PARAMS_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */