/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Dilithium2
*    includes the modules for Dilithium2
 ** @{ */
/** \addtogroup FsmSw_Dilithium2_params
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Dilithium2_params.h
* \brief  Declarations for the modul FsmSw_Dilithium2_params.h
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
#ifndef FSMSW_DILITHIUM2_POLY_H
#define FSMSW_DILITHIUM2_POLY_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Dilithium2_params.h"
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
} poly_D2;

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
void FsmSw_Dilithium2_Poly_Reduce(poly_D2 *const a);
void FsmSw_Dilithium2_Poly_CAddQ(poly_D2 *const a);

void FsmSw_Dilithium2_Poly_Add(poly_D2 *const c, const poly_D2 *const a, const poly_D2 *const b);
void FsmSw_Dilithium2_Poly_Sub(poly_D2 *const c, const poly_D2 *const a, const poly_D2 *const b);
void FsmSw_Dilithium2_Poly_Shiftl(poly_D2 *const a);

void FsmSw_Dilithium2_Poly_Ntt(poly_D2 *a);
void FsmSw_Dilithium2_Poly_InvnttTomont(poly_D2 *a);
void FsmSw_Dilithium2_Poly_PointwiseMontgomery(poly_D2 *const c, const poly_D2 *const a, const poly_D2 *const b);

void FsmSw_Dilithium2_Poly_Power2Round(poly_D2 *const a1, poly_D2 *a0, const poly_D2 *const a);
void FsmSw_Dilithium2_Poly_Decompose(poly_D2 *const a1, poly_D2 *a0, const poly_D2 *const a);
uint32 FsmSw_Dilithium2_Poly_MakeHint(poly_D2 *const h, const poly_D2 *const a0, const poly_D2 *const a1);
void FsmSw_Dilithium2_Poly_UseHint(poly_D2 *const b, const poly_D2 *const a, const poly_D2 *const h);

sint8 FsmSw_Dilithium2_Poly_Chknorm(const poly_D2 *const a, sint32 B);
void FsmSw_Dilithium2_Poly_Uniform(poly_D2 *a, const uint8 seed[SEEDBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium2_Poly_UniformEta(poly_D2 *a, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium2_Poly_UniformGamma1(poly_D2 *const a, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium2_Poly_Challenge(poly_D2 *const c, const uint8 seed[SEEDBYTES_DILITHIUM]);

void FsmSw_Dilithium2_Polyeta_EtaPack(uint8 *const r, const poly_D2 *const a);
void FsmSw_Dilithium2_Polyeta_EtaUnpack(poly_D2 *const r, const uint8 *const a);

void FsmSw_Dilithium2_Poly_T1Pack(uint8 *const r, const poly_D2 *const a);
void FsmSw_Dilithium2_Poly_T1Unpack(poly_D2 *const r, const uint8 *const a);

void FsmSw_Dilithium2_Poly_T0Pack(uint8 *const r, const poly_D2 *const a);
void FsmSw_Dilithium2_Poly_T0Unpack(poly_D2 *const r, const uint8 *const a);

void FsmSw_Dilithium2_Poly_ZPack(uint8 *const r, const poly_D2 *const a);
void FsmSw_Dilithium2_Poly_ZUnpack(poly_D2 *const r, const uint8 *const a);

void FsmSw_Dilithium2_Poly_W1Pack(uint8 *const r, const poly_D2 *const a);

#endif /* FSMSW_DILITHIUM2_POLY_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */