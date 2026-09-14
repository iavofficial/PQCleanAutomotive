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
/** \addtogroup SLH_DSA_SHAKE_256fSimple
*    includes the modules for SLH_DSA_SHAKE_256fSimple
 ** @{ */
/** \addtogroup SLH_DSA_SHAKE_256fSimple_wotsx1
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHAKE_256fSimple_wotsx1.h
* \brief  Description of SLH_DSA_SHAKE_256fSimple_wotsx1.h
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
#ifndef SLH_DSA_SHAKE_256FSIMPLE_WOTSX1_H
#define SLH_DSA_SHAKE_256FSIMPLE_WOTSX1_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "SLH_DSA_SHAKE_256fSimple_context.h"
#include "SLH_DSA_SHAKE_256fSimple_params.h"
#include "Std_Types.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define SLH_DSA_SIGN_ADDR_SIZE 8
/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct
{
  uint8 *wots_sig;
  uint32 wots_sign_leaf; /* The index of the WOTS we're using to sign */
  uint32 *wots_steps;
  uint32 leaf_addr[SLH_DSA_SIGN_ADDR_SIZE];
  uint32 pk_addr[SLH_DSA_SIGN_ADDR_SIZE];
} SLH_DSA_SHAKE_256fSimple_LeafInfoX1_T;
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
void SLH_DSA_SHAKE_256fSimple_Wots_GenLeafX1(uint8 *const dest, const slh_dsa_shake_256f_ctx *const ctx,
                                                  uint32 leaf_idx, void *const v_info);

#endif /* SLH_DSA_SHAKE_256FSIMPLE_WOTSX1_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
