/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsSha2_256sSimple
*    includes the modules for SphincsSha2_256sSimple
 ** @{ */
/** \addtogroup SphincsSha2_256sSimple_merkle
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsSha2_256sSimple_merkle.c
* \brief  description of FsmSw_SphincsSha2_256sSimple_merkle.c
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
#include "FsmSw_SphincsSha2_256sSimple_params.h"
#include "FsmSw_SphincsSha2_256sSimple_utils.h"
#include "FsmSw_SphincsSha2_256sSimple_utilsx1.h"
#include "FsmSw_SphincsSha2_256sSimple_wots.h"
#include "FsmSw_SphincsSha2_256sSimple_wotsx1.h"
#include "FsmSw_Sphincs_sha2_address.h"

#include "FsmSw_SphincsSha2_256sSimple_merkle.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define FSMSW_SPHINCS_ADDR_SIZE 8
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
 *        in this file because most of the complexity is involved with the WOTS signature; the Merkle
 *        authentication path logic is mostly hidden in treehashx4.
 *
 * \param[out] uint8                       *sig : t.b.d.
 * \param[out] uint8                      *root : t.b.d.
 * \param[in]  const sphincs_sha2_256s_ctx *ctx : t.b.d.
 * \param[in]  const uint32        wots_addr[8] : t.b.d.
 * \param[in]  uint32              tree_addr[8] : t.b.d.
 * \param[in]  uint32                  idx_leaf : t.b.d.
 *
 */
void FsmSw_SphincsSha2_256sSimple_Merkle_Sign(uint8 *const sig, uint8 *const root,
                                              const sphincs_sha2_256s_ctx *const ctx, const uint32 wots_addr[8],
                                              uint32 tree_addr[8], uint32 idx_leaf)
{
  uint8 *const auth_path = &sig[FSMSW_SPHINCSSHA2_256SSIMPLE_WOTS_BYTES];

  Fsmsw_Sphincssha2_256sSimple_LeafInfoX1_T info = {
      ((void *)0),

      0,
      ((void *)0),
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0}
  };
  uint32 steps[FSMSW_SPHINCSSHA2_256SSIMPLE_WOTS_LEN] = {0};

  info.wots_sig = sig;
  FsmSw_SphincsSha2_256sSimple_Wots_ChainLengths(steps, root);
  info.wots_steps = steps;

  FsmSw_SphincsSha2_SetType(&tree_addr[0], FSMSW_SPHINCS_ADDR_TYPE_HASHTREE);
  FsmSw_SphincsSha2_SetType(&info.pk_addr[0], FSMSW_SPHINCS_ADDR_TYPE_WOTSPK);
  FsmSw_SphincsSha2_CopySubTreeAddr(&info.leaf_addr[0], wots_addr);
  FsmSw_SphincsSha2_CopySubTreeAddr(&info.pk_addr[0], wots_addr);

  info.wots_sign_leaf = idx_leaf;

  FsmSw_SphincsSha2_256sSimple_TreeHashX1(root, auth_path, ctx, idx_leaf, 0, FSMSW_SPHINCSSHA2_256SSIMPLE_TREE_HEIGHT,
                                          FsmSw_SphincsSha2_256sSimple_Wots_GenLeafX1, tree_addr, &info);
} // end: FsmSw_SphincsSha2_256sSimple_Merkle_Sign

/*====================================================================================================================*/
/**
 * \brief Compute root node of the top-most subtree.
 *
 * \param[out] uint8                      *root : t.b.d.
 * \param[in]  const sphincs_sha2_256s_ctx *ctx : t.b.d.
 *
 */
/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsSha2_256sSimple_Merkle_GenRoot(uint8 *const root, const sphincs_sha2_256s_ctx *const ctx)
{
  /* We do not need the auth path in key generation, but it simplifies the code to have just one
   * FsmSw_SphincsShake_128sSimple_TreeHash routine that computes both root and path in one function. */
  uint8 auth_path[(FSMSW_SPHINCSSHA2_256SSIMPLE_TREE_HEIGHT * FSMSW_SPHINCSSHA2_256SSIMPLE_N) +
                  FSMSW_SPHINCSSHA2_256SSIMPLE_WOTS_BYTES] = {0};
  uint32 top_tree_addr[FSMSW_SPHINCS_ADDR_SIZE]            = {0};
  uint32 wots_addr[FSMSW_SPHINCS_ADDR_SIZE]                = {0};

  FsmSw_SphincsSha2_SetLayerAddr(top_tree_addr, FSMSW_SPHINCSSHA2_256SSIMPLE_D - 1u);
  FsmSw_SphincsSha2_SetLayerAddr(wots_addr, FSMSW_SPHINCSSHA2_256SSIMPLE_D - 1u);

  FsmSw_SphincsSha2_256sSimple_Merkle_Sign(auth_path, root, ctx, wots_addr, top_tree_addr,
                                           ~((uint32)0u) /* ~0 means "don't bother generating an auth path */);
} // end: FsmSw_SphincsSha2_256sSimple_Merkle_GenRoot

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */