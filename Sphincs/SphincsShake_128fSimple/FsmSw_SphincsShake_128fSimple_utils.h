/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsShake_128fSimple
*    includes the modules for SphincsShake_128fSimple
 ** @{ */
/** \addtogroup SphincsShake_128fSimple_utils
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsShake_128fSimple_utils.h
* \brief  Description of FsmSw_SphincsShake_128fSimple_utils.h
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
#ifndef FSMSW_SPHINCSSHAKE_128FSIMPLE_UTILS_H
#define FSMSW_SPHINCSSHAKE_128FSIMPLE_UTILS_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_SphincsShake_128fSimple_context.h"
#include "FsmSw_SphincsShake_128fSimple_params.h"
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
void FsmSw_SphincsShake_128fSimple_ComputeRoot(uint8 *root, const uint8 *leaf, uint32 leaf_idx, uint32 idx_offset,
                                               const uint8 *auth_path, uint32 tree_height,
                                               const sphincs_shake_128f_ctx *ctx, uint32 addr[8]);

/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsShake_128fSimple_TreeHash(uint8 *root, uint8 *auth_path, const sphincs_shake_128f_ctx *ctx,
                                            uint32 leaf_idx, uint32 idx_offset, uint32 tree_height,
                                            void (*gen_leaf)(uint8 *leaf, const sphincs_shake_128f_ctx *ctx,
                                                             uint32 addr_idx, const uint32 tree_addr[8]),
                                            uint32 tree_addr[8]);

#endif /* FSMSW_SPHINCSSHAKE_128FSIMPLE_UTILS_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */