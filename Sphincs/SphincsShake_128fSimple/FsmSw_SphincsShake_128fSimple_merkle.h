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
#ifndef FSMSW_SPHINCSSHAKE_128FSIMPLE_MERKLE_H
#define FSMSW_SPHINCSSHAKE_128FSIMPLE_MERKLE_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_SphincsShake_128fSimple_context.h"
#include "FsmSw_SphincsShake_128fSimple_params.h"

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
void FsmSw_SphincsShake_128fSimple_merkle_sign(uint8 *sig, uint8 *root, const sphincs_shake_128f_ctx *ctx,
                                               uint32 wots_addr[8], uint32 tree_addr[8], uint32 idx_leaf);

void FsmSw_SphincsShake_128fSimple_merkle_gen_root(uint8 *root, const sphincs_shake_128f_ctx *ctx);

#endif /* FSMSW_SPHINCSSHAKE_128FSIMPLE_MERKLE_H */
