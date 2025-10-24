/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsShake_192sSimple
*    includes the modules for SphincsShake_192sSimple
 ** @{ */
/** \addtogroup SphincsShake_192sSimple_merkle
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsShake_192sSimple_merkle.c
* \brief  description of FsmSw_SphincsShake_192sSimple_merkle.c
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
#include "FsmSw_SphincsShake_192sSimple_params.h"
#include "FsmSw_SphincsShake_192sSimple_utils.h"
#include "FsmSw_SphincsShake_192sSimple_utilsx1.h"
#include "FsmSw_SphincsShake_192sSimple_wots.h"
#include "FsmSw_SphincsShake_192sSimple_wotsx1.h"
#include "FsmSw_Sphincs_shake_address.h"

#include "FsmSw_SphincsShake_192sSimple_merkle.h"
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
 * \param[out] uint8                        *sig : t.b.d.
 * \param[out] uint8                       *root : t.b.d.
 * \param[in]  const sphincs_shake_192s_ctx *ctx : t.b.d.
 * \param[in]  const uint32         wots_addr[8] : t.b.d.
 * \param[in]  uint32               tree_addr[8] : t.b.d.
 * \param[in]  uint32                   idx_leaf : t.b.d.
 *
 */
void FsmSw_SphincsShake_192sSimple_Merkle_Sign(uint8 *sig, uint8 *root, const sphincs_shake_192s_ctx *ctx,
                                               const uint32 wots_addr[8], uint32 tree_addr[8], uint32 idx_leaf)
{
  uint8 *auth_path = &sig[FSMSW_SPHINCSSHAKE_192SSIMPLE_WOTS_BYTES];

  Fsmsw_Sphincsshake_192sSimple_LeafInfoX1_T info = {
      ((void *)0),

      0,
      ((void *)0),
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0}
  };
  uint32 steps[FSMSW_SPHINCSSHAKE_192SSIMPLE_WOTS_LEN] = {0};

  info.wots_sig = sig;
  FsmSw_SphincsShake_192sSimple_Wots_ChainLengths(steps, root);
  info.wots_steps = steps;

  FsmSw_SphincsShake_SetType(&tree_addr[0], FSMSW_SPHINCS_ADDR_TYPE_HASHTREE);
  FsmSw_SphincsShake_SetType(&info.pk_addr[0], FSMSW_SPHINCS_ADDR_TYPE_WOTSPK);
  FsmSw_SphincsShake_CopySubTreeAddr(&info.leaf_addr[0], wots_addr);
  FsmSw_SphincsShake_CopySubTreeAddr(&info.pk_addr[0], wots_addr);

  info.wots_sign_leaf = idx_leaf;

  FsmSw_SphincsShake_192sSimple_3_TreeHashX1(root, auth_path, ctx, idx_leaf, 0,
                                             FSMSW_SPHINCSSHAKE_192SSIMPLE_TREE_HEIGHT,
                                             FsmSw_SphincsShake_192sSimple_Wots_Gen_LeafX1, tree_addr, &info);
} // end: FsmSw_SphincsShake_192sSimple_Merkle_Sign

/*====================================================================================================================*/
/**
 * \brief Compute root node of the top-most subtree.
 *
 * \param[out] uint8                       *root : t.b.d.
 * \param[in]  const sphincs_shake_192s_ctx *ctx : t.b.d.
 *
 */
/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsShake_192sSimple_Merkle_GenRoot(uint8 *root, const sphincs_shake_192s_ctx *ctx)
{
  /* We do not need the auth path in key generation, but it simplifies the
     code to have just one FsmSw_SphincsShake_192sSimple_treehash routine that computes both root and path
     in one function. */
  uint8 auth_path[(FSMSW_SPHINCSSHAKE_192SSIMPLE_TREE_HEIGHT * FSMSW_SPHINCSSHAKE_192SSIMPLE_N) +
                  FSMSW_SPHINCSSHAKE_192SSIMPLE_WOTS_BYTES] = {0};
  uint32 top_tree_addr[8]                                   = {0};
  uint32 wots_addr[8]                                       = {0};

  FsmSw_SphincsShake_SetLayerAddr(top_tree_addr, FSMSW_SPHINCSSHAKE_192SSIMPLE_D - 1u);
  FsmSw_SphincsShake_SetLayerAddr(wots_addr, FSMSW_SPHINCSSHAKE_192SSIMPLE_D - 1u);

  FsmSw_SphincsShake_192sSimple_Merkle_Sign(auth_path, root, ctx, wots_addr, top_tree_addr,
                                            ~((uint32)0u) /* ~0 means "don't bother generating an auth path */);
} // end: FsmSw_SphincsShake_192sSimple_Merkle_GenRoot

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */