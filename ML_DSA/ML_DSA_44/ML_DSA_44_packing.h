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
/** \addtogroup ML_DSA_44
*    includes the modules for ML_DSA_44
 ** @{ */
/** \addtogroup ML_DSA_44_packing
 ** @{ */

/*====================================================================================================================*/
/** \file ML_DSA_44_packing.h
* \brief  Declarations for the modul ML_DSA_44_packing.c
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
#ifndef FSMSW_ML_DSA_44_PACKING_H
#define FSMSW_ML_DSA_44_PACKING_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "ML_DSA_44_params.h"
#include "ML_DSA_44_polyvec.h"
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
void ML_DSA_44_PackPk(uint8 pk[FSMSW_ML_DSA_44_CRYPTO_PUBLICKEYBYTES], const uint8 rho[SEEDBYTES_ML_DSA],
                             const polyveck_D2 *const t1);

void ML_DSA_44_PackSk(uint8 sk[FSMSW_ML_DSA_44_CRYPTO_SECRETKEYBYTES], const uint8 rho[SEEDBYTES_ML_DSA],
                             const uint8 tr[TRBYTES_ML_DSA], const uint8 key[SEEDBYTES_ML_DSA],
                             const polyveck_D2 *const t0, const polyvecl_D2 *const s1, const polyveck_D2 *const s2);

void ML_DSA_44_PackSig(uint8 sig[FSMSW_ML_DSA_44_CRYPTO_BYTES], const uint8 c[CTILDEBYTES_ML_DSA_44],
                              const polyvecl_D2 *const z, const polyveck_D2 *const h);

void ML_DSA_44_UnpackPk(uint8 rho[SEEDBYTES_ML_DSA], polyveck_D2 *t1,
                               const uint8 pk[FSMSW_ML_DSA_44_CRYPTO_PUBLICKEYBYTES]);

void ML_DSA_44_UnpackSk(uint8 rho[SEEDBYTES_ML_DSA], uint8 tr[TRBYTES_ML_DSA],
                               uint8 key[SEEDBYTES_ML_DSA], polyveck_D2 *t0, polyvecl_D2 *s1, polyveck_D2 *s2,
                               const uint8 sk[FSMSW_ML_DSA_44_CRYPTO_SECRETKEYBYTES]);

sint8 ML_DSA_44_UnpackSig(uint8 c[CTILDEBYTES_ML_DSA_44], polyvecl_D2 *z, polyveck_D2 *const h,
                                 const uint8 sig[FSMSW_ML_DSA_44_CRYPTO_BYTES]);

#endif /* FSMSW_ML_DSA_44_PACKING_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */