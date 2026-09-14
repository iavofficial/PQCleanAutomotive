/***********************************************************************************************************************
 *
 * Original implementation: PQClean, FN_DSA
 *
 * Copyright (c) 2017-2019 FN_DSA Project
 * Copyright 2026 IAV GmbH
 *
 * Original portions are licensed under the MIT License.
 * IAV modifications are licensed under the Apache License, Version 2.0.
 *
 * SPDX-License-Identifier: MIT AND Apache-2.0
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FN_DSA
*    includes the modules for SwC FN_DSA
 ** @{ */
/** \addtogroup common
*    includes the modules for common
 ** @{ */
/** \addtogroup FN_DSA_fft
 ** @{ */

/*====================================================================================================================*/
/** \file FN_DSA_fft.h
* \brief  description of FN_DSA_fft.h
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
#ifndef FN_DSA_FFT_H
#define FN_DSA_FFT_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FN_DSA_fpr.h"
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
void FN_DSA_FFT(fpr *const f, uint32 logn);

void FN_DSA_IFFT(fpr *const f, uint32 logn);

void FN_DSA_Poly_Add(fpr *const a, const fpr *const b, uint32 logn);

void FN_DSA_Poly_Sub(fpr *const a, const fpr *const b, uint32 logn);

void FN_DSA_Poly_Neg(fpr *const a, uint32 logn);

void FN_DSA_Poly_AdjFFT(fpr *const a, uint32 logn);

void FN_DSA_Poly_MulFFT(fpr *const a, const fpr *const b, uint32 logn);

void FN_DSA_Poly_MuladjFFT(fpr *const a, const fpr *const b, uint32 logn);

void FN_DSA_Poly_MulselfadjFFT(fpr *const a, uint32 logn);

void FN_DSA_Poly_Mulconst(fpr *const a, fpr x, uint32 logn);

void FN_DSA_Poly_Invnorm2FFT(fpr *const d, const fpr *const a, const fpr *const b, uint32 logn);

void FN_DSA_Poly_Add_MuladjFFT(fpr *const d, const fpr *const F, const fpr *const G, const fpr *const f,
                                     const fpr *const g, uint32 logn);

void FN_DSA_Poly_Mul_AutoadjFFT(fpr *const a, const fpr *const b, uint32 logn);

void FN_DSA_Poly_Div_AutoadjFFT(fpr *const a, const fpr *const b, uint32 logn);

void FN_DSA_Poly_LdlFFT(const fpr *const g00, fpr *const g01, fpr *const g11, uint32 logn);

void FN_DSA_Poly_LdlMvFFT(fpr *const d11, fpr *const l10, const fpr *const g00, const fpr *const g01,
                                const fpr *const g11, uint32 logn);

void FN_DSA_Poly_SplitFFT(fpr *const f0, fpr *const f1, const fpr *const f, uint32 logn);

void FN_DSA_Poly_MergeFFT(fpr *const f, const fpr *const f0, const fpr *const f1, uint32 logn);

#endif /* FN_DSA_FFT_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */