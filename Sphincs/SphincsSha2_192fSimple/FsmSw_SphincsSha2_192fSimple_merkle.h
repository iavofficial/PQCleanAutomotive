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
/** \addtogroup SphincsSha2_192fSimple_merkle
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsSha2_192fSimple_merkle.h
* \brief  Description of FsmSw_SphincsSha2_192fSimple_merkle.h
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
#ifndef FSMSW_SPHINCSSHA2_192FSIMPLE_MERKLE_H
#define FSMSW_SPHINCSSHA2_192FSIMPLE_MERKLE_H
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
void FsmSw_SphincsSha2_192fSimple_Merkle_Sign(uint8 *const sig, uint8 *const root,
                                              const sphincs_sha2_192f_ctx *const ctx, const uint32 wots_addr[8],
                                              uint32 tree_addr[8], uint32 idx_leaf);

/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsSha2_192fSimple_Merkle_GenRoot(uint8 *const root, const sphincs_sha2_192f_ctx *const ctx);

#endif /* FSMSW_SPHINCSSHA2_192FSIMPLE_MERKLE_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
