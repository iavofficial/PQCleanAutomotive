/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsShake_128sSimple
*    includes the modules for SphincsShake_128sSimple
 ** @{ */
/** \addtogroup SphincsShake_128sSimple_context
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsShake_128sSimple_context.h
* \brief  Description of FsmSw_SphincsShake_128sSimple_context.h
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
#ifndef FSMSW_SPHINCSSHAKE_128SSIMPLE_CONTEXT_H
#define FSMSW_SPHINCSSHAKE_128SSIMPLE_CONTEXT_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_SphincsShake_128sSimple_params.h"
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
  uint8 pub_seed[FSMSW_SPHINCSSHAKE_128SSIMPLE_N];
  uint8 sk_seed[FSMSW_SPHINCSSHAKE_128SSIMPLE_N];

} sphincs_shake_128s_ctx;
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
void FsmSw_SphincsShake_128sSimple_InitializeHashFunction(sphincs_shake_128s_ctx *ctx);

void FsmSw_SphincsShake_128sSimple_1FreeHashFunction(sphincs_shake_128s_ctx *ctx);

#endif /* FSMSW_SPHINCSSHAKE_128SSIMPLE_CONTEXT_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */