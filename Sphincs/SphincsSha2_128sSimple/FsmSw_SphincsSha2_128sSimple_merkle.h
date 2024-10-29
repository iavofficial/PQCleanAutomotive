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
#ifndef FSMSW_SPHINCSSHA2_128SSIMPLE_MERKLE_H
#define FSMSW_SPHINCSSHA2_128SSIMPLE_MERKLE_H

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
void FsmSw_SphincsSha2_128sSimple_merkle_sign(uint8 *sig, uint8 *root, const sphincs_sha2_128s_ctx *ctx,
                                              const uint32 wots_addr[8], uint32 tree_addr[8], uint32 idx_leaf);

void FsmSw_SphincsSha2_128sSimple_merkle_gen_root(uint8 *root, const sphincs_sha2_128s_ctx *ctx);

#endif /* FSMSW_SPHINCSSHA2_128SSIMPLE_MERKLE_H */
