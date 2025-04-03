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
#ifndef FSMSW_SPHINCSSHA2_128SSIMPLE_HASH_H
#define FSMSW_SPHINCSSHA2_128SSIMPLE_HASH_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Types.h"
#include "FsmSw_SphincsSha2_128sSimple_context.h"
#include "FsmSw_SphincsSha2_128sSimple_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define SPX_SHA256_ADDR_BYTES 22u

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
void FsmSw_SphincsSha2_128sSimple_prf_addr(uint8 *out, const sphincs_sha2_128s_ctx *ctx, const uint32 addr[8]);

void FsmSw_SphincsSha2_128sSimple_gen_message_random(uint8 *R, const uint8 *sk_prf, const uint8 *optrand,
                                                     const uint8 *m, uint32 mlen, const sphincs_sha2_128s_ctx *ctx);

void FsmSw_SphincsSha2_128sSimple_hash_message(uint8 *digest, uint64 *tree, uint32 *leaf_idx, const uint8 *R,
                                               const uint8 *pk, const uint8 *m, uint32 mlen,
                                               const sphincs_sha2_128s_ctx *ctx);

void FsmSw_SphincsSha2_128sSimple_mgf1_256(uint8 *out, uint32 outlen, const uint8 *in, uint32 inlen);

/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsSha2_128sSimple_mgf1_512(uint8 *out, uint32 outlen, const uint8 *in, uint32 inlen);

#endif /* FSMSW_SPHINCSSHA2_128SSIMPLE_HASH_H */
