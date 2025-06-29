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
#ifndef FSMSW_SPHINCSSHAKE_256FSIMPLE_HASH_H
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_HASH_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_SphincsShake_256fSimple_context.h"
#include "FsmSw_SphincsShake_256fSimple_params.h"
#include "FsmSw_Types.h"

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
void FsmSw_SphincsShake_256fSimple_PrfAddr(uint8 *out, const sphincs_shake_256f_ctx *ctx, const uint32 addr[8]);

void FsmSw_SphincsShake_256fSimple_GenMessageRandom(uint8 *R, const uint8 *sk_prf, const uint8 *optrand, const uint8 *m,
                                                    uint32 mlen, const sphincs_shake_256f_ctx *ctx);

void FsmSw_SphincsShake_256fSimple_HashMessage(uint8 *digest, uint64 *tree, uint32 *leaf_idx, const uint8 *R,
                                               const uint8 *pk, const uint8 *m, uint32 mlen,
                                               const sphincs_shake_256f_ctx *ctx);

#endif /* FSMSW_SPHINCSSHAKE_256FSIMPLE_HASH_H */
