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
#include "FsmSw_Fips202.h"
#include "FsmSw_CommonLib.h"
#include "FsmSw_Sphincs_shake_address.h"
#include "FsmSw_SphincsShake_128sSimple_params.h"
#include "FsmSw_SphincsShake_128sSimple_thash.h"
#include "FsmSw_SphincsShake_128sSimple_utils.h"

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
* Name:        FsmSw_SphincsShake_128sSimple_thash
*
* Description: Takes an array of inblocks concatenated arrays of FSMSW_SPHINCSSHAKE_128SSIMPLE_N bytes.
*
* Arguments:   -       uint8                  *out:      t.b.d.
*              - const uint8                  *in:       t.b.d.
*              -       uint32                  inblocks: t.b.d.
*              - const sphincs_shake_128s_ctx *ctx:      t.b.d.
*              - const uint32                  addr[8]:  t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsShake_128sSimple_thash(uint8 *out, const uint8 *in, uint32 inblocks,
                                         const sphincs_shake_128s_ctx *ctx, const uint32 addr[8])
{
    uint8 buf[FSMSW_SPHINCSSHAKE_128SSIMPLE_N + FSMSW_SPHINCSSHAKE_128SSIMPLE_ADDR_BYTES +
              (FSMSW_SPHINCSSHAKE_128SSIMPLE_THASH_BUF_LEN * FSMSW_SPHINCSSHAKE_128SSIMPLE_N)];

    FsmSw_CommonLib_memcpy(buf, ctx->pub_seed, FSMSW_SPHINCSSHAKE_128SSIMPLE_N);
    FsmSw_CommonLib_memcpy(&buf[FSMSW_SPHINCSSHAKE_128SSIMPLE_N], addr, FSMSW_SPHINCSSHAKE_128SSIMPLE_ADDR_BYTES);
    FsmSw_CommonLib_memcpy(&buf[FSMSW_SPHINCSSHAKE_128SSIMPLE_N + FSMSW_SPHINCSSHAKE_128SSIMPLE_ADDR_BYTES], in,
                           inblocks * FSMSW_SPHINCSSHAKE_128SSIMPLE_N);

    FsmSw_Fips202_shake256(out, FSMSW_SPHINCSSHAKE_128SSIMPLE_N, buf, FSMSW_SPHINCSSHAKE_128SSIMPLE_N +
                           FSMSW_SPHINCSSHAKE_128SSIMPLE_ADDR_BYTES + (inblocks * FSMSW_SPHINCSSHAKE_128SSIMPLE_N));
}
