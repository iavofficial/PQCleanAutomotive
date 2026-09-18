/***********************************************************************************************************************
 *
 * Original implementation: PQClean, SLH-DSA (standardized as SLH-DSA)
 *
 * Copyright 2026 IAV GmbH
 *
 * Original portions are dedicated to the public domain under CC0 1.0 Universal.
 * See the NOTICE file in the repository root for attribution information.
 * IAV modifications are licensed under the Apache License, Version 2.0.
 *
 * SPDX-License-Identifier: CC0-1.0 AND Apache-2.0
 *
 **********************************************************************************************************************/

#ifndef SLH_DSA_SHAKE_192SSIMPLE_API_H
#define SLH_DSA_SHAKE_192SSIMPLE_API_H

/** \addtogroup SwC SLH-DSA
*    includes the modules for SwC SLH-DSA
 ** @{ */
/** \addtogroup SLH_DSA_SHAKE_192sSimple
*    includes the modules for SLH_DSA_SHAKE_192sSimple
 ** @{ */
/** \addtogroup SLH_DSA_SHAKE_192sSimple_api
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHAKE_192sSimple_api.h
* \brief  Description of the SLH_DSA_SHAKE_192sSimple_api.h
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

/***********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"

/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
/* Resulting SPX sizes. */
#define SLH_DSA_SHAKE_192SSIMPLE_BYTES    16224u
#define SLH_DSA_SHAKE_192SSIMPLE_PK_BYTES 48u
#define SLH_DSA_SHAKE_192SSIMPLE_SK_BYTES 96u

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
void SLH_DSA_SHAKE_192sSimple_Crypto_Sign_KeyPair(uint8 *const pk, uint8 *const sk);
void SLH_DSA_SHAKE_192sSimple_Crypto_Sign_Signature(uint8 *const sig, uint32 *const siglen, const uint8 *const m,
                                                         uint32 mlen, const uint8 *const sk);
uint8 SLH_DSA_SHAKE_192sSimple_Crypto_Sign_Verify(const uint8 *const sig, uint32 siglen, const uint8 *const m,
                                                       uint32 mlen, const uint8 *const pk);
void SLH_DSA_SHAKE_192sSimple_Crypto_Sign(uint8 *const sm, uint32 *const smlen, const uint8 *const m, uint32 mlen,
                                               const uint8 *const sk);
uint8 SLH_DSA_SHAKE_192sSimple_Crypto_Sign_Open(uint8 *const m, uint32 *const mlen, const uint8 *const sm,
                                                     uint32 smlen, const uint8 *const pk);
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
#endif /* SLH_DSA_SHAKE_192SSIMPLE_API_H */
