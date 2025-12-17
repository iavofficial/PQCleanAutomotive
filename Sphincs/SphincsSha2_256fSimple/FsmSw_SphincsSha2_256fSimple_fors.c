/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsSha2_256fSimple
*    includes the modules for SphincsSha2_256fSimple
 ** @{ */
/** \addtogroup SphincsSha2_256fSimple_fors
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsSha2_256fSimple_fors.c
* \brief  description of FsmSw_SphincsSha2_256fSimple_fors.c
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
#include "FsmSw_SphincsSha2_256fSimple_FctWrapper.h"
#include "FsmSw_SphincsSha2_256fSimple_hash.h"
#include "FsmSw_SphincsSha2_256fSimple_thash.h"
#include "FsmSw_SphincsSha2_256fSimple_utils.h"
#include "FsmSw_SphincsSha2_256fSimple_utilsx1.h"
#include "FsmSw_Sphincs_sha2_address.h"

#include "FsmSw_SphincsSha2_256fSimple_fors.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define FSMSW_SPHINCS_ADDR_SIZE 8
/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct
{
  uint32 leaf_addrx[FSMSW_SPHINCS_ADDR_SIZE];
} Fsmsw_Sphincssha2_256fSimple_ForsGenLeafInfo_T;
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
static void fsmsw_sphincssha2_256fsimple_fors_GenSk(uint8 *const sk, const sphincs_sha2_256f_ctx *const ctx,
                                                    const uint32 fors_leaf_addr[8]);
static void fsmsw_sphincssha2_256fsimple_fors_SkToLeaf(uint8 *const leaf, const uint8 *const sk,
                                                       const sphincs_sha2_256f_ctx *const ctx,
                                                       const uint32 fors_leaf_addr[8]);
static void fsmsw_sphincssha2_256fsimple_fors_GenLeafx1(uint8 *const leaf, const sphincs_sha2_256f_ctx *const ctx,
                                                        uint32 addr_idx, void *const info);
static void fsmsw_sphincssha2_256fsimple_fors_MessageToIndices(uint32 *const indices, const uint8 *const m);
/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/** 
 * \brief t.b.d
 *
 * \param[out] uint8                        *sk : t.b.d.
 * \param[in]  const sphincs_sha2_256f_ctx *ctx : t.b.d.
 * \param[in]  const uint32   fors_leaf_addr[8] : t.b.d.
 *
 */
static void fsmsw_sphincssha2_256fsimple_fors_GenSk(uint8 *const sk, const sphincs_sha2_256f_ctx *const ctx,
                                                    const uint32 fors_leaf_addr[8])
{
  FsmSw_SphincsSha2_256fSimple_PrfAddr(sk, ctx, fors_leaf_addr);
} // end: fsmsw_sphincssha2_256fsimple_fors_GenSk

/*====================================================================================================================*/
/** 
 * \brief t.b.d
 *
 * \param[out] uint8                      *leaf : t.b.d.
 * \param[in]  const uint8                  *sk : t.b.d.
 * \param[in]  const sphincs_sha2_256f_ctx *ctx : t.b.d.
 * \param[in]  const uint32   fors_leaf_addr[8] : t.b.d.
 *
 */
static void fsmsw_sphincssha2_256fsimple_fors_SkToLeaf(uint8 *const leaf, const uint8 *const sk,
                                                       const sphincs_sha2_256f_ctx *const ctx,
                                                       const uint32 fors_leaf_addr[8])
{
  FsmSw_SphincsSha2_256fSimple_Thash(leaf, sk, 1, ctx, fors_leaf_addr);
} // end: fsmsw_sphincssha2_256fsimple_fors_SkToLeaf

/*====================================================================================================================*/
/** 
 * \brief t.b.d
 *
 * \param[out] uint8                      *leaf : t.b.d.
 * \param[in]  const sphincs_sha2_256f_ctx *ctx : t.b.d.
 * \param[in]  uint32                  addr_idx : t.b.d.
 * \param[in]  void                       *info : t.b.d.
 *
 */
static void fsmsw_sphincssha2_256fsimple_fors_GenLeafx1(uint8 *const leaf, const sphincs_sha2_256f_ctx *const ctx,
                                                        uint32 addr_idx, void *const info)
{
  /* polyspace +4 CERT-C:EXP36-C [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality.
    Ensured proper alignment and validity." */
  Fsmsw_Sphincssha2_256fSimple_ForsGenLeafInfo_T *fors_info = info;
  uint32 *const fors_leaf_addr                              = fors_info->leaf_addrx;

  /* Only set the parts that the caller doesn't set */
  FsmSw_SphincsSha2_SetTreeIndex(fors_leaf_addr, addr_idx);
  FsmSw_SphincsSha2_SetType(fors_leaf_addr, FSMSW_SPHINCS_ADDR_TYPE_FORSPRF);
  fsmsw_sphincssha2_256fsimple_fors_GenSk(leaf, ctx, fors_leaf_addr);

  FsmSw_SphincsSha2_SetType(fors_leaf_addr, FSMSW_SPHINCS_ADDR_TYPE_FORSTREE);
  fsmsw_sphincssha2_256fsimple_fors_SkToLeaf(leaf, leaf, ctx, fors_leaf_addr);
} // end: fsmsw_sphincssha2_256fsimple_fors_GenLeafx1

/*====================================================================================================================*/
/** 
 * \brief Interprets m as FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_HEIGHT-bit uint32. Assumes m contains at least
 *        FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_HEIGHT * FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_TREES bits. Assumes indices
 *        has space for FSMSW_SPHINCSSHA2_192SSIMPLE_FORS_TREES integers.
 *
 * \param[out] uint32 *indices : t.b.d.
 * \param[in]  const uint8  *m : t.b.d.
 *
 */
static void fsmsw_sphincssha2_256fsimple_fors_MessageToIndices(uint32 *const indices, const uint8 *const m)
{
  uint32 i      = 0;
  uint32 j      = 0;
  uint32 offset = 0;

  for (i = 0; i < FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_TREES; i++)
  {
    indices[i] = 0;
    for (j = 0; j < FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_HEIGHT; j++)
    {
      indices[i] ^= (uint32)(((((uint32)(m[offset >> 3]) >> (offset & 0x7u)) & 0x1u)) << j);
      offset++;
    }
  }
} // end: fsmsw_sphincssha2_256fsimple_fors_MessageToIndices
/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/** 
 * \brief Signs a message m, deriving the secret key from sk_seed and the FTS address. Assumes m contains at least
 *        FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_HEIGHT * FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_TREES bits.
 *
 * \param[out] uint8                       *sig : t.b.d.
 * \param[out] uint8                        *pk : t.b.d.
 * \param[in]  const uint8                   *m : t.b.d.
 * \param[in]  const sphincs_sha2_256f_ctx *ctx : t.b.d.
 * \param[in]  const uint32        fors_addr[8] : t.b.d.
 *
 */
void FsmSw_SphincsSha2_256fSimple_Fors_Sign(uint8 *const sig, uint8 *const pk, const uint8 *const m,
                                            const sphincs_sha2_256f_ctx *const ctx, const uint32 fors_addr[8])
{
  uint32 indices[FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_TREES]                               = {0};
  uint8 roots[FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_TREES * FSMSW_SPHINCSSHA2_256FSIMPLE_N] = {0};
  uint32 fors_tree_addr[FSMSW_SPHINCS_ADDR_SIZE]                                        = {0};
  Fsmsw_Sphincssha2_256fSimple_ForsGenLeafInfo_T fors_info                              = {{0}};
  uint32 *const fors_leaf_addr                                                          = fors_info.leaf_addrx;
  uint32 fors_pk_addr[FSMSW_SPHINCS_ADDR_SIZE]                                          = {0};
  uint32 idx_offset                                                                     = 0;
  uint32 i                                                                              = 0;

  /* sig_temp is used to avoid modifying the input. */
  uint8 *sig_temp = sig;

  FsmSw_SphincsSha2_256fSimple_copy_keypair_addr(fors_tree_addr, fors_addr);
  FsmSw_SphincsSha2_256fSimple_copy_keypair_addr(fors_leaf_addr, fors_addr);

  FsmSw_SphincsSha2_256fSimple_copy_keypair_addr(fors_pk_addr, fors_addr);
  FsmSw_SphincsSha2_SetType(fors_pk_addr, FSMSW_SPHINCS_ADDR_TYPE_FORSPK);

  fsmsw_sphincssha2_256fsimple_fors_MessageToIndices(indices, m);

  for (i = 0; i < FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_TREES; i++)
  {
    idx_offset = i * (uint32)((uint64)((uint64)1u << FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_HEIGHT));

    FsmSw_SphincsSha2_SetTreeHeight(fors_tree_addr, 0);
    FsmSw_SphincsSha2_SetTreeIndex(fors_tree_addr, indices[i] + idx_offset);
    FsmSw_SphincsSha2_SetType(fors_tree_addr, FSMSW_SPHINCS_ADDR_TYPE_FORSPRF);

    /* Include the secret key part that produces the selected leaf node. */
    fsmsw_sphincssha2_256fsimple_fors_GenSk(sig_temp, ctx, fors_tree_addr);
    FsmSw_SphincsSha2_SetType(fors_tree_addr, FSMSW_SPHINCS_ADDR_TYPE_FORSTREE);
    sig_temp = &sig_temp[FSMSW_SPHINCSSHA2_256FSIMPLE_N];

    /* Compute the authentication path for this leaf node. */
    FsmSw_SphincsSha2_256fSimple_TreeHashX1(&roots[i * FSMSW_SPHINCSSHA2_256FSIMPLE_N], sig_temp, ctx, indices[i],
                                            idx_offset, FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_HEIGHT,
                                            fsmsw_sphincssha2_256fsimple_fors_GenLeafx1, fors_tree_addr, &fors_info);

    sig_temp = &sig_temp[FSMSW_SPHINCSSHA2_256FSIMPLE_N * FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_HEIGHT];
  }

  /* Hash horizontally across all tree roots to derive the public key. */
  FsmSw_SphincsSha2_256fSimple_Thash(pk, roots, FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_TREES, ctx, fors_pk_addr);
} // end: FsmSw_SphincsSha2_256fSimple_Fors_Sign

/*====================================================================================================================*/
/** 
 * \brief Derives the FORS public key from a signature. This can be used for verification by comparing to a known
 *        public key, or to subsequently verify a signature on the derived public key. The latter is the typical
 *        use-case when used as an FTS below an OTS in a hypertree. Assumes m contains at least
 *        FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_HEIGHT * FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_TREES bits.
 *
 * \param[out] uint8                        *pk : t.b.d.
 * \param[out] uint8                       *sig : t.b.d.
 * \param[in]  const uint8                   *m : t.b.d.
 * \param[in]  const sphincs_sha2_256f_ctx *ctx : t.b.d.
 * \param[in]  const uint32        fors_addr[8] : t.b.d.
 *
 */
/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsSha2_256fSimple_Fors_PkFromSig(uint8 *const pk, const uint8 *const sig, const uint8 *const m,
                                                 const sphincs_sha2_256f_ctx *const ctx, const uint32 fors_addr[8])
{
  uint32 indices[FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_TREES]                               = {0};
  uint8 roots[FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_TREES * FSMSW_SPHINCSSHA2_256FSIMPLE_N] = {0};
  uint8 leaf[FSMSW_SPHINCSSHA2_256FSIMPLE_N]                                            = {0};
  uint32 fors_tree_addr[FSMSW_SPHINCS_ADDR_SIZE]                                        = {0};
  uint32 fors_pk_addr[FSMSW_SPHINCS_ADDR_SIZE]                                          = {0};
  uint32 idx_offset                                                                     = 0;
  uint32 i                                                                              = 0;

  /* sig_temp is used to avoid modifying the input. */
  const uint8 *sig_temp = sig;

  FsmSw_SphincsSha2_256fSimple_copy_keypair_addr(fors_tree_addr, fors_addr);
  FsmSw_SphincsSha2_256fSimple_copy_keypair_addr(fors_pk_addr, fors_addr);

  FsmSw_SphincsSha2_SetType(fors_tree_addr, FSMSW_SPHINCS_ADDR_TYPE_FORSTREE);
  FsmSw_SphincsSha2_SetType(fors_pk_addr, FSMSW_SPHINCS_ADDR_TYPE_FORSPK);

  fsmsw_sphincssha2_256fsimple_fors_MessageToIndices(indices, m);

  for (i = 0; i < FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_TREES; i++)
  {
    idx_offset = i * (uint32)((uint64)((uint64)1u << FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_HEIGHT));

    FsmSw_SphincsSha2_SetTreeHeight(fors_tree_addr, 0);
    FsmSw_SphincsSha2_SetTreeIndex(fors_tree_addr, indices[i] + idx_offset);

    /* Derive the leaf from the included secret key part. */
    fsmsw_sphincssha2_256fsimple_fors_SkToLeaf(leaf, sig_temp, ctx, fors_tree_addr);
    sig_temp = &sig_temp[FSMSW_SPHINCSSHA2_256FSIMPLE_N];

    /* Derive the corresponding root node of this tree. */
    FsmSw_SphincsSha2_256fSimple_ComputeRoot(&roots[i * FSMSW_SPHINCSSHA2_256FSIMPLE_N], leaf, indices[i], idx_offset,
                                             sig_temp, FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_HEIGHT, ctx, fors_tree_addr);
    sig_temp = &sig_temp[FSMSW_SPHINCSSHA2_256FSIMPLE_N * FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_HEIGHT];
  }

  /* Hash horizontally across all tree roots to derive the public key. */
  FsmSw_SphincsSha2_256fSimple_Thash(pk, roots, FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_TREES, ctx, fors_pk_addr);
} // end: FsmSw_SphincsSha2_256fSimple_Fors_PkFromSig

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
