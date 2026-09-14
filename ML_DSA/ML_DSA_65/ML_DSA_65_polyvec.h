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
/** \addtogroup ML_DSA_65_polyvec
 ** @{ */

/*====================================================================================================================*/
/** \file ML_DSA_65_polyvec.h
* \brief  Description of the ML_DSA_65_polyvec.h
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
#ifndef FSMSW_ML_DSA_65_POLYVEC_H
#define FSMSW_ML_DSA_65_POLYVEC_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "ML_DSA_65_params.h"
#include "ML_DSA_65_poly.h"
#include "Std_Types.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct
{
  poly_D3 vec[L_ML_DSA_65];
} polyvecl_D3;

/* Vectors of polynomials of length K */
typedef struct
{
  poly_D3 vec[K_ML_DSA_65];
} polyveck_D3;
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
void ML_DSA_65_Polyvecl_UniformEta(polyvecl_D3 *v, const uint8 seed[CRHBYTES_ML_DSA], uint16 nonce);
void ML_DSA_65_Polyvecl_UniformGamma1(polyvecl_D3 *v, const uint8 seed[CRHBYTES_ML_DSA], uint16 nonce);
void ML_DSA_65_Polyvecl_Reduce(polyvecl_D3 *v);
void ML_DSA_65_Polyvecl_Add(polyvecl_D3 *w, const polyvecl_D3 *const u, const polyvecl_D3 *const v);
void ML_DSA_65_Polyvecl_Ntt(polyvecl_D3 *v);
void ML_DSA_65_Polyvecl_InvnttTomont(polyvecl_D3 *v);
void ML_DSA_65_Polyvecl_PointwisePolyMontgomery(polyvecl_D3 *r, const poly_D3 *const a,
                                                       const polyvecl_D3 *const v);
sint8 ML_DSA_65_Polyvecl_Chknorm(const polyvecl_D3 *const v, sint32 bound);
void ML_DSA_65_Polyveck_UniformEta(polyveck_D3 *v, const uint8 seed[CRHBYTES_ML_DSA], uint16 nonce);
void ML_DSA_65_Polyveck_Reduce(polyveck_D3 *v);
void ML_DSA_65_Polyveck_CAddQ(polyveck_D3 *v);
void ML_DSA_65_Polyveck_Add(polyveck_D3 *w, const polyveck_D3 *const u, const polyveck_D3 *const v);
void ML_DSA_65_Polyveck_Sub(polyveck_D3 *w, const polyveck_D3 *const u, const polyveck_D3 *const v);
void ML_DSA_65_Polyveck_Shiftl(polyveck_D3 *v);
void ML_DSA_65_Polyveck_Ntt(polyveck_D3 *v);
void ML_DSA_65_Polyveck_InvnttTomont(polyveck_D3 *v);
void ML_DSA_65_Polyveck_PointwisePolyMontgomery(polyveck_D3 *r, const poly_D3 *const a,
                                                       const polyveck_D3 *const v);
sint8 ML_DSA_65_Polyveck_Chknorm(const polyveck_D3 *const v, sint32 bound);
void ML_DSA_65_Polyveck_Power2Round(polyveck_D3 *v1, polyveck_D3 *v0, const polyveck_D3 *const v);
void ML_DSA_65_Polyveck_Decompose(polyveck_D3 *v1, polyveck_D3 *v0, const polyveck_D3 *const v);
uint32 ML_DSA_65_Polyveck_MakeHint(polyveck_D3 *h, const polyveck_D3 *const v0, const polyveck_D3 *const v1);
void ML_DSA_65_Polyveck_UseHint(polyveck_D3 *w, const polyveck_D3 *const v, const polyveck_D3 *const h);
void ML_DSA_65_Polyveck_PackW1(uint8 r[K_ML_DSA_65 * POLYW1_PACKEDBYTES_ML_DSA_65],
                                      const polyveck_D3 *const w1);
void ML_DSA_65_Polyvec_MatrixExpand(polyvecl_D3 mat[K_ML_DSA_65], const uint8 rho[SEEDBYTES_ML_DSA]);
void ML_DSA_65_Polyvec_MatrixPointwiseMontgomery(polyveck_D3 *t, const polyvecl_D3 mat[K_ML_DSA_65],
                                                        const polyvecl_D3 *const v);

#endif /* FSMSW_ML_DSA_65_POLYVEC_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
