/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsShake_192sSimple
*    includes the modules for SphincsShake_192sSimple
 ** @{ */
/** \addtogroup SphincsShake_192sSimple_context
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsShake_192sSimple_context.h
* \brief  Description of FsmSw_SphincsShake_192sSimple_context.h
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
#ifndef FSMSW_SPHINCSSHAKE_192SSIMPLE_CONTEXT_H
#define FSMSW_SPHINCSSHAKE_192SSIMPLE_CONTEXT_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_SphincsShake_192sSimple_params.h"
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
  uint8 pub_seed[FSMSW_SPHINCSSHAKE_192SSIMPLE_N];
  uint8 sk_seed[FSMSW_SPHINCSSHAKE_192SSIMPLE_N];

} sphincs_shake_192s_ctx;
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
void FsmSw_SphincsShake_192sSimple_InitializeHashFunction(sphincs_shake_192s_ctx *const ctx);

void FsmSw_SphincsShake_192sSimple_4FreeHashFunction(sphincs_shake_192s_ctx *const ctx);

#endif /* FSMSW_SPHINCSSHAKE_192SSIMPLE_CONTEXT_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */