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

/** \addtogroup SwC SLH-DSA
*    includes the modules for SwC SLH-DSA
 ** @{ */
/** \addtogroup SLH_DSA_SHA2_256sSimple
*    includes the modules for SLH_DSA_SHA2_256sSimple
 ** @{ */
/** \addtogroup SLH_DSA_SHA2_256sSimple_params
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHA2_256sSimple_params.h
* \brief  Description of SLH_DSA_SHA2_256sSimple_params.h
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
#ifndef SLH_DSA_SHA2_256SSIMPLE_PARAMS_H
#define SLH_DSA_SHA2_256SSIMPLE_PARAMS_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "SLH_DSA_SHA2_256sSimple_api.h"
#include "SLH_DSA_SHA2_offsets.h"
#include "FsmSw_StaticAssert.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
/* Hash output length in bytes. */
#define SLH_DSA_SHA2_256SSIMPLE_N 32u
/* Height of the hypertree. */
#define SLH_DSA_SHA2_256SSIMPLE_FULL_HEIGHT 64u
/* Number of subtree layer. */
#define SLH_DSA_SHA2_256SSIMPLE_D 8u
/* FORS tree dimensions. */
#define SLH_DSA_SHA2_256SSIMPLE_FORS_HEIGHT 14u
#define SLH_DSA_SHA2_256SSIMPLE_FORS_TREES  22u
/* Winternitz parameter, */
#define SLH_DSA_SHA2_256SSIMPLE_WOTS_W 16u

/* WOTS parameters. */
#define SLH_DSA_SHA2_256SSIMPLE_WOTS_LOGW 4u

#define SLH_DSA_SHA2_256SSIMPLE_WOTS_LEN1                                                                         \
  (8u * SLH_DSA_SHA2_256SSIMPLE_N / SLH_DSA_SHA2_256SSIMPLE_WOTS_LOGW)

/* SLH_DSA_SHA2_256SSIMPLE_WOTS_LEN2 is floor(log(len_1 * (w - 1)) / log(w)) + 1 we precompute */
#define SLH_DSA_SHA2_256SSIMPLE_WOTS_LEN2 3u

#define SLH_DSA_SHA2_256SSIMPLE_WOTS_LEN                                                                          \
  (SLH_DSA_SHA2_256SSIMPLE_WOTS_LEN1 + SLH_DSA_SHA2_256SSIMPLE_WOTS_LEN2)
#define SLH_DSA_SHA2_256SSIMPLE_WOTS_BYTES (SLH_DSA_SHA2_256SSIMPLE_WOTS_LEN * SLH_DSA_SHA2_256SSIMPLE_N)

/* Subtree size. */
#define SLH_DSA_SHA2_256SSIMPLE_TREE_HEIGHT                                                                       \
  (SLH_DSA_SHA2_256SSIMPLE_FULL_HEIGHT / SLH_DSA_SHA2_256SSIMPLE_D)

/* FORS parameters. */
#define SLH_DSA_SHA2_256SSIMPLE_FORS_MSG_BYTES                                                                    \
  (((SLH_DSA_SHA2_256SSIMPLE_FORS_HEIGHT * SLH_DSA_SHA2_256SSIMPLE_FORS_TREES) + 7u) / 8u)
#define SLH_DSA_SHA2_256SSIMPLE_FORS_BYTES                                                                        \
  ((SLH_DSA_SHA2_256SSIMPLE_FORS_HEIGHT + 1u) *                                                                   \
   (SLH_DSA_SHA2_256SSIMPLE_FORS_TREES * SLH_DSA_SHA2_256SSIMPLE_N))

/* Resulting SPX sizes. */
FSMSW_STATIC_ASSERT(SLH_DSA_SHA2_256SSIMPLE_BYTES ==
                    (SLH_DSA_SHA2_256SSIMPLE_N + SLH_DSA_SHA2_256SSIMPLE_FORS_BYTES +
                     (SLH_DSA_SHA2_256SSIMPLE_D * SLH_DSA_SHA2_256SSIMPLE_WOTS_BYTES) +
                     (SLH_DSA_SHA2_256SSIMPLE_FULL_HEIGHT * SLH_DSA_SHA2_256SSIMPLE_N)));

FSMSW_STATIC_ASSERT(SLH_DSA_SHA2_256SSIMPLE_PK_BYTES == (2u * SLH_DSA_SHA2_256SSIMPLE_N));
FSMSW_STATIC_ASSERT(SLH_DSA_SHA2_256SSIMPLE_SK_BYTES ==
                    (2 * SLH_DSA_SHA2_256SSIMPLE_N + SLH_DSA_SHA2_256SSIMPLE_PK_BYTES));

#define SLH_DSA_SHA2_256SSIMPLE_CRYPTO_SEEDBYTES (3u * SLH_DSA_SHA2_256SSIMPLE_N)

/* Defines for max array length */
#define SLH_DSA_SHA2_256SSIMPLE_MGF1_512_BUF_LEN   128u
#define SLH_DSA_SHA2_256SSIMPLE_THASH_BUF_LEN      67u
#define SLH_DSA_SHA2_256SSIMPLE_TREEHASH_BUF_LEN   1u /* Buffer with this length is currently not used */
#define SLH_DSA_SHA2_256SSIMPLE_TREEHASHX1_BUF_LEN 14u
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

#endif /* SLH_DSA_SHA2_256SSIMPLE_PARAMS_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
