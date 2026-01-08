/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsSha2_256fSimple
*    includes the modules for SphincsSha2_256fSimple
 ** @{ */
/** \addtogroup SphincsSha2_256fSimple_context
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsSha2_256fSimple_context.h
* \brief  Description of FsmSw_SphincsSha2_256fSimple_context.h
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
#ifndef FSMSW_SPHINCSSHA2_256FSIMPLE_CONTEXT_H
#define FSMSW_SPHINCSSHA2_256FSIMPLE_CONTEXT_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_SphincsSha2_256fSimple_params.h"
#include "FsmSw_Types.h"
#include "FsmSw_sha2.h"
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
  uint8 pub_seed[FSMSW_SPHINCSSHA2_256FSIMPLE_N];
  uint8 sk_seed[FSMSW_SPHINCSSHA2_256FSIMPLE_N];
  /* sha256 state that absorbed pub_seed */
  sha256ctx state_seeded;
  /* sha512 state that absorbed pub_seed */
  sha512ctx state_seeded_512;

} sphincs_sha2_256f_ctx;
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
void FsmSw_SphincsSha2_256fSimple_InitializeHashFunction(sphincs_sha2_256f_ctx *const ctx);

#endif /* FSMSW_SPHINCSSHA2_256FSIMPLE_CONTEXT_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */