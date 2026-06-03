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
/** \addtogroup SphincsShake_192sSimple_utilsx1
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsShake_192sSimple_utilsx1.h
* \brief  Description of FsmSw_SphincsShake_192sSimple_utilsx1.h
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
#ifndef FSMSW_SPHINCSSHAKE_192SSIMPLE_UTILSX4_H
#define FSMSW_SPHINCSSHAKE_192SSIMPLE_UTILSX4_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_SphincsShake_192sSimple_context.h"
#include "FsmSw_SphincsShake_192sSimple_params.h"
#include "Std_Types.h"
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
/* polyspace +4 CERT-C:DCL23-C [Justified:]
 * polyspace +3 ISO-17961:funcdecl [Justified:]
 * polyspace +2 MISRA2012:5.1 [Justified:] "The identifiers are distinct. The naming convention ensures clarity
 and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsShake_192sSimple_TreeHashX1(uint8 *const root, uint8 *const auth_path,
                                              const sphincs_shake_192s_ctx *const ctx, uint32 leaf_idx,
                                              uint32 idx_offset, uint32 tree_height,
                                              void (*const gen_leaf)(uint8 *const leaf /* Where to write the leaf */,
                                                                     const sphincs_shake_192s_ctx *const ctx,
                                                                     uint32 addr_idx, void *const info),
                                              uint32 tree_addr[8], void *const info);

#endif /* FSMSW_SPHINCSSHAKE_192SSIMPLE_UTILSX4_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */