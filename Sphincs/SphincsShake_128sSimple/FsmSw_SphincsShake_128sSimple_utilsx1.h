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
#ifndef FSMSW_SPHINCSSHAKE_128SSIMPLE_UTILSX4_H
#define FSMSW_SPHINCSSHAKE_128SSIMPLE_UTILSX4_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_SphincsShake_128sSimple_context.h"
#include "FsmSw_SphincsShake_128sSimple_params.h"

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
void FsmSw_SphincsShake_128sSimple_treehashx1(uint8 *root, uint8 *auth_path, const sphincs_shake_128s_ctx *ctx,
                                              uint32 leaf_idx, uint32 idx_offset, uint32 tree_height,
                                              void (*gen_leaf)
                                              (
                                                uint8 *leaf /* Where to write the leaf */,
                                                const sphincs_shake_128s_ctx *ctx,
                                                uint32 addr_idx, void *info
                                              ),
                                              uint32 tree_addr[8], void *info);

#endif /* FSMSW_SPHINCSSHAKE_128SSIMPLE_UTILSX4_H */
