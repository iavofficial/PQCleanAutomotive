/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Dilithium3
*    includes the modules for Dilithium3
 ** @{ */
/** \addtogroup FsmSw_Dilithium3_poly
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Dilithium3_poly.h
* \brief  Description of the FsmSw_Dilithium3_poly.h
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
#ifndef FSMSW_DILITHIUM3_POLY_H
#define FSMSW_DILITHIUM3_POLY_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Dilithium3_params.h"
#include "FsmSw_Types.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct
{
  sint32 coeffs[N_DILITHIUM];
} poly_D3;
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
void FsmSw_Dilithium3_Poly_Reduce(poly_D3 *const a);
void FsmSw_Dilithium3_Poly_CAddQ(poly_D3 *const a);

void FsmSw_Dilithium3_Poly_Add(poly_D3 *const c, const poly_D3 *const a, const poly_D3 *const b);
void FsmSw_Dilithium3_Poly_Sub(poly_D3 *const c, const poly_D3 *const a, const poly_D3 *const b);
void FsmSw_Dilithium3_Poly_Shiftl(poly_D3 *const a);

void FsmSw_Dilithium3_Poly_Ntt(poly_D3 *a);
void FsmSw_Dilithium3_Poly_InvnttTomont(poly_D3 *a);
void FsmSw_Dilithium3_Poly_PointwiseMontgomery(poly_D3 *const c, const poly_D3 *const a, const poly_D3 *const b);

void FsmSw_Dilithium3_Poly_Power2Round(poly_D3 *const a1, poly_D3 *a0, const poly_D3 *const a);
void FsmSw_Dilithium3_Poly_Decompose(poly_D3 *const a1, poly_D3 *a0, const poly_D3 *const a);
uint32 FsmSw_Dilithium3_Poly_MakeHint(poly_D3 *const h, const poly_D3 *const a0, const poly_D3 *const a1);
void FsmSw_Dilithium3_Poly_UseHint(poly_D3 *const b, const poly_D3 *const a, const poly_D3 *const h);

sint8 FsmSw_Dilithium3_Poly_Chknorm(const poly_D3 *const a, sint32 B);
void FsmSw_Dilithium3_Poly_Uniform(poly_D3 *a, const uint8 seed[SEEDBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium3_Poly_UniformEta(poly_D3 *a, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium3_Poly_UniformGamma1(poly_D3 *const a, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium3_Poly_Challenge(poly_D3 *const c, const uint8 seed[SEEDBYTES_DILITHIUM]);

void FsmSw_Dilithium3_Polyeta_EtaPack(uint8 *const r, const poly_D3 *const a);
void FsmSw_Dilithium3_Polyeta_EtaUnpack(poly_D3 *const r, const uint8 *const a);

void FsmSw_Dilithium3_Poly_T1Pack(uint8 *const r, const poly_D3 *const a);
void FsmSw_Dilithium3_Poly_T1Unpack(poly_D3 *const r, const uint8 *const a);

void FsmSw_Dilithium3_Poly_T0Pack(uint8 *const r, const poly_D3 *const a);
void FsmSw_Dilithium3_Poly_T0Unpack(poly_D3 *const r, const uint8 *const a);

void FsmSw_Dilithium3_Poly_ZPack(uint8 *const r, const poly_D3 *const a);
void FsmSw_Dilithium3_Poly_ZUnpack(poly_D3 *const r, const uint8 *const a);

void FsmSw_Dilithium3_Poly_W1Pack(uint8 *const r, const poly_D3 *const a);

#endif /* FSMSW_DILITHIUM3_POLY_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */