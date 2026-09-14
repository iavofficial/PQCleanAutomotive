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
/** \addtogroup ML_DSA_65
*    includes the modules for ML_DSA_65
 ** @{ */
/** \addtogroup ML_DSA_65_packing
 ** @{ */

/*====================================================================================================================*/
/** \file ML_DSA_65_packing.h
* \brief  Description of the ML_DSA_65_packing.h
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
#ifndef FSMSW_ML_DSA_65_PACKING_H
#define FSMSW_ML_DSA_65_PACKING_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "ML_DSA_65_params.h"
#include "ML_DSA_65_polyvec.h"
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
void ML_DSA_65_PackPk(uint8 pk[FSMSW_ML_DSA_65_CRYPTO_PUBLICKEYBYTES], const uint8 rho[SEEDBYTES_ML_DSA],
                             const polyveck_D3 *const t1);

void ML_DSA_65_PackSk(uint8 sk[FSMSW_ML_DSA_65_CRYPTO_SECRETKEYBYTES], const uint8 rho[SEEDBYTES_ML_DSA],
                             const uint8 tr[TRBYTES_ML_DSA], const uint8 key[SEEDBYTES_ML_DSA],
                             const polyveck_D3 *const t0, const polyvecl_D3 *const s1, const polyveck_D3 *const s2);

void ML_DSA_65_PackSig(uint8 sig[FSMSW_ML_DSA_65_CRYPTO_BYTES], const uint8 c[CTILDEBYTES_ML_DSA_65],
                              const polyvecl_D3 *const z, const polyveck_D3 *const h);

void ML_DSA_65_UnpackPk(uint8 rho[SEEDBYTES_ML_DSA], polyveck_D3 *t1,
                               const uint8 pk[FSMSW_ML_DSA_65_CRYPTO_PUBLICKEYBYTES]);

void ML_DSA_65_UnpackSk(uint8 rho[SEEDBYTES_ML_DSA], uint8 tr[TRBYTES_ML_DSA],
                               uint8 key[SEEDBYTES_ML_DSA], polyveck_D3 *t0, polyvecl_D3 *s1, polyveck_D3 *s2,
                               const uint8 sk[FSMSW_ML_DSA_65_CRYPTO_SECRETKEYBYTES]);

sint8 ML_DSA_65_UnpackSig(uint8 c[CTILDEBYTES_ML_DSA_65], polyvecl_D3 *z, polyveck_D3 *const h,
                                 const uint8 sig[FSMSW_ML_DSA_65_CRYPTO_BYTES]);

#endif /* FSMSW_ML_DSA_65_PACKING_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */