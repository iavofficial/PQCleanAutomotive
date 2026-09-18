/***********************************************************************************************************************
 *
 * Original implementation: PQClean, SLH-DSA (standardized as SLH-DSA)
 *
 * Copyright 2026 IAV GmbH
 *
 * Original portions are dedicated to the public domain under CC0 1.0 Universal.
 * See the NOTICE file in the repository root for attribution information.
 * IAV modifications are licensed under the Apache License, Version 2.0.
 *
 * SPDX-License-Identifier: CC0-1.0 AND Apache-2.0
 *
 **********************************************************************************************************************/

/** \addtogroup SwC SLH-DSA
*    includes the modules for SwC SLH-DSA
 ** @{ */
/** \addtogroup SLH_DSA_SHA2_192sSimple
*    includes the modules for SLH_DSA_SHA2_192sSimple
 ** @{ */
/** \addtogroup SLH_DSA_SHA2_192sSimple_utils
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHA2_192sSimple_utils.c
* \brief  description of SLH_DSA_SHA2_192sSimple_utils.c
*
* \details
*
*
*/
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
 **********************************************************************************************************************/

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_CommonLib.h"
#include "SLH_DSA_SHA2_192sSimple_hash.h"
#include "SLH_DSA_SHA2_192sSimple_params.h"
#include "SLH_DSA_SHA2_192sSimple_thash.h"
#include "SLH_DSA_SHA2_address.h"

#include "SLH_DSA_SHA2_192sSimple_utils.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL CONSTANTS                                                                                                   */
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

/*====================================================================================================================*/
/** 
 * \brief Computes a root node given a leaf and an auth path. Expects address to be complete other than the
 *              tree_height and tree_index.
 *
 * \param[out] uint8                      *root : t.b.d.
 * \param[in]  const uint8                *leaf : t.b.d.
 * \param[in]  uint32                  leaf_idx : t.b.d.
 * \param[in]  uint32                idx_offset : t.b.d.
 * \param[in]  const uint8           *auth_path : t.b.d.
 * \param[in]  uint32               tree_height : t.b.d.
 * \param[in]  const slh_dsa_sha2_192s_ctx *ctx : t.b.d.
 * \param[in]  uint32                   addr[8] : t.b.d.
 *
 */
void SLH_DSA_SHA2_192sSimple_ComputeRoot(uint8 *const root, const uint8 *const leaf, uint32 leaf_idx,
                                              uint32 idx_offset, const uint8 *const auth_path, uint32 tree_height,
                                              const slh_dsa_sha2_192s_ctx *const ctx, uint32 addr[8])
{
  uint32 i                                          = 0;
  uint8 buffer[2u * SLH_DSA_SHA2_192SSIMPLE_N] = {0};

  /* leaf_idx_temp, idx_offset_temp and auth_path_temp are used to avoid modifying the input. */
  uint32 leaf_idx_temp        = leaf_idx;
  uint32 idx_offset_temp      = idx_offset;
  const uint8 *auth_path_temp = auth_path;

  /* If leaf_idx is odd (last bit = 1), current path element is a right child
     and auth_path has to go left. Otherwise it is the other way around. */
  if (0u < (leaf_idx_temp & 1u))
  {
    FsmSw_CommonLib_MemCpy(&buffer[SLH_DSA_SHA2_192SSIMPLE_N], leaf, SLH_DSA_SHA2_192SSIMPLE_N);
    FsmSw_CommonLib_MemCpy(buffer, auth_path_temp, SLH_DSA_SHA2_192SSIMPLE_N);
  }
  else
  {
    FsmSw_CommonLib_MemCpy(buffer, leaf, SLH_DSA_SHA2_192SSIMPLE_N);
    FsmSw_CommonLib_MemCpy(&buffer[SLH_DSA_SHA2_192SSIMPLE_N], auth_path_temp, SLH_DSA_SHA2_192SSIMPLE_N);
  }
  auth_path_temp = &auth_path_temp[SLH_DSA_SHA2_192SSIMPLE_N];

  for (i = 0; i < (tree_height - 1u); i++)
  {
    leaf_idx_temp >>= 1;
    idx_offset_temp >>= 1;
    /* Set the address of the node we're creating. */
    SLH_DSA_SHA2_SetTreeHeight(addr, i + 1u);
    SLH_DSA_SHA2_SetTreeIndex(addr, leaf_idx_temp + idx_offset_temp);

    /* Pick the right or left neighbor, depending on parity of the node. */
    if (0u < (leaf_idx_temp & 1u))
    {
      SLH_DSA_SHA2_192sSimple_Thash(&buffer[SLH_DSA_SHA2_192SSIMPLE_N], buffer, 2u, ctx, addr);
      FsmSw_CommonLib_MemCpy(buffer, auth_path_temp, SLH_DSA_SHA2_192SSIMPLE_N);
    }
    else
    {
      SLH_DSA_SHA2_192sSimple_Thash(buffer, buffer, 2u, ctx, addr);
      FsmSw_CommonLib_MemCpy(&buffer[SLH_DSA_SHA2_192SSIMPLE_N], auth_path_temp, SLH_DSA_SHA2_192SSIMPLE_N);
    }
    auth_path_temp = &auth_path_temp[SLH_DSA_SHA2_192SSIMPLE_N];
  }

  /* The last iteration is exceptional; we do not copy an auth_path node. */
  leaf_idx_temp >>= 1;
  idx_offset_temp >>= 1;
  SLH_DSA_SHA2_SetTreeHeight(addr, tree_height);
  SLH_DSA_SHA2_SetTreeIndex(addr, leaf_idx_temp + idx_offset_temp);
  SLH_DSA_SHA2_192sSimple_Thash(root, buffer, 2u, ctx, addr);
} // end: SLH_DSA_SHA2_192sSimple_ComputeRoot

