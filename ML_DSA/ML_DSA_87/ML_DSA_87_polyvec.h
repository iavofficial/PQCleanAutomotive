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
/** \addtogroup ML_DSA_87_polyvec
 ** @{ */

/*====================================================================================================================*/
/** \file ML_DSA_87_polyvec.h
* \brief  Description of the ML_DSA_87_polyvec.h
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
#ifndef FSMSW_ML_DSA_87_POLYVEC_H
#define FSMSW_ML_DSA_87_POLYVEC_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "ML_DSA_87_params.h"
#include "ML_DSA_87_poly.h"
#include "Std_Types.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct
{
  poly_D5 vec[L_ML_DSA_87];
} polyvecl_D5;

/* Vectors of polynomials of length K */
typedef struct
{
  poly_D5 vec[K_ML_DSA_87];
} polyveck_D5;
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
void ML_DSA_87_Polyvecl_UniformEta(polyvecl_D5 *v, const uint8 seed[CRHBYTES_ML_DSA], uint16 nonce);
void ML_DSA_87_Polyvecl_UniformGamma1(polyvecl_D5 *v, const uint8 seed[CRHBYTES_ML_DSA], uint16 nonce);
void ML_DSA_87_Polyvecl_Reduce(polyvecl_D5 *v);
void ML_DSA_87_Polyvecl_Add(polyvecl_D5 *w, const polyvecl_D5 *const u, const polyvecl_D5 *const v);
void ML_DSA_87_Polyvecl_Ntt(polyvecl_D5 *v);
void ML_DSA_87_Polyvecl_InvnttTomont(polyvecl_D5 *v);
void ML_DSA_87_Polyvecl_PointwisePolyMontgomery(polyvecl_D5 *r, const poly_D5 *const a,
                                                       const polyvecl_D5 *const v);
sint8 ML_DSA_87_Polyvecl_Chknorm(const polyvecl_D5 *const v, sint32 bound);
void ML_DSA_87_Polyveck_UniformEta(polyveck_D5 *v, const uint8 seed[CRHBYTES_ML_DSA], uint16 nonce);
void ML_DSA_87_Polyveck_Reduce(polyveck_D5 *v);
void ML_DSA_87_Polyveck_CAddQ(polyveck_D5 *v);
void ML_DSA_87_Polyveck_Add(polyveck_D5 *w, const polyveck_D5 *const u, const polyveck_D5 *const v);
void ML_DSA_87_Polyveck_Sub(polyveck_D5 *w, const polyveck_D5 *const u, const polyveck_D5 *const v);
void ML_DSA_87_Polyveck_Shiftl(polyveck_D5 *v);
void ML_DSA_87_Polyveck_Ntt(polyveck_D5 *v);
void ML_DSA_87_Polyveck_InvnttTomont(polyveck_D5 *v);
void ML_DSA_87_Polyveck_PointwisePolyMontgomery(polyveck_D5 *r, const poly_D5 *const a,
                                                       const polyveck_D5 *const v);
sint8 ML_DSA_87_Polyveck_Chknorm(const polyveck_D5 *const v, sint32 bound);
void ML_DSA_87_Polyveck_Power2Round(polyveck_D5 *v1, polyveck_D5 *v0, const polyveck_D5 *const v);
void ML_DSA_87_Polyveck_Decompose(polyveck_D5 *v1, polyveck_D5 *v0, const polyveck_D5 *const v);
uint32 ML_DSA_87_Polyveck_MakeHint(polyveck_D5 *h, const polyveck_D5 *const v0, const polyveck_D5 *const v1);
void ML_DSA_87_Polyveck_UseHint(polyveck_D5 *w, const polyveck_D5 *const v, const polyveck_D5 *const h);
void ML_DSA_87_Polyveck_PackW1(uint8 r[K_ML_DSA_87 * POLYW1_PACKEDBYTES_ML_DSA_87],
                                      const polyveck_D5 *const w1);
void ML_DSA_87_Polyvec_MatrixExpand(polyvecl_D5 mat[K_ML_DSA_87], const uint8 rho[SEEDBYTES_ML_DSA]);
void ML_DSA_87_Polyvec_MatrixPointwiseMontgomery(polyveck_D5 *t, const polyvecl_D5 mat[K_ML_DSA_87],
                                                        const polyvecl_D5 *const v);

#endif /* FSMSW_ML_DSA_87_POLYVEC_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
