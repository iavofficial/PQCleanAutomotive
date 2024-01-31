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
#ifndef FSMSW_SPHINCSSHA2_128FSIMPLE_UTILSX4_H
#define FSMSW_SPHINCSSHA2_128FSIMPLE_UTILSX4_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_SphincsSha2_128fSimple_context.h"
#include "FsmSw_SphincsSha2_128fSimple_params.h"

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
void FsmSw_SphincsSha2_128fSimple_treehashx1(uint8 *root, uint8 *auth_path, const sphincs_sha2_128f_ctx *ctx,
                                             uint32 leaf_idx, uint32 idx_offset, uint32 tree_height,
                                             void (*gen_leaf)
                                             (
                                               uint8 *out /* Where to write the leaf */,
                                               const sphincs_sha2_128f_ctx *ctx,
                                               uint32 idx, void *info
                                             ),
                                             uint32 tree_addr[8], void *info);

#endif /* FSMSW_SPHINCSSHA2_128FSIMPLE_UTILSX4_H */