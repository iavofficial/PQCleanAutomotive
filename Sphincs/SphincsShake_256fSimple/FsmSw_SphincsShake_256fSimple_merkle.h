/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsShake_256fSimple
*    includes the modules for SphincsShake_256fSimple
 ** @{ */
/** \addtogroup SphincsShake_256fSimple_merkle
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsShake_256fSimple_merkle.h
* \brief  Description of FsmSw_SphincsShake_256fSimple_merkle.h
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
#ifndef FSMSW_SPHINCSSHAKE_256FSIMPLE_MERKLE_H
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_MERKLE_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_SphincsShake_256fSimple_context.h"
#include "FsmSw_SphincsShake_256fSimple_params.h"
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
void FsmSw_SphincsShake_256fSimple_Merkle_Sign(uint8 *const sig, uint8 *const root,
                                               const sphincs_shake_256f_ctx *const ctx, const uint32 wots_addr[8],
                                               uint32 tree_addr[8], uint32 idx_leaf);

/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsShake_256fSimple_Merkle_GenRoot(uint8 *const root, const sphincs_shake_256f_ctx *const ctx);

#endif /* FSMSW_SPHINCSSHAKE_256FSIMPLE_MERKLE_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */