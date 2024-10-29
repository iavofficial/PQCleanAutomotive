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
***********************************************************************************************************************/
#ifndef FSMSW_SPHINCSSHA2_256SSIMPLE_UTILSX4_H
#define FSMSW_SPHINCSSHA2_256SSIMPLE_UTILSX4_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Types.h"
#include "FsmSw_SphincsSha2_256sSimple_context.h"
#include "FsmSw_SphincsSha2_256sSimple_params.h"

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

void FsmSw_SphincsSha2_256sSimple_treehashx1(uint8 *root, uint8 *auth_path, const sphincs_sha2_256s_ctx *ctx,
                                             uint32 leaf_idx, uint32 idx_offset, uint32 tree_height,
                                             void (*gen_leaf)
                                             (
                                               uint8 *leaf /* Where to write the leaf */,
                                               const sphincs_sha2_256s_ctx *ctx,
                                               uint32 addr_idx, void *info
                                             ),
                                             uint32 tree_addr[8], void *info);

#endif /* FSMSW_SPHINCSSHA2_256SSIMPLE_UTILSX4_H */
