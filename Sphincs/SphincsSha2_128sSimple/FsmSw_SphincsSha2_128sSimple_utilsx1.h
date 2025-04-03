/***********************************************************************************************************************
*
*                                          IAV GmbH
*
***********************************************************************************************************************/
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
#ifndef FSMSW_SPHINCSSHA2_128SSIMPLE_UTILSX4_H
#define FSMSW_SPHINCSSHA2_128SSIMPLE_UTILSX4_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Types.h"
#include "FsmSw_SphincsSha2_128sSimple_context.h"
#include "FsmSw_SphincsSha2_128sSimple_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsSha2_128sSimple_treehashx1(uint8 *root, uint8 *auth_path, const sphincs_sha2_128s_ctx *ctx,
                                             uint32 leaf_idx, uint32 idx_offset, uint32 tree_height,
                                             void (*gen_leaf)
                                             (
                                               uint8 *out /* Where to write the leaf */,
                                               const sphincs_sha2_128s_ctx *ctx,
                                               uint32 idx, void *info
                                             ),
                                             uint32 tree_addr[8], void *info);

#endif /* FSMSW_SPHINCSSHA2_128SSIMPLE_UTILSX4_H */
