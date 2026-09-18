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
/** \addtogroup SLH_DSA_SHAKE_128sSimple
*    includes the modules for SLH_DSA_SHAKE_128sSimple
 ** @{ */
/** \addtogroup SLH_DSA_SHAKE_128sSimple_merkle
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHAKE_128sSimple_merkle.c
* \brief  description of SLH_DSA_SHAKE_128sSimple_merkle.c
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
#include "SLH_DSA_SHAKE_128sSimple_params.h"
#include "SLH_DSA_SHAKE_128sSimple_utils.h"
#include "SLH_DSA_SHAKE_128sSimple_utilsx1.h"
#include "SLH_DSA_SHAKE_128sSimple_wots.h"
#include "SLH_DSA_SHAKE_128sSimple_wotsx1.h"
#include "SLH_DSA_SHAKE_address.h"

#include "SLH_DSA_SHAKE_128sSimple_merkle.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define SLH_DSA_SIGN_ADDR_SIZE 8
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
 * \brief This generates a Merkle signature (WOTS signature followed by the Merkle authentication path). This is
 *              in this file because most of the complexity is involved with the WOTS signature; the Merkle
 *              authentication path logic is mostly hidden in treehashx4.
 *
 * \param[out] uint8                        *sig : t.b.d.
 * \param[out] uint8                       *root : t.b.d.
 * \param[in]  const slh_dsa_shake_128s_ctx *ctx : t.b.d.
 * \param[in]  const uint32         wots_addr[8] : t.b.d.
 * \param[in]  uint32               tree_addr[8] : t.b.d.
 * \param[in]  uint32                   idx_leaf : t.b.d.
 *
 */
void SLH_DSA_SHAKE_128sSimple_Merkle_Sign(uint8 *const sig, uint8 *const root,
                                               const slh_dsa_shake_128s_ctx *const ctx, const uint32 wots_addr[8],
                                               uint32 tree_addr[8], uint32 idx_leaf)
{
  uint8 *const auth_path = &sig[SLH_DSA_SHAKE_128SSIMPLE_WOTS_BYTES];

  SLH_DSA_SHAKE_128sSimple_LeafInfoX1_T info = {
      ((void *)0),

      0,
      ((void *)0),
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0}
  };
  uint32 steps[SLH_DSA_SHAKE_128SSIMPLE_WOTS_LEN] = {0};

  info.wots_sig = sig;
  SLH_DSA_SHAKE_128sSimple_Wots_ChainLengths(steps, root);
  info.wots_steps = steps;

  SLH_DSA_SHAKE_SetType(&tree_addr[0], SLH_DSA_ADDR_TYPE_HASHTREE);
  SLH_DSA_SHAKE_SetType(&info.pk_addr[0], SLH_DSA_ADDR_TYPE_WOTSPK);
  SLH_DSA_SHAKE_CopySubTreeAddr(&info.leaf_addr[0], wots_addr);
  SLH_DSA_SHAKE_CopySubTreeAddr(&info.pk_addr[0], wots_addr);

  info.wots_sign_leaf = idx_leaf;

  SLH_DSA_SHAKE_128sSimple_TreeHashX1(root, auth_path, ctx, idx_leaf, 0, SLH_DSA_SHAKE_128SSIMPLE_TREE_HEIGHT,
                                           SLH_DSA_SHAKE_128sSimple_Wots_GenLeafX1, tree_addr, &info);
} // end: SLH_DSA_SHAKE_128sSimple_Merkle_Sign

/*====================================================================================================================*/
/**
 * \brief Compute root node of the top-most subtree.
 *
 * \param[out] uint8                       *root : t.b.d.
 * \param[in]  const slh_dsa_shake_128s_ctx *ctx : t.b.d.
 *
 */
/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void SLH_DSA_SHAKE_128sSimple_Merkle_GenRoot(uint8 *const root, const slh_dsa_shake_128s_ctx *const ctx)
{
  /* We do not need the auth path in key generation, but it simplifies the code to have just one
   * SLH_DSA_SHAKE_128sSimple_TreeHash routine that computes both root and path in one function. */
  uint8 auth_path[(SLH_DSA_SHAKE_128SSIMPLE_TREE_HEIGHT * SLH_DSA_SHAKE_128SSIMPLE_N) +
                  SLH_DSA_SHAKE_128SSIMPLE_WOTS_BYTES] = {0};
  uint32 top_tree_addr[SLH_DSA_SIGN_ADDR_SIZE]        = {0};
  uint32 wots_addr[SLH_DSA_SIGN_ADDR_SIZE]            = {0};

  SLH_DSA_SHAKE_SetLayerAddr(top_tree_addr, SLH_DSA_SHAKE_128SSIMPLE_D - 1u);
  SLH_DSA_SHAKE_SetLayerAddr(wots_addr, SLH_DSA_SHAKE_128SSIMPLE_D - 1u);

  SLH_DSA_SHAKE_128sSimple_Merkle_Sign(auth_path, root, ctx, wots_addr, top_tree_addr,
                                            ~((uint32)0u) /* ~0 means "don't bother generating an auth path */);
} // end: SLH_DSA_SHAKE_128sSimple_Merkle_GenRoot

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
