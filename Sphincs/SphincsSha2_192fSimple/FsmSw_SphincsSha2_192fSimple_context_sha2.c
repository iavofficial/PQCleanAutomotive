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
#include "FsmSw_SphincsSha2_192fSimple_context.h"

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
static void fsmsw_sphincssha2_192fsimple_SeedState(sphincs_sha2_192f_ctx *ctx);

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        fsmsw_sphincssha2_192fsimple_SeedState
*
* Description: Absorb the constant pub_seed using one round of the compression function. This initializes state_seeded
*              and state_seeded_512, which can then be reused in FsmSw_SphincsSha2_192fSimple_thash
*
* Arguments:   - sphincs_sha2_192f_ctx *ctx: t.b.d.
*
*
***********************************************************************************************************************/
static void fsmsw_sphincssha2_192fsimple_SeedState(sphincs_sha2_192f_ctx *ctx)
{
    uint8 block[FSMSW_SPHINCS_SHA512_BLOCK_BYTES];
    uint32 i;

    for (i = 0; i < FSMSW_SPHINCSSHA2_192FSIMPLE_N; ++i)
    {
        block[i] = ctx->pub_seed[i];
    }

    for (i = FSMSW_SPHINCSSHA2_192FSIMPLE_N; i < FSMSW_SPHINCS_SHA512_BLOCK_BYTES; ++i)
    {
        block[i] = 0;
    }

    /* block has been properly initialized for both SHA-256 and SHA-512 */
    FsmSw_sha256_inc_init(&ctx->state_seeded);
    FsmSw_sha256_inc_blocks(&ctx->state_seeded, block, 1);

    FsmSw_sha512_inc_init(&ctx->state_seeded_512);
    FsmSw_sha512_inc_blocks(&ctx->state_seeded_512, block, 1);

}
/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_192fSimple_initialize_hash_function
*
* Description: We initialize the state for the hash functions.
*
* Arguments:   - sphincs_sha2_192f_ctx *ctx: t.b.d.
*
*
***********************************************************************************************************************/
void FsmSw_SphincsSha2_192fSimple_initialize_hash_function(sphincs_sha2_192f_ctx *ctx)
{
    fsmsw_sphincssha2_192fsimple_SeedState(ctx);
}
