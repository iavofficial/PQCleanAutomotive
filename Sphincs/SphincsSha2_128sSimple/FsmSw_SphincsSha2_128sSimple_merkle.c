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
#include "FsmSw_Sphincs_sha2_address.h"
#include "FsmSw_SphincsSha2_128sSimple_params.h"
#include "FsmSw_SphincsSha2_128sSimple_utils.h"
#include "FsmSw_SphincsSha2_128sSimple_utilsx1.h"
#include "FsmSw_SphincsSha2_128sSimple_wots.h"
#include "FsmSw_SphincsSha2_128sSimple_wotsx1.h"
#include "FsmSw_SphincsSha2_128sSimple_merkle.h"

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
* Name:        FsmSw_SphincsSha2_128sSimple_merkle_sign
*
* Description: This generates a Merkle signature (WOTS signature followed by the Merkle authentication path). This is
*              in this file because most of the complexity is involved with the WOTS signature; the Merkle
*              authentication path logic is mostly hidden in treehashx4.
*
* Arguments:   -       uint8                 *sig:          t.b.d.
*              -       uint8                 *root:         t.b.d.
*              - const sphincs_sha2_128s_ctx *ctx:          t.b.d.
*              -       uint32                 wots_addr[8]: t.b.d.
*              -       uint32                 tree_addr[8]: t.b.d.
*              -       uint32                 idx_leaf:     t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsSha2_128sSimple_merkle_sign(uint8 *sig, uint8 *root, const sphincs_sha2_128s_ctx *ctx,
                                              uint32 wots_addr[8], uint32 tree_addr[8], uint32 idx_leaf)
{
    uint8 *auth_path = &sig[FSMSW_SPHINCSSHA2_128SSIMPLE_WOTS_BYTES];
    struct leaf_info_x1 info = { {(void*)0},
                                        {0},
                                 {(void*)0},
                                 {0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0}
                               };
    uint32 steps[ FSMSW_SPHINCSSHA2_128SSIMPLE_WOTS_LEN ];

    info.wots_sig = sig;
    FsmSw_SphincsSha2_128sSimple_chain_lengths(steps, root);
    info.wots_steps = steps;

    FsmSw_SphincsSha2_set_type(&tree_addr[0], FSMSW_SPHINCS_ADDR_TYPE_HASHTREE);
    FsmSw_SphincsSha2_set_type(&info.pk_addr[0], FSMSW_SPHINCS_ADDR_TYPE_WOTSPK);
    FsmSw_SphincsSha2_copy_subtree_addr(&info.leaf_addr[0], wots_addr);
    FsmSw_SphincsSha2_copy_subtree_addr(&info.pk_addr[0], wots_addr);

    info.wots_sign_leaf = idx_leaf;

    FsmSw_SphincsSha2_128sSimple_treehashx1(root, auth_path, ctx, idx_leaf, 0, FSMSW_SPHINCSSHA2_128SSIMPLE_TREE_HEIGHT,
                                            FsmSw_SphincsSha2_128sSimple_wots_gen_leafx1, tree_addr, &info);
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_128sSimple_merkle_sign
*
* Description: Compute root node of the top-most subtree.
*
* Arguments:   -       uint8                 *root:          t.b.d.
*              - const sphincs_sha2_128s_ctx *ctx:          t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsSha2_128sSimple_merkle_gen_root(uint8 *root, const sphincs_sha2_128s_ctx *ctx)
{
    /* We do not need the auth path in key generation, but it simplifies the
       code to have just one treehash routine that computes both root and path
       in one function. */
    uint8 auth_path[FSMSW_SPHINCSSHA2_128SSIMPLE_TREE_HEIGHT * FSMSW_SPHINCSSHA2_128SSIMPLE_N +
                    FSMSW_SPHINCSSHA2_128SSIMPLE_WOTS_BYTES];
    uint32 top_tree_addr[8] = {0};
    uint32 wots_addr[8] = {0};

    FsmSw_SphincsSha2_set_layer_addr(top_tree_addr, FSMSW_SPHINCSSHA2_128SSIMPLE_D - 1u);
    FsmSw_SphincsSha2_set_layer_addr(wots_addr, FSMSW_SPHINCSSHA2_128SSIMPLE_D - 1u);

    FsmSw_SphincsSha2_128sSimple_merkle_sign(auth_path, root, ctx,
                                             wots_addr, top_tree_addr,
                                             ~((uint32)0u) /* ~0 means "don't bother generating an auth path */ );
}
