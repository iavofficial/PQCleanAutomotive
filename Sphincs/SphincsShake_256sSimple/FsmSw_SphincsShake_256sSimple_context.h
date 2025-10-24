/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsShake_256sSimple
*    includes the modules for SphincsShake_256sSimple
 ** @{ */
/** \addtogroup SphincsShake_256sSimple_context
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsShake_256sSimple_context.h
* \brief  Description of FsmSw_SphincsShake_256sSimple_context.h
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
#ifndef FSMSW_SPHINCSSHAKE_256SSIMPLE_CONTEXT_H
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_CONTEXT_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_SphincsShake_256sSimple_params.h"
#include "FsmSw_Types.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
/* polyspace +5 CERT-C:DCL12-C [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* polyspace +3 MISRA2012:D4.8 [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
typedef struct
{
  uint8 pub_seed[FSMSW_SPHINCSSHAKE_256SSIMPLE_N];
  uint8 sk_seed[FSMSW_SPHINCSSHAKE_256SSIMPLE_N];

} sphincs_shake_256s_ctx;
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
void FsmSw_SphincsShake_256sSimple_InitializeHashFunction(sphincs_shake_256s_ctx *ctx);

void FsmSw_SphincsShake_256sSimple_1FreeHashFunction(sphincs_shake_256s_ctx *ctx);

#endif /* FSMSW_SPHINCSSHAKE_256SSIMPLE_CONTEXT_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */