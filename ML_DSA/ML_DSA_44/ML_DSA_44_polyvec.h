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
/** \addtogroup ML_DSA_44_polyvec
 ** @{ */

/*====================================================================================================================*/
/** \file ML_DSA_44_polyvec.h
* \brief  Description of the ML_DSA_44_polyvec.h
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
#ifndef FSMSW_ML_DSA_44_POLYVEC_H
#define FSMSW_ML_DSA_44_POLYVEC_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "ML_DSA_44_params.h"
#include "ML_DSA_44_poly.h"
#include "Std_Types.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct
{
  poly_D2 vec[L_ML_DSA_44];
} polyvecl_D2;

/* Vectors of polynomials of length K */
typedef struct
{
  poly_D2 vec[K_ML_DSA_44];
} polyveck_D2;

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
void ML_DSA_44_Polyvecl_UniformEta(polyvecl_D2 *v, const uint8 seed[CRHBYTES_ML_DSA], uint16 nonce);
void ML_DSA_44_Polyvecl_UniformGamma1(polyvecl_D2 *v, const uint8 seed[CRHBYTES_ML_DSA], uint16 nonce);
void ML_DSA_44_Polyvecl_Reduce(polyvecl_D2 *v);
void ML_DSA_44_Polyvecl_Add(polyvecl_D2 *w, const polyvecl_D2 *const u, const polyvecl_D2 *const v);
void ML_DSA_44_Polyvecl_Ntt(polyvecl_D2 *v);
void ML_DSA_44_Polyvecl_InvnttTomont(polyvecl_D2 *v);
void ML_DSA_44_Polyvecl_PointwisePolyMontgomery(polyvecl_D2 *r, const poly_D2 *const a,
                                                       const polyvecl_D2 *const v);
sint8 ML_DSA_44_Polyvecl_Chknorm(const polyvecl_D2 *const v, sint32 bound);
void ML_DSA_44_Polyveck_UniformEta(polyveck_D2 *v, const uint8 seed[CRHBYTES_ML_DSA], uint16 nonce);
void ML_DSA_44_Polyveck_Reduce(polyveck_D2 *v);
void ML_DSA_44_Polyveck_CAddQ(polyveck_D2 *v);
void ML_DSA_44_Polyveck_Add(polyveck_D2 *w, const polyveck_D2 *const u, const polyveck_D2 *const v);
void ML_DSA_44_Polyveck_Sub(polyveck_D2 *w, const polyveck_D2 *const u, const polyveck_D2 *const v);
void ML_DSA_44_Polyveck_Shiftl(polyveck_D2 *v);
void ML_DSA_44_Polyveck_Ntt(polyveck_D2 *v);
void ML_DSA_44_Polyveck_InvnttTomont(polyveck_D2 *v);
void ML_DSA_44_Polyveck_PointwisePolyMontgomery(polyveck_D2 *r, const poly_D2 *const a,
                                                       const polyveck_D2 *const v);
sint8 ML_DSA_44_Polyveck_Chknorm(const polyveck_D2 *const v, sint32 bound);
void ML_DSA_44_Polyveck_Power2Round(polyveck_D2 *v1, polyveck_D2 *v0, const polyveck_D2 *const v);
void ML_DSA_44_Polyveck_Decompose(polyveck_D2 *v1, polyveck_D2 *v0, const polyveck_D2 *const v);
uint32 ML_DSA_44_Polyveck_MakeHint(polyveck_D2 *h, const polyveck_D2 *const v0, const polyveck_D2 *const v1);
void ML_DSA_44_Polyveck_UseHint(polyveck_D2 *w, const polyveck_D2 *const v, const polyveck_D2 *const h);
void ML_DSA_44_Polyveck_PackW1(uint8 r[K_ML_DSA_44 * POLYW1_PACKEDBYTES_ML_DSA_44],
                                      const polyveck_D2 *const w1);
void ML_DSA_44_Polyvec_MatrixExpand(polyvecl_D2 mat[K_ML_DSA_44], const uint8 rho[SEEDBYTES_ML_DSA]);
void ML_DSA_44_Polyvec_MatrixPointwiseMontgomery(polyveck_D2 *t, const polyvecl_D2 mat[K_ML_DSA_44],
                                                        const polyvecl_D2 *const v);

#endif /* FSMSW_ML_DSA_44_POLYVEC_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
