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
/** \addtogroup ML_DSA_87_api
 ** @{ */

/*====================================================================================================================*/
/** \file ML_DSA_87_api.h
* \brief  Description of the ML_DSA_87_api.h
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
#ifndef FSMSW_ML_DSA_87_API_H
#define FSMSW_ML_DSA_87_API_H
/***********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"

/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define FSMSW_ML_DSA_87_CRYPTO_PUBLICKEYBYTES 2592u
#define FSMSW_ML_DSA_87_CRYPTO_SECRETKEYBYTES 4896u
#define FSMSW_ML_DSA_87_CRYPTO_BYTES          4627u

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
void ML_DSA_87_Crypto_Sign_KeyPair(uint8 *const pk, uint8 *const sk);
uint8 ML_DSA_87_Crypto_Sign_Signature(uint8 *const sig, uint32 *const siglen, const uint8 *const m, uint32 mlen,
                                             const uint8 *const sk);
uint8 ML_DSA_87_Crypto_Sign(uint8 *const sm, uint32 *const smlen, const uint8 *const m, uint32 mlen,
                                   const uint8 *const sk);
uint8 ML_DSA_87_Crypto_Sign_Verify(const uint8 *const sig, uint32 siglen, const uint8 *const m, uint32 mlen,
                                          const uint8 *const pk);
uint8 ML_DSA_87_Crypto_Sign_Open(uint8 *const m, uint32 *const mlen, const uint8 *const sm, uint32 smlen,
                                        const uint8 *const pk);

// supply the context sensitive methods as well (formerly statics)
uint8 ML_DSA_87_Crypto_Sign_Signature_Ctx(uint8 *const sig, uint32 *const siglen, const uint8 *const m,
                                                 uint32 mlen, const uint8 *ctx, uint32 ctxlen, const uint8 *const sk);
uint8 ML_DSA_87_Crypto_Sign_Ctx(uint8 *const sm, uint32 *const smlen, const uint8 *const m, uint32 mlen,
                                       const uint8 *ctx, uint32 ctxlen, const uint8 *const sk);
uint8 ML_DSA_87_Crypto_Sign_Verify_Ctx(const uint8 *const sig, uint32 siglen, const uint8 *const m, uint32 mlen,
                                              const uint8 *ctx, uint32 ctxlen, const uint8 *const pk);
uint8 ML_DSA_87_Crypto_Sign_Open_Ctx(uint8 *const m, uint32 *const mlen, const uint8 *const sm, uint32 smlen,
                                            const uint8 *ctx, uint32 ctxlen, const uint8 *const pk);
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
#endif /* FSMSW_ML_DSA_87_API_H */
