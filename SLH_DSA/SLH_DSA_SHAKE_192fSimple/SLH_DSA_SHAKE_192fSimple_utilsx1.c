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
/** \addtogroup SLH_DSA_SHAKE_192fSimple
*    includes the modules for SLH_DSA_SHAKE_192fSimple
 ** @{ */
/** \addtogroup SLH_DSA_SHAKE_192fSimple_utilsx1
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHAKE_192fSimple_utilsx1.c
* \brief  description of SLH_DSA_SHAKE_192fSimple_utilsx1.c
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
#include "SLH_DSA_SHAKE_192fSimple_params.h"
#include "SLH_DSA_SHAKE_192fSimple_thash.h"
#include "SLH_DSA_SHAKE_192fSimple_utils.h"
#include "SLH_DSA_SHAKE_address.h"

#include "SLH_DSA_SHAKE_192fSimple_utilsx1.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define SLH_DSA_UINT32_MAX_VALUE 0xFFFFFFFFu
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
 * \brief Generate the entire Merkle tree, computing the authentication path for leaf_idx, and the resulting root
 *        node using Merkle's TreeHash algorithm. Expects the layer and tree parts of the tree_addr to be set, as
 *        well as the tree type (i.e. SLH_DSA_ADDR_TYPE_HASHTREE or SLH_DSA_ADDR_TYPE_FORSTREE).
 *        This expects tree_addr to be initialized to the addr structures for the Merkle tree nodes.
 *        Applies the offset idx_offset to indices before building addresses, so that it is possible to continue
 *        counting indices across trees. This works by using the standard Merkle tree building algorithm.
 *
 * \param[out] uint8                       *root : t.b.d.
 * \param[out] uint8                  *auth_path : t.b.d.
 * \param[in]  const slh_dsa_shake_192f_ctx *ctx : t.b.d.
 * \param[in]  uint32                   leaf_idx : t.b.d.
 * \param[in]  uint32                 idx_offset : t.b.d.
 * \param[in]  uint32                tree_height : t.b.d.
 * \param[in]  void                 (*gen_leaf)
 *                                             (
 *                                                     uint8                 *leaf : t.b.d.
 *                                               const slh_dsa_shake_192f_ctx *ctx : t.b.d.
 *                                                     uint32                  idx : t.b.d.
 *                                               const uint32         tree_addr[8] : t.b.d.
 *                                                     void                  *info : t.b.d.
 *                                             )
 *
 */
/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void SLH_DSA_SHAKE_192fSimple_TreeHashX1(uint8 *const root, uint8 *const auth_path,
                                              const slh_dsa_shake_192f_ctx *const ctx, uint32 leaf_idx,
                                              uint32 idx_offset, uint32 tree_height,
                                              void (*const gen_leaf)(uint8 *const leave /* Where to write the leaves */,
                                                                     const slh_dsa_shake_192f_ctx *const ctx,
                                                                     uint32 idx, void *const info),
                                              uint32 tree_addr[8], void *const info)
{
  uint8 stack[SLH_DSA_SHAKE_192FSIMPLE_TREEHASHX1_BUF_LEN * SLH_DSA_SHAKE_192FSIMPLE_N] = {0};

  uint32 idx           = 0;
  uint32 const max_idx = ((uint32)1u << tree_height) - 1u;
  boolean bStopFunc    = FALSE;
  for (idx = 0; idx < SLH_DSA_UINT32_MAX_VALUE; idx++)
  {
    if (TRUE == bStopFunc)
    {
      break;
    }

    /* Current logical node is at index[SLH_DSA_SHAKE_192FSIMPLE_N]. We do this to minimize the number of
     * copies needed during a SLH_DSA_SHAKE_192fSimple_Thash */
    uint8 current[2u * SLH_DSA_SHAKE_192FSIMPLE_N];
    gen_leaf(&current[SLH_DSA_SHAKE_192FSIMPLE_N], ctx, idx + idx_offset, info);

    /* Now combine the freshly generated right node with previously generated left ones */
    uint32 internal_idx_offset = idx_offset;
    uint32 internal_idx        = idx;
    uint32 internal_leaf       = leaf_idx;
    /* The height we are in the Merkle tree */
    uint32 h;
    for (h = 0; h < SLH_DSA_UINT32_MAX_VALUE; h++)
    {

      /* Check if we hit the top of the tree */
      if (h == tree_height)
      {
        /* We hit the root; return it */
        FsmSw_CommonLib_MemCpy(root, &current[SLH_DSA_SHAKE_192FSIMPLE_N], SLH_DSA_SHAKE_192FSIMPLE_N);
        bStopFunc = TRUE;
      }

      /* Check if the node we have is a part of the authentication path; if it is, write it out */
      if (((internal_idx ^ internal_leaf) == 0x01u) && (FALSE == bStopFunc))
      {
        FsmSw_CommonLib_MemCpy(&auth_path[h * SLH_DSA_SHAKE_192FSIMPLE_N],
                               &current[SLH_DSA_SHAKE_192FSIMPLE_N], SLH_DSA_SHAKE_192FSIMPLE_N);
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
      SLH_DSA_SHAKE_SetTreeHeight(tree_addr, h + 1u);
      SLH_DSA_SHAKE_SetTreeIndex(tree_addr, (internal_idx / 2u) + internal_idx_offset);

      uint8 *const left = &stack[h * SLH_DSA_SHAKE_192FSIMPLE_N];
      FsmSw_CommonLib_MemCpy(&current[0], left, SLH_DSA_SHAKE_192FSIMPLE_N);
      SLH_DSA_SHAKE_192fSimple_Thash(&current[SLH_DSA_SHAKE_192FSIMPLE_N],
                                          &current[0u * SLH_DSA_SHAKE_192FSIMPLE_N], 2u, ctx, tree_addr);

      internal_idx >>= 1;
      internal_leaf >>= 1;
    }

    if (FALSE == bStopFunc)
    {
      /* We've hit a left child; save the current for when we get the corresponding right right */
      FsmSw_CommonLib_MemCpy(&stack[h * SLH_DSA_SHAKE_192FSIMPLE_N], &current[SLH_DSA_SHAKE_192FSIMPLE_N],
                             SLH_DSA_SHAKE_192FSIMPLE_N);
    }
  }
} // end: SLH_DSA_SHAKE_192fSimple_TreeHashX1

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */