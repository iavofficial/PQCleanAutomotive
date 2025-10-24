/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsSha2_256sSimple
*    includes the modules for SphincsSha2_256sSimple
 ** @{ */
/** \addtogroup SphincsSha2_256sSimple_fors
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsSha2_256sSimple_fors.h
* \brief  Description of FsmSw_SphincsSha2_256sSimple_fors.h
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
#ifndef FSMSW_SPHINCSSHA2_256SSIMPLE_FORS_H
#define FSMSW_SPHINCSSHA2_256SSIMPLE_FORS_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_SphincsSha2_256sSimple_context.h"
#include "FsmSw_SphincsSha2_256sSimple_params.h"
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
void FsmSw_SphincsSha2_256sSimple_Fors_Sign(uint8 *sig, uint8 *pk, const uint8 *m, const sphincs_sha2_256s_ctx *ctx,
                                            const uint32 fors_addr[8]);

/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsSha2_256sSimple_Fors_PkFromSig(uint8 *pk, const uint8 *sig, const uint8 *m,
                                                 const sphincs_sha2_256s_ctx *ctx, const uint32 fors_addr[8]);

#endif /* FSMSW_SPHINCSSHA2_256SSIMPLE_FORS_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */