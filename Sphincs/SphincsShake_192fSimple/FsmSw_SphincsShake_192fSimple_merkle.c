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
#include "FsmSw_Sphincs_shake_address.h"
#include "FsmSw_SphincsShake_192fSimple_params.h"
#include "FsmSw_SphincsShake_192fSimple_utils.h"
#include "FsmSw_SphincsShake_192fSimple_utilsx1.h"
#include "FsmSw_SphincsShake_192fSimple_wots.h"
#include "FsmSw_SphincsShake_192fSimple_wotsx1.h"
#include "FsmSw_SphincsShake_192fSimple_merkle.h"

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
* Name:        FsmSw_SphincsShake_192fSimple_merkle_sign
*
* Description: This generates a Merkle signature (WOTS signature followed by the Merkle authentication path). This is
*              in this file because most of the complexity is involved with the WOTS signature;
*              the Merkle authentication path logic is mostly hidden in treehashx4.
*
* Arguments:   -       uint8                  *sig:          t.b.d.
*              -       uint8                  *root:         t.b.d.
*              - const sphincs_shake_192f_ctx *ctx:          t.b.d.
*              -       uint32                  wots_addr[8]: t.b.d.
*              -       uint32                  tree_addr[8]: t.b.d.
*              -       uint32                  idx_leaf:     t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsShake_192fSimple_merkle_sign(uint8 *sig, uint8 *root, const sphincs_shake_192f_ctx *ctx,
                                               uint32 wots_addr[8], uint32 tree_addr[8], uint32 idx_leaf)
{
    uint8 *auth_path = &sig[FSMSW_SPHINCSSHAKE_192FSIMPLE_WOTS_BYTES];
    struct leaf_info_x1 info = { {(void*)0},
                                        {0},
                                 {(void*)0},
                                 {0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0}
                               };
    uint32 steps[ FSMSW_SPHINCSSHAKE_192FSIMPLE_WOTS_LEN ];

    info.wots_sig = sig;
    FsmSw_SphincsShake_192fSimple_chain_lengths(steps, root);
    info.wots_steps = steps;

    FsmSw_SphincsShake_set_type(&tree_addr[0], FSMSW_SPHINCS_ADDR_TYPE_HASHTREE);
    FsmSw_SphincsShake_set_type(&info.pk_addr[0], FSMSW_SPHINCS_ADDR_TYPE_WOTSPK);
    FsmSw_SphincsShake_copy_subtree_addr(&info.leaf_addr[0], wots_addr);
    FsmSw_SphincsShake_copy_subtree_addr(&info.pk_addr[0], wots_addr);

    info.wots_sign_leaf = idx_leaf;

    FsmSw_SphincsShake_192fSimple_treehashx1(root, auth_path, ctx, idx_leaf, 0,
                                             FSMSW_SPHINCSSHAKE_192FSIMPLE_TREE_HEIGHT,
                                             FsmSw_SphincsShake_192fSimple_wots_gen_leafx1, tree_addr, &info);
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsShake_192fSimple_merkle_gen_root
*
* Description: Compute root node of the top-most subtree.
*
* Arguments:   -       uint8                  *root: t.b.d.
*              - const sphincs_shake_192f_ctx *ctx:  t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsShake_192fSimple_merkle_gen_root(uint8 *root, const sphincs_shake_192f_ctx *ctx)
{
    /* We do not need the auth path in key generation, but it simplifies the
       code to have just one FsmSw_SphincsShake_192fSimple_treehash routine that computes both root and path
       in one function. */
    uint8 auth_path[FSMSW_SPHINCSSHAKE_192FSIMPLE_TREE_HEIGHT * FSMSW_SPHINCSSHAKE_192FSIMPLE_N +
                    FSMSW_SPHINCSSHAKE_192FSIMPLE_WOTS_BYTES];
    uint32 top_tree_addr[8] = {0};
    uint32 wots_addr[8] = {0};

    FsmSw_SphincsShake_set_layer_addr(top_tree_addr, FSMSW_SPHINCSSHAKE_192FSIMPLE_D - 1u);
    FsmSw_SphincsShake_set_layer_addr(wots_addr, FSMSW_SPHINCSSHAKE_192FSIMPLE_D - 1u);

    FsmSw_SphincsShake_192fSimple_merkle_sign(auth_path, root, ctx,
                                              wots_addr, top_tree_addr,
                                              ~((uint32)0u) /* ~0 means "don't bother generating an auth path */ );
}