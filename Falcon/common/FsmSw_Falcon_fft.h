/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup common
*    includes the modules for common
 ** @{ */
/** \addtogroup Falcon_fft
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Falcon_fft.h
* \brief  description of FsmSw_Falcon_fft.h
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
#ifndef FSMSW_FALCON_FFT_H
#define FSMSW_FALCON_FFT_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Falcon_fpr.h"
#include "FsmSw_Types.h"
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
void FsmSw_Falcon_FFT(fpr *const f, uint32 logn);

void FsmSw_Falcon_IFFT(fpr *const f, uint32 logn);

void FsmSw_Falcon_Poly_Add(fpr *const a, const fpr *const b, uint32 logn);

void FsmSw_Falcon_Poly_Sub(fpr *const a, const fpr *const b, uint32 logn);

void FsmSw_Falcon_Poly_Neg(fpr *const a, uint32 logn);

void FsmSw_Falcon_Poly_AdjFFT(fpr *const a, uint32 logn);

void FsmSw_Falcon_Poly_MulFFT(fpr *const a, const fpr *const b, uint32 logn);

void FsmSw_Falcon_Poly_MuladjFFT(fpr *const a, const fpr *const b, uint32 logn);

void FsmSw_Falcon_Poly_MulselfadjFFT(fpr *const a, uint32 logn);

void FsmSw_Falcon_Poly_Mulconst(fpr *const a, fpr x, uint32 logn);

void FsmSw_Falcon_Poly_Invnorm2FFT(fpr *const d, const fpr *const a, const fpr *const b, uint32 logn);

void FsmSw_Falcon_Poly_Add_MuladjFFT(fpr *const d, const fpr *const F, const fpr *const G, const fpr *const f,
                                     const fpr *const g, uint32 logn);

void FsmSw_Falcon_Poly_Mul_AutoadjFFT(fpr *const a, const fpr *const b, uint32 logn);

void FsmSw_Falcon_Poly_Div_AutoadjFFT(fpr *const a, const fpr *const b, uint32 logn);

void FsmSw_Falcon_Poly_LdlFFT(const fpr *const g00, fpr *const g01, fpr *const g11, uint32 logn);

void FsmSw_Falcon_Poly_LdlMvFFT(fpr *const d11, fpr *const l10, const fpr *const g00, const fpr *const g01,
                                const fpr *const g11, uint32 logn);

void FsmSw_Falcon_Poly_SplitFFT(fpr *const f0, fpr *const f1, const fpr *const f, uint32 logn);

void FsmSw_Falcon_Poly_MergeFFT(fpr *const f, const fpr *const f0, const fpr *const f1, uint32 logn);

#endif /* FSMSW_FALCON_FFT_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */