/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsSha2_192fSimple
*    includes the modules for SphincsSha2_192fSimple
 ** @{ */
/** \addtogroup SphincsSha2_192fSimple_wots
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsSha2_192fSimple_wots.h
* \brief  Description of FsmSw_SphincsSha2_192fSimple_wots.h
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
#ifndef FSMSW_SPHINCSSHA2_192FSIMPLE_WOTS_H
#define FSMSW_SPHINCSSHA2_192FSIMPLE_WOTS_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_SphincsSha2_192fSimple_context.h"
#include "FsmSw_SphincsSha2_192fSimple_params.h"
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
/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsSha2_192fSimple_Wots_PkFromSig(uint8 *const pk, const uint8 *const sig, const uint8 *const msg,
                                                 const sphincs_sha2_192f_ctx *const ctx, uint32 addr[8]);
void FsmSw_SphincsSha2_192fSimple_Wots_ChainLengths(uint32 *const lengths, const uint8 *const msg);

#endif /* FSMSW_SPHINCSSHA2_192FSIMPLE_WOTS_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */