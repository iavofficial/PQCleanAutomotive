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
/** \addtogroup SLH_DSA_SHAKE_128fSimple
*    includes the modules for SLH_DSA_SHAKE_128fSimple
 ** @{ */
/** \addtogroup SLH_DSA_SHAKE_128fSimple_fors
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHAKE_128fSimple_fors.c
* \brief  description of SLH_DSA_SHAKE_128fSimple_fors.c
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
#include "SLH_DSA_SHAKE_128fSimple_FctWrapper.h"
#include "SLH_DSA_SHAKE_128fSimple_hash.h"
#include "SLH_DSA_SHAKE_128fSimple_thash.h"
#include "SLH_DSA_SHAKE_128fSimple_utils.h"
#include "SLH_DSA_SHAKE_128fSimple_utilsx1.h"
#include "SLH_DSA_SHAKE_address.h"

#include "SLH_DSA_SHAKE_128fSimple_fors.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define SLH_DSA_SIGN_ADDR_SIZE 8
/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct
{
  uint32 leaf_addrx[SLH_DSA_SIGN_ADDR_SIZE];
} SLH_DSA_SHAKE_128fSimple_ForsGenLeafInfo_T;
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
static void slh_dsa_shake_128fsimple_fors_GenSk(uint8 *const sk, const slh_dsa_shake_128f_ctx *const ctx,
                                                     const uint32 fors_leaf_addr[8]);
static void slh_dsa_shake_128fsimple_fors_SkToLeaf(uint8 *const leaf, const uint8 *const sk,
                                                        const slh_dsa_shake_128f_ctx *const ctx,
                                                        const uint32 fors_leaf_addr[8]);
static void slh_dsa_shake_128fsimple_fors_GenLeafx1(uint8 *const leaf, const slh_dsa_shake_128f_ctx *const ctx,
                                                         uint32 addr_idx, void *const info);
static void slh_dsa_shake_128fsimple_fors_MessageToIndices(uint32 *const indices, const uint8 *const m);
/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
 * \brief t.b.d
 *
 * \param[out] uint8                         *sk : t.b.d
 * \param[in]  const slh_dsa_shake_128f_ctx *ctx : t.b.d
 * \param[in]  const uint32    fors_leaf_addr[8] : t.b.d
 */
static void slh_dsa_shake_128fsimple_fors_GenSk(uint8 *const sk, const slh_dsa_shake_128f_ctx *const ctx,
                                                     const uint32 fors_leaf_addr[8])
{
  SLH_DSA_SHAKE_128fSimple_PrfAddr(sk, ctx, fors_leaf_addr);
} // end: slh_dsa_shake_128fsimple_fors_GenSk

/*====================================================================================================================*/
/**
 * \brief t.b.d
 *
 * \param[out] uint8                       *leaf : t.b.d
 * \param[in]  const uint8                   *sk : t.b.d
 * \param[in]  const slh_dsa_shake_128f_ctx *ctx : t.b.d
 * \param[in]  const uint32    fors_leaf_addr[8] : t.b.d
 */
static void slh_dsa_shake_128fsimple_fors_SkToLeaf(uint8 *const leaf, const uint8 *const sk,
                                                        const slh_dsa_shake_128f_ctx *const ctx,
                                                        const uint32 fors_leaf_addr[8])
{
  SLH_DSA_SHAKE_128fSimple_Thash(leaf, sk, 1, ctx, fors_leaf_addr);
} // end: slh_dsa_shake_128fsimple_fors_SkToLeaf

/*====================================================================================================================*/
/**
 * \brief t.b.d
 *
 * \param[out] uint8                       *leaf : t.b.d
 * \param[in]  const slh_dsa_shake_128f_ctx *ctx : t.b.d
 * \param[in]  uint32                   addr_idx : t.b.d
 * \param[in]  void                        *info : t.b.d
 */
static void slh_dsa_shake_128fsimple_fors_GenLeafx1(uint8 *const leaf, const slh_dsa_shake_128f_ctx *const ctx,
                                                         uint32 addr_idx, void *const info)
{
  /* polyspace +4 CERT-C:EXP36-C [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality.
    Ensured proper alignment and validity." */
  SLH_DSA_SHAKE_128fSimple_ForsGenLeafInfo_T *fors_info = info;
  uint32 *const fors_leaf_addr                               = fors_info->leaf_addrx;

  /* Only set the parts that the caller doesn't set */
  SLH_DSA_SHAKE_SetTreeIndex(fors_leaf_addr, addr_idx);
  SLH_DSA_SHAKE_SetType(fors_leaf_addr, SLH_DSA_ADDR_TYPE_FORSPRF);
  slh_dsa_shake_128fsimple_fors_GenSk(leaf, ctx, fors_leaf_addr);

  SLH_DSA_SHAKE_SetType(fors_leaf_addr, SLH_DSA_ADDR_TYPE_FORSTREE);
  slh_dsa_shake_128fsimple_fors_SkToLeaf(leaf, leaf, ctx, fors_leaf_addr);
} // end: slh_dsa_shake_128fsimple_fors_GenLeafx1

/*====================================================================================================================*/
/**
 * \brief Interprets m as SLH_DSA_SHAKE_128FSIMPLE_FORS_HEIGHT-bit uint32. Assumes m contains at least
 *        SLH_DSA_SHAKE_128FSIMPLE_FORS_HEIGHT * SLH_DSA_SHAKE_128FSIMPLE_FORS_TREES bits.
 *        Assumes indices has space for SLH_DSA_SHAKE_128FSIMPLE_FORS_TREES integers.
 *
 * \param[out] uint32 *indices : t.b.d
 * \param[in]  const uint8  *m : t.b.d
 *
 */
static void slh_dsa_shake_128fsimple_fors_MessageToIndices(uint32 *const indices, const uint8 *const m)
{
  uint32 i      = 0;
  uint32 j      = 0;
  uint32 offset = 0;

  for (i = 0; i < SLH_DSA_SHAKE_128FSIMPLE_FORS_TREES; i++)
  {
    indices[i] = 0;
    for (j = 0; j < SLH_DSA_SHAKE_128FSIMPLE_FORS_HEIGHT; j++)
    {
      indices[i] ^= (uint32)(((((uint32)(m[offset >> 3]) >> (offset & 0x7u)) & 0x1u)) << j);
      offset++;
    }
  }
} // end: slh_dsa_shake_128fsimple_fors_MessageToIndices
/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
 * \brief Signs a message m, deriving the secret key from sk_seed and the FTS address. Assumes m contains at least
 *        SLH_DSA_SHAKE_128FSIMPLE_FORS_HEIGHT * SLH_DSA_SHAKE_128FSIMPLE_FORS_TREES bits.
 *
 * \param[out] uint8                        *sig : t.b.d
 * \param[out] uint8                         *pk : t.b.d
 * \param[in]  const uint8                    *m : t.b.d
 * \param[in]  const slh_dsa_shake_128f_ctx *ctx : t.b.d
 * \param[in]  const uint32         fors_addr[8] : t.b.d
 */
void SLH_DSA_SHAKE_128fSimple_Fors_Sign(uint8 *const sig, uint8 *const pk, const uint8 *const m,
                                             const slh_dsa_shake_128f_ctx *const ctx, const uint32 fors_addr[8])
{
  uint32 indices[SLH_DSA_SHAKE_128FSIMPLE_FORS_TREES]                                = {0};
  uint8 roots[SLH_DSA_SHAKE_128FSIMPLE_FORS_TREES * SLH_DSA_SHAKE_128FSIMPLE_N] = {0};
  uint32 fors_tree_addr[SLH_DSA_SIGN_ADDR_SIZE]                                     = {0};
  SLH_DSA_SHAKE_128fSimple_ForsGenLeafInfo_T fors_info                               = {{0}};
  uint32 *const fors_leaf_addr                                                            = fors_info.leaf_addrx;
  uint32 fors_pk_addr[SLH_DSA_SIGN_ADDR_SIZE]                                       = {0};
  uint32 idx_offset                                                                       = 0;
  uint32 i                                                                                = 0;

  /* sig_temp is used to avoid modifying the input. */
  uint8 *sig_temp = sig;

  SLH_DSA_SHAKE_128fSimple_copy_keypair_addr(fors_tree_addr, fors_addr);
  SLH_DSA_SHAKE_128fSimple_copy_keypair_addr(fors_leaf_addr, fors_addr);

  SLH_DSA_SHAKE_128fSimple_copy_keypair_addr(fors_pk_addr, fors_addr);
  SLH_DSA_SHAKE_SetType(fors_pk_addr, SLH_DSA_ADDR_TYPE_FORSPK);

  slh_dsa_shake_128fsimple_fors_MessageToIndices(indices, m);

  for (i = 0; i < SLH_DSA_SHAKE_128FSIMPLE_FORS_TREES; i++)
  {
    idx_offset = i * (uint32)((uint64)((uint64)1u << SLH_DSA_SHAKE_128FSIMPLE_FORS_HEIGHT));

    SLH_DSA_SHAKE_SetTreeHeight(fors_tree_addr, 0);
    SLH_DSA_SHAKE_SetTreeIndex(fors_tree_addr, indices[i] + idx_offset);
    SLH_DSA_SHAKE_SetType(fors_tree_addr, SLH_DSA_ADDR_TYPE_FORSPRF);

    /* Include the secret key part that produces the selected leaf node. */
    slh_dsa_shake_128fsimple_fors_GenSk(sig_temp, ctx, fors_tree_addr);
    SLH_DSA_SHAKE_SetType(fors_tree_addr, SLH_DSA_ADDR_TYPE_FORSTREE);
    sig_temp = &sig_temp[SLH_DSA_SHAKE_128FSIMPLE_N];

    /* Compute the authentication path for this leaf node. */
    SLH_DSA_SHAKE_128fSimple_TreeHashX1(&roots[i * SLH_DSA_SHAKE_128FSIMPLE_N], sig_temp, ctx, indices[i],
                                             idx_offset, SLH_DSA_SHAKE_128FSIMPLE_FORS_HEIGHT,
                                             slh_dsa_shake_128fsimple_fors_GenLeafx1, fors_tree_addr, &fors_info);

    sig_temp = &sig_temp[SLH_DSA_SHAKE_128FSIMPLE_N * SLH_DSA_SHAKE_128FSIMPLE_FORS_HEIGHT];
  }

  /* Hash horizontally across all tree roots to derive the public key. */
  SLH_DSA_SHAKE_128fSimple_Thash(pk, roots, SLH_DSA_SHAKE_128FSIMPLE_FORS_TREES, ctx, fors_pk_addr);
} // end: SLH_DSA_SHAKE_128fSimple_Fors_Sign

/*====================================================================================================================*/
/**
 * \brief Derives the FORS public key from a signature. This can be used for verification by comparing to a known
 *        public key, or to subsequently verify a signature on the derived public key. The latter is the typical
 *        use-case when used as an FTS below an OTS in a hypertree. Assumes m contains at least
 *        SLH_DSA_SHAKE_128FSIMPLE_FORS_HEIGHT * SLH_DSA_SHAKE_128FSIMPLE_FORS_TREES bits.
 *
 * \param[out] uint8                         *pk : t.b.d
 * \param[in]  const uint8                  *sig : t.b.d
 * \param[in]  const uint8                    *m : t.b.d
 * \param[in]  const slh_dsa_shake_128f_ctx *ctx : t.b.d
 * \param[in]  const uint32         fors_addr[8] : t.b.d
 *
 */
void SLH_DSA_SHAKE_128fSimple_Fors_PkFromSig(uint8 *const pk, const uint8 *const sig, const uint8 *const m,
                                                  const slh_dsa_shake_128f_ctx *const ctx, const uint32 fors_addr[8])
{
  uint32 indices[SLH_DSA_SHAKE_128FSIMPLE_FORS_TREES]                                = {0};
  uint8 roots[SLH_DSA_SHAKE_128FSIMPLE_FORS_TREES * SLH_DSA_SHAKE_128FSIMPLE_N] = {0};
  uint8 leaf[SLH_DSA_SHAKE_128FSIMPLE_N]                                             = {0};
  uint32 fors_tree_addr[SLH_DSA_SIGN_ADDR_SIZE]                                     = {0};
  uint32 fors_pk_addr[SLH_DSA_SIGN_ADDR_SIZE]                                       = {0};
  uint32 idx_offset                                                                       = 0;
  uint32 i                                                                                = 0;

  /* sig_temp is used to avoid modifying the input. */
  const uint8 *sig_temp = sig;

  SLH_DSA_SHAKE_128fSimple_copy_keypair_addr(fors_tree_addr, fors_addr);
  SLH_DSA_SHAKE_128fSimple_copy_keypair_addr(fors_pk_addr, fors_addr);

  SLH_DSA_SHAKE_SetType(fors_tree_addr, SLH_DSA_ADDR_TYPE_FORSTREE);
  SLH_DSA_SHAKE_SetType(fors_pk_addr, SLH_DSA_ADDR_TYPE_FORSPK);

  slh_dsa_shake_128fsimple_fors_MessageToIndices(indices, m);

  for (i = 0; i < SLH_DSA_SHAKE_128FSIMPLE_FORS_TREES; i++)
  {
    idx_offset = i * (uint32)((uint64)((uint64)1u << SLH_DSA_SHAKE_128FSIMPLE_FORS_HEIGHT));

    SLH_DSA_SHAKE_SetTreeHeight(fors_tree_addr, 0);
    SLH_DSA_SHAKE_SetTreeIndex(fors_tree_addr, indices[i] + idx_offset);

    /* Derive the leaf from the included secret key part. */
    slh_dsa_shake_128fsimple_fors_SkToLeaf(leaf, sig_temp, ctx, fors_tree_addr);
    sig_temp = &sig_temp[SLH_DSA_SHAKE_128FSIMPLE_N];

    /* Derive the corresponding root node of this tree. */
    SLH_DSA_SHAKE_128fSimple_ComputeRoot(&roots[i * SLH_DSA_SHAKE_128FSIMPLE_N], leaf, indices[i], idx_offset,
                                              sig_temp, SLH_DSA_SHAKE_128FSIMPLE_FORS_HEIGHT, ctx, fors_tree_addr);
    sig_temp = &sig_temp[SLH_DSA_SHAKE_128FSIMPLE_N * SLH_DSA_SHAKE_128FSIMPLE_FORS_HEIGHT];
  }

  /* Hash horizontally across all tree roots to derive the public key. */
  SLH_DSA_SHAKE_128fSimple_Thash(pk, roots, SLH_DSA_SHAKE_128FSIMPLE_FORS_TREES, ctx, fors_pk_addr);
} // end: SLH_DSA_SHAKE_128fSimple_Fors_PkFromSig

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
