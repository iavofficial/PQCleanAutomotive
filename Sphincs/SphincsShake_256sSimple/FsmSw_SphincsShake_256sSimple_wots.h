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
/** \addtogroup SphincsShake_256sSimple_wots
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsShake_256sSimple_wots.h
* \brief  Description of FsmSw_SphincsShake_256sSimple_wots.h
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
#ifndef FSMSW_SPHINCSSHAKE_256SSIMPLE_WOTS_H
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_WOTS_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_SphincsShake_256sSimple_context.h"
#include "FsmSw_SphincsShake_256sSimple_params.h"
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
void FsmSw_SphincsShake_256sSimple_Wots_PkFromSig(uint8 *const pk, const uint8 *const sig, const uint8 *const msg,
                                                  const sphincs_shake_256s_ctx *const ctx, uint32 addr[8]);

void FsmSw_SphincsShake_256sSimple_Wots_ChainLengths(uint32 *const lengths, const uint8 *const msg);

#endif /* FSMSW_SPHINCSSHAKE_256SSIMPLE_WOTS_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */