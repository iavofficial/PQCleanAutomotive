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
#ifndef FSMSW_SPHINCSSHA2_256FSIMPLE_MERKLE_H
#define FSMSW_SPHINCSSHA2_256FSIMPLE_MERKLE_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Types.h"
#include "FsmSw_SphincsSha2_256fSimple_context.h"
#include "FsmSw_SphincsSha2_256fSimple_params.h"

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
void FsmSw_SphincsSha2_256fSimple_merkle_sign(uint8 *sig, uint8 *root, const sphincs_sha2_256f_ctx *ctx,
                                              const uint32 wots_addr[8], uint32 tree_addr[8], uint32 idx_leaf);

/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsSha2_256fSimple_merkle_gen_root(uint8 *root, const sphincs_sha2_256f_ctx *ctx);

#endif /* FSMSW_SPHINCSSHA2_256FSIMPLE_MERKLE_H */
