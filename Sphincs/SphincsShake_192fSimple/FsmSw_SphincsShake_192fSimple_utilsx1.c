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
#include "FsmSw_CommonLib.h"
#include "FsmSw_Sphincs_shake_address.h"
#include "FsmSw_SphincsShake_192fSimple_params.h"
#include "FsmSw_SphincsShake_192fSimple_thash.h"
#include "FsmSw_SphincsShake_192fSimple_utils.h"
#include "FsmSw_SphincsShake_192fSimple_utilsx1.h"

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
* Name:        FsmSw_SphincsShake_192fSimple_treehashx1
*
* Description: Generate the entire Merkle tree, computing the authentication path for leaf_idx, and the resulting root
*              node using Merkle's TreeHash algorithm. Expects the layer and tree parts of the tree_addr to be set, as
*              well as the tree type (i.e. FSMSW_SPHINCS_ADDR_TYPE_HASHTREE or FSMSW_SPHINCS_ADDR_TYPE_FORSTREE).
*              This expects tree_addr to be initialized to the addr structures for the Merkle tree nodes.
*              Applies the offset idx_offset to indices before building addresses, so that it is possible to continue
*              counting indices across trees. This works by using the standard Merkle tree building algorithm.
*
* Arguments:   -       uint8                  *root:        t.b.d.
*              -       uint8                  *auth_path:   t.b.d.
*              - const sphincs_shake_192f_ctx *ctx:         t.b.d.
*              -       uint32                  leaf_idx:    t.b.d.
*              -       uint32                  idx_offset:  t.b.d.
*              -       uint32                  tree_height: t.b.d.
*              -       void                 (*gen_leaf)
*                                             (
*                                                     uint8                  *leaf:         t.b.d.
*                                               const sphincs_shake_192f_ctx *ctx:          t.b.d.
*                                                     uint32                  idx:          t.b.d.
*                                               const uint32                  tree_addr[8]: t.b.d.
*                                                     void                   *info:         t.b.d.
*                                             )
*
***********************************************************************************************************************/
void FsmSw_SphincsShake_192fSimple_treehashx1(uint8 *root, uint8 *auth_path, const sphincs_shake_192f_ctx *ctx,
                                              uint32 leaf_idx, uint32 idx_offset, uint32 tree_height,
                                              void (*gen_leaf)
                                              (
                                                uint8 *leave /* Where to write the leaves */,
                                                const sphincs_shake_192f_ctx *ctx,
                                                uint32 idx, void *info),
                                                uint32 tree_addr[8],
                                                void *info
                                              )
{
    uint8 stack[FSMSW_SPHINCSSHAKE_192FSIMPLE_TREEHASHX1_BUF_LEN * FSMSW_SPHINCSSHAKE_192FSIMPLE_N];

    uint32 idx;
    uint32 max_idx = ((uint32)1u << tree_height) - 1u;
    boolean bStopFunc = FALSE;
    for (idx = 0; idx < 0xFFFFFFFFu; idx++)
    {
        if (TRUE == bStopFunc)
        {
            break;
        }

        /* Current logical node is at index[FSMSW_SPHINCSSHAKE_192FSIMPLE_N]. We do this to minimize the number of
         * copies needed during a FsmSw_SphincsShake_192fSimple_thash */
        uint8 current[2u * FSMSW_SPHINCSSHAKE_192FSIMPLE_N];
        gen_leaf( &current[FSMSW_SPHINCSSHAKE_192FSIMPLE_N], ctx, idx + idx_offset, info );

        /* Now combine the freshly generated right node with previously generated left ones */
        uint32 internal_idx_offset = idx_offset;
        uint32 internal_idx = idx;
        uint32 internal_leaf = leaf_idx;
        /* The height we are in the Merkle tree */
        uint32 h;
        for (h = 0; h < 0xFFFFFFFFu; h++)
        {

            /* Check if we hit the top of the tree */
            if (h == tree_height)
            {
                /* We hit the root; return it */
                FsmSw_CommonLib_memcpy(root, &current[FSMSW_SPHINCSSHAKE_192FSIMPLE_N],
                                       FSMSW_SPHINCSSHAKE_192FSIMPLE_N);
                bStopFunc = TRUE;
            }

            /* Check if the node we have is a part of the authentication path; if it is, write it out */
            if (((internal_idx ^ internal_leaf) == 0x01u) && (FALSE == bStopFunc))
            {
                FsmSw_CommonLib_memcpy(&auth_path[ h * FSMSW_SPHINCSSHAKE_192FSIMPLE_N ],
                                       &current[FSMSW_SPHINCSSHAKE_192FSIMPLE_N],
                                       FSMSW_SPHINCSSHAKE_192FSIMPLE_N);
            }

            /* Check if we're at a left child; if so, stop going up the stack. Exception: if we've reached the end of
             * the tree, keep on going (so we combine the last 4 nodes into the one root node in two more iterations) */
            if ((((internal_idx & 1u) == 0u) && (idx < max_idx)) || (TRUE == bStopFunc))
            {
                break;
            }

            /* Ok, we're at a right node. Now combine the left and right logical nodes together.
             * Set the address of the node we're creating. */
            internal_idx_offset >>= 1;
            FsmSw_SphincsShake_set_tree_height(tree_addr, h + 1u);
            FsmSw_SphincsShake_set_tree_index(tree_addr, (internal_idx / 2u) + internal_idx_offset );

            uint8 *left = &stack[h * FSMSW_SPHINCSSHAKE_192FSIMPLE_N];
            FsmSw_CommonLib_memcpy( &current[0], left, FSMSW_SPHINCSSHAKE_192FSIMPLE_N );
            FsmSw_SphincsShake_192fSimple_thash(&current[FSMSW_SPHINCSSHAKE_192FSIMPLE_N],
                                                &current[0u * FSMSW_SPHINCSSHAKE_192FSIMPLE_N],
                                                2u, ctx, tree_addr);

            internal_idx >>= 1;
            internal_leaf >>= 1;
        }

        if (FALSE == bStopFunc)
        {
            /* We've hit a left child; save the current for when we get the corresponding right right */
            FsmSw_CommonLib_memcpy(&stack[h * FSMSW_SPHINCSSHAKE_192FSIMPLE_N], &current[FSMSW_SPHINCSSHAKE_192FSIMPLE_N],
                                FSMSW_SPHINCSSHAKE_192FSIMPLE_N);
        }
    }
}
