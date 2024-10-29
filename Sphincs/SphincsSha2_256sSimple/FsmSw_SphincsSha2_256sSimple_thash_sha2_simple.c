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

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_sha2.h"
#include "FsmSw_CommonLib.h"
#include "FsmSw_Sphincs_sha2_address.h"
#include "FsmSw_SphincsSha2_256sSimple_params.h"
#include "FsmSw_SphincsSha2_256sSimple_thash.h"
#include "FsmSw_SphincsSha2_256sSimple_utils.h"
#include "FsmSw_SphincsSha2_256sSimple_hash.h"

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
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_256fSimple_thash
*
* Description: Takes an array of inblocks concatenated arrays of FSMSW_SPHINCSSHA2_256SSIMPLE_N bytes.
*
* Arguments:   -       uint8                 *out:      t.b.d.
*              - const uint8                 *in:       t.b.d.
*              -       uint32                 inblocks: t.b.d.
*              - const sphincs_sha2_256s_ctx *ctx:      t.b.d.
*              - const uint32                 addr[8]:  t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsSha2_256sSimple_thash(uint8 *out, const uint8 *in, uint32 inblocks, const sphincs_sha2_256s_ctx *ctx,
                                        const uint32 addr[8])
{

    uint8 outbuf[FSMSW_SPHINCS_SHA256_OUTPUT_BYTES];
    sha256ctx sha2_state;
    uint8 buf[SPX_SHA256_ADDR_BYTES + (FSMSW_SPHINCSSHA2_256SSIMPLE_THASH_BUF_LEN * FSMSW_SPHINCSSHA2_256SSIMPLE_N)];

    /* Retrieve precomputed state containing pub_seed */
    FsmSw_sha256_inc_ctx_clone(&sha2_state, &ctx->state_seeded);

    FsmSw_CommonLib_memcpy(buf, addr, SPX_SHA256_ADDR_BYTES);
    FsmSw_CommonLib_memcpy(&buf[SPX_SHA256_ADDR_BYTES], in, inblocks * FSMSW_SPHINCSSHA2_256SSIMPLE_N);

    FsmSw_sha256_inc_finalize(outbuf, &sha2_state, buf, SPX_SHA256_ADDR_BYTES + (inblocks * FSMSW_SPHINCSSHA2_256SSIMPLE_N));
    FsmSw_CommonLib_memcpy(out, outbuf, FSMSW_SPHINCSSHA2_256SSIMPLE_N);
}
