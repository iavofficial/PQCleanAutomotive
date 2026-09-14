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
/** \addtogroup SLH_DSA_SHAKE_192fSimple
*    includes the modules for SLH_DSA_SHAKE_192fSimple
 ** @{ */
/** \addtogroup SLH_DSA_SHAKE_192fSimple_fors
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHAKE_192fSimple_fors.h
* \brief  Description of SLH_DSA_SHAKE_192fSimple_fors.h
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
#ifndef SLH_DSA_SHAKE_192FSIMPLE_FORS_H
#define SLH_DSA_SHAKE_192FSIMPLE_FORS_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "SLH_DSA_SHAKE_192fSimple_context.h"
#include "SLH_DSA_SHAKE_192fSimple_params.h"
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
void SLH_DSA_SHAKE_192fSimple_Fors_Sign(uint8 *const sig, uint8 *const pk, const uint8 *const m,
                                             const slh_dsa_shake_192f_ctx *const ctx, const uint32 fors_addr[8]);

void SLH_DSA_SHAKE_192fSimple_Fors_PkFromSig(uint8 *const pk, const uint8 *const sig, const uint8 *const m,
                                                  const slh_dsa_shake_192f_ctx *const ctx, const uint32 fors_addr[8]);

#endif /* SLH_DSA_SHAKE_192FSIMPLE_FORS_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