/*====================================================================================================================*/
/** 
 * \brief For a given leaf index, computes the authentication path and the resulting root node using Merkle's
 *        TreeHash algorithm. Expects the layer and tree parts of the tree_addr to be set, as well as the tree
 *        type (i.e. SLH_DSA_ADDR_TYPE_HASHTREE or SLH_DSA_ADDR_TYPE_FORSTREE). Applies the offset
 *        idx_offset to indices before building addresses, so that it is possible to continue counting indices
 *        across trees.
 *
 * \param[out] uint8                      *root : t.b.d.
 * \param[out] uint8                 *auth_path : t.b.d.
 * \param[in]  const slh_dsa_sha2_192s_ctx *ctx : t.b.d.
 * \param[in]  uint32                  leaf_idx : t.b.d.
 * \param[in]  uint32                idx_offset : t.b.d.
 * \param[in]  uint32               tree_height : t.b.d.
 * \param[in]  void                 (*gen_leaf)
 *                                             (
 *                                                     uint8                *leaf : t.b.d.
 *                                               const slh_dsa_sha2_192s_ctx *ctx : t.b.d.
 *                                                     uint32                 idx : t.b.d.
 *                                               const uint32        tree_addr[8] : t.b.d.
 *                                             )
 * \param[in]  uint32              tree_addr[8] : t.b.d.
 *
 * Note:        This function is currently not used.
 */
void SLH_DSA_SHA2_192sSimple_TreeHash(uint8 *const root, uint8 *const auth_path,
                                           const slh_dsa_sha2_192s_ctx *const ctx, uint32 leaf_idx, uint32 idx_offset,
                                           uint32 tree_height,
                                           void (*const gen_leaf)(uint8 *leaf, const slh_dsa_sha2_192s_ctx *ctx,
                                                                  uint32 addr_idx, const uint32 tree_addr[8]),
                                           uint32 tree_addr[8])
{
  uint8 stack[(SLH_DSA_SHA2_192SSIMPLE_TREEHASH_BUF_LEN + 1u) * SLH_DSA_SHA2_192SSIMPLE_N] = {0};
  uint32 heights[SLH_DSA_SHA2_192SSIMPLE_TREEHASH_BUF_LEN + 1u]                                 = {0};
  uint32 offset                                                                                      = 0;
  uint32 idx                                                                                         = 0;
  uint32 tree_idx                                                                                    = 0;

  for (idx = 0; idx < (uint32)((uint32)1u << tree_height); idx++)
  {
    /* Add the next leaf node to the stack. */
    gen_leaf(&stack[offset * SLH_DSA_SHA2_192SSIMPLE_N], ctx, idx + idx_offset, tree_addr);
    offset++;
    heights[offset - 1u] = 0;

    /* If this is a node we need for the auth path. */
    if ((leaf_idx ^ 0x1u) == idx)
    {
      FsmSw_CommonLib_MemCpy(auth_path, &stack[(offset - 1u) * SLH_DSA_SHA2_192SSIMPLE_N],
                             SLH_DSA_SHA2_192SSIMPLE_N);
    }

    /* While the top-most nodes are of equal height. */
    while ((offset >= 2u) && (heights[offset - 1u] == heights[offset - 2u]))
    {
      /* Compute index of the new node, in the next layer. */
      tree_idx = (idx >> (heights[offset - 1u] + 1u));

      /* Set the address of the node we're creating. */
      SLH_DSA_SHA2_SetTreeHeight(tree_addr, heights[offset - 1u] + 1u);
      SLH_DSA_SHA2_SetTreeIndex(tree_addr, tree_idx + (idx_offset >> (heights[offset - 1u] + 1u)));
      /* Hash the top-most nodes from the stack together. */
      SLH_DSA_SHA2_192sSimple_Thash(&stack[(offset - 2u) * SLH_DSA_SHA2_192SSIMPLE_N],
                                         &stack[(offset - 2u) * SLH_DSA_SHA2_192SSIMPLE_N], 2u, ctx, tree_addr);
      offset--;
      /* Note that the top-most node is now one layer higher. */
      heights[offset - 1u]++;

      /* If this is a node we need for the auth path.. */
      if (((leaf_idx >> heights[offset - 1u]) ^ 0x1u) == tree_idx)
      {
        FsmSw_CommonLib_MemCpy(&auth_path[heights[offset - 1u] * SLH_DSA_SHA2_192SSIMPLE_N],
                               &stack[(offset - 1u) * SLH_DSA_SHA2_192SSIMPLE_N], SLH_DSA_SHA2_192SSIMPLE_N);
      }
    }
  }
  FsmSw_CommonLib_MemCpy(root, stack, SLH_DSA_SHA2_192SSIMPLE_N);
} // end: SLH_DSA_SHA2_192sSimple_TreeHash

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */