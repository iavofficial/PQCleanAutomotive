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
/** \addtogroup SLH_DSA_SHAKE_256sSimple
*    includes the modules for SLH_DSA_SHAKE_256sSimple
 ** @{ */
/** \addtogroup SLH_DSA_SHAKE_256sSimple_sign
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHAKE_256sSimple_sign.c
* \brief  description of SLH_DSA_SHAKE_256sSimple_sign.c
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
#include "SLH_DSA_SHAKE_256sSimple_FctWrapper.h"
#include "SLH_DSA_SHAKE_256sSimple_context.h"
#include "SLH_DSA_SHAKE_256sSimple_fors.h"
#include "SLH_DSA_SHAKE_256sSimple_hash.h"
#include "SLH_DSA_SHAKE_256sSimple_merkle.h"
#include "SLH_DSA_SHAKE_256sSimple_params.h"
#include "SLH_DSA_SHAKE_256sSimple_thash.h"
#include "SLH_DSA_SHAKE_256sSimple_utils.h"
#include "SLH_DSA_SHAKE_256sSimple_wots.h"
#include "SLH_DSA_SHAKE_address.h"

#include "SLH_DSA_SHAKE_256sSimple_sign.h"
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
static void slh_dsa_shake_256ssimple_crypto_sign_SeedKeyPair(uint8 *const pk, uint8 *const sk,
                                                                  const uint8 *const seed);
/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
 * \brief Generates an SPX key pair given a seed of length
 *        Format sk: [SK_SEED  || SK_PRF || PUB_SEED || root]
 *        Format pk: [PUB_SEED || root]
 *
 * \param[out] uint8         *pk : t.b.d.
 * \param[out] uint8         *sk : t.b.d.
 * \param[in]  const uint8 *seed : t.b.d.
 *
 */
static void slh_dsa_shake_256ssimple_crypto_sign_SeedKeyPair(uint8 *const pk, uint8 *const sk,
                                                                  const uint8 *const seed)
{
  slh_dsa_shake_256s_ctx ctx = {{0}};

  /* Initialize SK_SEED, SK_PRF and PUB_SEED from seed. */
  FsmSw_CommonLib_MemCpy(sk, seed, SLH_DSA_SHAKE_256SSIMPLE_CRYPTO_SEEDBYTES);

  FsmSw_CommonLib_MemCpy(pk, &sk[2u * SLH_DSA_SHAKE_256SSIMPLE_N], SLH_DSA_SHAKE_256SSIMPLE_N);

  FsmSw_CommonLib_MemCpy(ctx.pub_seed, pk, SLH_DSA_SHAKE_256SSIMPLE_N);
  FsmSw_CommonLib_MemCpy(ctx.sk_seed, sk, SLH_DSA_SHAKE_256SSIMPLE_N);

  /* This hook allows the hash function instantiation to do whatever preparation or computation it needs,
   * based on the public seed. */
  SLH_DSA_SHAKE_256sSimple_InitializeHashFunction(&ctx);

  /* Compute root node of the top-most subtree. */
  SLH_DSA_SHAKE_256sSimple_Merkle_GenRoot(&sk[3u * SLH_DSA_SHAKE_256SSIMPLE_N], &ctx);

  /* cleanup */
  SLH_DSA_SHAKE_256sSimple_1FreeHashFunction(&ctx);

  FsmSw_CommonLib_MemCpy(&pk[SLH_DSA_SHAKE_256SSIMPLE_N], &sk[3u * SLH_DSA_SHAKE_256SSIMPLE_N],
                         SLH_DSA_SHAKE_256SSIMPLE_N);

  return;
} // end: slh_dsa_shake_256ssimple_crypto_sign_SeedKeyPair
/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
 * \brief Generates an SPX key pair.
 *        Format sk: [SK_SEED  || SK_PRF || PUB_SEED || root]
 *        Format pk: [PUB_SEED || root]
 *
 * \param[out] uint8 *pk:   t.b.d.
 * \param[out] uint8 *sk:   t.b.d.
 *
 */
void SLH_DSA_SHAKE_256sSimple_Crypto_Sign_KeyPair(uint8 *const pk, uint8 *const sk)
{
  uint8 seed[SLH_DSA_SHAKE_256SSIMPLE_CRYPTO_SEEDBYTES] = {0};
  (void)FsmSw_CommonLib_RandomBytes(seed, SLH_DSA_SHAKE_256SSIMPLE_CRYPTO_SEEDBYTES);
  slh_dsa_shake_256ssimple_crypto_sign_SeedKeyPair(pk, sk, seed);

  return;
} // end: SLH_DSA_SHAKE_256sSimple_Crypto_Sign_KeyPair

/*====================================================================================================================*/
/**
 * \brief Returns an array containing a detached signature.
 *
 * \param[out] uint8      *sig : t.b.d.
 * \param[out] uint32  *siglen : t.b.d.
 * \param[in]  const uint8  *m : t.b.d.
 * \param[in]  uint32     mlen : t.b.d.
 * \param[in]  const uint8 *sk : t.b.d.
 *
 */
/* polyspace +12 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +10 CERT-C:DCL15-C [Justified:]"This is an interface function
designed for use by other systems that aim to integrate the SLH_DSA." */
/* polyspace +8 CERT-C:DCL19-C [Justified:]"This is an interface function
designed for use by other systems that aim to integrate the SLH_DSA." */
/* polyspace +6 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:8.7 [Justified:]"This is an interface function
designed for use by other systems that aim to integrate the SLH_DSA." */
void SLH_DSA_SHAKE_256sSimple_Crypto_Sign_Signature(uint8 *const sig, uint32 *const siglen, const uint8 *const m,
                                                         uint32 mlen, const uint8 *const sk)
{
  slh_dsa_shake_256s_ctx ctx                                = {{0}};
  const uint8 *const sk_prf                                 = &sk[SLH_DSA_SHAKE_256SSIMPLE_N];
  const uint8 *const pk                                     = &sk[2u * SLH_DSA_SHAKE_256SSIMPLE_N];
  uint8 optrand[SLH_DSA_SHAKE_256SSIMPLE_N]            = {0};
  uint8 mhash[SLH_DSA_SHAKE_256SSIMPLE_FORS_MSG_BYTES] = {0};
  uint8 root[SLH_DSA_SHAKE_256SSIMPLE_N]               = {0};
  uint32 i                                                  = 0;
  uint64 tree                                               = 0;
  uint32 idx_leaf                                           = 0;
  uint32 wots_addr[SLH_DSA_SIGN_ADDR_SIZE]            = {0};
  uint32 tree_addr[SLH_DSA_SIGN_ADDR_SIZE]            = {0};

  /* sig_temp is used to avoid modifying the input. */
  uint8 *sig_temp = sig;

  FsmSw_CommonLib_MemCpy(ctx.sk_seed, sk, SLH_DSA_SHAKE_256SSIMPLE_N);
  FsmSw_CommonLib_MemCpy(ctx.pub_seed, pk, SLH_DSA_SHAKE_256SSIMPLE_N);

  /* This hook allows the hash function instantiation to do whatever preparation or computation it needs,
   * based on the public seed. */
  SLH_DSA_SHAKE_256sSimple_InitializeHashFunction(&ctx);

  SLH_DSA_SHAKE_SetType(wots_addr, SLH_DSA_ADDR_TYPE_WOTS);
  SLH_DSA_SHAKE_SetType(tree_addr, SLH_DSA_ADDR_TYPE_HASHTREE);

  /* Optionally, signing can be made non-deterministic using optrand. This can help counter side-channel attacks
   * that would benefit from getting a large number of traces when the signer uses the same nodes. */
  (void)FsmSw_CommonLib_RandomBytes(optrand, SLH_DSA_SHAKE_256SSIMPLE_N);
  /* Compute the digest randomization value. */
  SLH_DSA_SHAKE_256sSimple_GenMessageRandom(sig_temp, sk_prf, optrand, m, mlen, &ctx);

  /* Derive the message digest and leaf index from R, PK and M. */
  SLH_DSA_SHAKE_256sSimple_HashMessage(mhash, &tree, &idx_leaf, sig_temp, pk, m, mlen, &ctx);
  sig_temp = &sig_temp[SLH_DSA_SHAKE_256SSIMPLE_N];

  SLH_DSA_SHAKE_SetTreeAddr(wots_addr, tree);
  SLH_DSA_SHAKE_256sSimple_set_keypair_addr(wots_addr, idx_leaf);

  /* Sign the message hash using FORS. */
  SLH_DSA_SHAKE_256sSimple_Fors_Sign(sig_temp, root, mhash, &ctx, wots_addr);
  sig_temp = &sig_temp[SLH_DSA_SHAKE_256SSIMPLE_FORS_BYTES];

  for (i = 0; i < SLH_DSA_SHAKE_256SSIMPLE_D; i++)
  {
    SLH_DSA_SHAKE_SetLayerAddr(tree_addr, i);
    SLH_DSA_SHAKE_SetTreeAddr(tree_addr, tree);

    SLH_DSA_SHAKE_CopySubTreeAddr(wots_addr, tree_addr);
    SLH_DSA_SHAKE_256sSimple_set_keypair_addr(wots_addr, idx_leaf);

    SLH_DSA_SHAKE_256sSimple_Merkle_Sign(sig_temp, root, &ctx, wots_addr, tree_addr, idx_leaf);
    sig_temp = &sig_temp[SLH_DSA_SHAKE_256SSIMPLE_WOTS_BYTES +
                         (SLH_DSA_SHAKE_256SSIMPLE_TREE_HEIGHT * SLH_DSA_SHAKE_256SSIMPLE_N)];

    /* Update the indices for the next layer. */
    idx_leaf = (uint32)(tree & (((uint64)((uint64)1u << SLH_DSA_SHAKE_256SSIMPLE_TREE_HEIGHT)) - 1u));
    tree     = tree >> SLH_DSA_SHAKE_256SSIMPLE_TREE_HEIGHT;
  }

  SLH_DSA_SHAKE_256sSimple_1FreeHashFunction(&ctx);

  *siglen = SLH_DSA_SHAKE_256SSIMPLE_BYTES;

  return;
} // end: SLH_DSA_SHAKE_256sSimple_Crypto_Sign_Signature

/*====================================================================================================================*/
/**
 * \brief Verifies a detached signature and message under a given public key.
 *
 * \param[out] uint8      *sig : t.b.d.
 * \param[in]  uint32   siglen : t.b.d.
 * \param[in]  const uint8  *m : t.b.d.
 * \param[in]  uint32     mlen : t.b.d.
 * \param[in]  const uint8 *pk : t.b.d.
 *
 * \returns ERR_OK on success, ERR_NOT_OK on error.
 *
 */
/* polyspace +12 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +10 CERT-C:DCL15-C [Justified:]"This is an interface function
designed for use by other systems that aim to integrate the SLH_DSA." */
/* polyspace +8 CERT-C:DCL19-C [Justified:]"This is an interface function
designed for use by other systems that aim to integrate the SLH_DSA." */
/* polyspace +6 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:8.7 [Justified:]"This is an interface function
designed for use by other systems that aim to integrate the SLH_DSA." */
uint8 SLH_DSA_SHAKE_256sSimple_Crypto_Sign_Verify(const uint8 *const sig, uint32 siglen, const uint8 *const m,
                                                       uint32 mlen, const uint8 *const pk)
{
  slh_dsa_shake_256s_ctx ctx                                = {{0}};
  const uint8 *const pub_root                               = &pk[SLH_DSA_SHAKE_256SSIMPLE_N];
  uint8 mhash[SLH_DSA_SHAKE_256SSIMPLE_FORS_MSG_BYTES] = {0};
  uint8 wots_pk[SLH_DSA_SHAKE_256SSIMPLE_WOTS_BYTES]   = {0};
  uint8 root[SLH_DSA_SHAKE_256SSIMPLE_N]               = {0};
  uint8 leaf[SLH_DSA_SHAKE_256SSIMPLE_N]               = {0};
  uint32 i                                                  = 0;
  uint64 tree                                               = 0;
  uint32 idx_leaf                                           = 0;
  uint32 wots_addr[SLH_DSA_SIGN_ADDR_SIZE]            = {0};
  uint32 tree_addr[SLH_DSA_SIGN_ADDR_SIZE]            = {0};
  uint32 wots_pk_addr[SLH_DSA_SIGN_ADDR_SIZE]         = {0};
  uint8 retVal                                              = ERR_OK;

  /* sig_temp is used to avoid modifying the input. */
  const uint8 *sig_temp = sig;

  if (siglen != SLH_DSA_SHAKE_256SSIMPLE_BYTES)
  {
    retVal = ERR_NOT_OK;
  }

  FsmSw_CommonLib_MemCpy(ctx.pub_seed, pk, SLH_DSA_SHAKE_256SSIMPLE_N);

  /* This hook allows the hash function instantiation to do whatever preparation or computation it needs,
   * based on the public seed. */
  SLH_DSA_SHAKE_256sSimple_InitializeHashFunction(&ctx);

  SLH_DSA_SHAKE_SetType(wots_addr, SLH_DSA_ADDR_TYPE_WOTS);
  SLH_DSA_SHAKE_SetType(tree_addr, SLH_DSA_ADDR_TYPE_HASHTREE);
  SLH_DSA_SHAKE_SetType(wots_pk_addr, SLH_DSA_ADDR_TYPE_WOTSPK);

  /* Derive the message digest and leaf index from R || PK || M.
   * The additional SLH_DSA_SHAKE_256SSIMPLE_N is a result of the hash domain separator. */
  SLH_DSA_SHAKE_256sSimple_HashMessage(mhash, &tree, &idx_leaf, sig_temp, pk, m, mlen, &ctx);
  sig_temp = &sig_temp[SLH_DSA_SHAKE_256SSIMPLE_N];

  /* Layer correctly defaults to 0, so no need to SLH_DSA_set_layer_addr */
  SLH_DSA_SHAKE_SetTreeAddr(wots_addr, tree);
  SLH_DSA_SHAKE_256sSimple_set_keypair_addr(wots_addr, idx_leaf);

  SLH_DSA_SHAKE_256sSimple_Fors_PkFromSig(root, sig_temp, mhash, &ctx, wots_addr);
  sig_temp = &sig_temp[SLH_DSA_SHAKE_256SSIMPLE_FORS_BYTES];

  /* For each subtree.. */
  for (i = 0; i < SLH_DSA_SHAKE_256SSIMPLE_D; i++)
  {
    SLH_DSA_SHAKE_SetLayerAddr(tree_addr, i);
    SLH_DSA_SHAKE_SetTreeAddr(tree_addr, tree);

    SLH_DSA_SHAKE_CopySubTreeAddr(wots_addr, tree_addr);
    SLH_DSA_SHAKE_256sSimple_set_keypair_addr(wots_addr, idx_leaf);

    SLH_DSA_SHAKE_256sSimple_copy_keypair_addr(wots_pk_addr, wots_addr);

    /* The WOTS public key is only correct if the signature was correct. Initially, root is the FORS pk, but on
     * subsequent iterations it is the root of the subtree below the currently processed subtree. */
    SLH_DSA_SHAKE_256sSimple_Wots_PkFromSig(wots_pk, sig_temp, root, &ctx, wots_addr);
    sig_temp = &sig_temp[SLH_DSA_SHAKE_256SSIMPLE_WOTS_BYTES];

    /* Compute the leaf node using the WOTS public key. */
    SLH_DSA_SHAKE_256sSimple_Thash(leaf, wots_pk, SLH_DSA_SHAKE_256SSIMPLE_WOTS_LEN, &ctx, wots_pk_addr);

    /* Compute the root node of this subtree. */
    SLH_DSA_SHAKE_256sSimple_ComputeRoot(root, leaf, idx_leaf, 0, sig_temp,
                                              SLH_DSA_SHAKE_256SSIMPLE_TREE_HEIGHT, &ctx, tree_addr);
    sig_temp = &sig_temp[SLH_DSA_SHAKE_256SSIMPLE_TREE_HEIGHT * SLH_DSA_SHAKE_256SSIMPLE_N];

    /* Update the indices for the next layer. */
    idx_leaf = (uint32)(tree & (((uint64)((uint64)1u << SLH_DSA_SHAKE_256SSIMPLE_TREE_HEIGHT)) - 1u));
    tree     = tree >> SLH_DSA_SHAKE_256SSIMPLE_TREE_HEIGHT;
  }

  /* cleanup */
  SLH_DSA_SHAKE_256sSimple_1FreeHashFunction(&ctx);

  /* Check if the root node equals the root node in the public key. */
  if (FsmSw_CommonLib_MemCmp(root, pub_root, SLH_DSA_SHAKE_256SSIMPLE_N) != 0u)
  {
    retVal = ERR_NOT_OK;
  }

  return retVal;
} // end: SLH_DSA_SHAKE_256sSimple_Crypto_Sign_Verify

/*====================================================================================================================*/
/**
 * \brief Returns an array containing the signature followed by the message.
 *
 * \param[out] uint8       *sm : t.b.d.
 * \param[out] uint32   *smlen : t.b.d.
 * \param[in]  const uint8  *m : t.b.d.
 * \param[in]  uint32     mlen : t.b.d.
 * \param[in]  const uint8 *sk : t.b.d.
 *
 */
/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void SLH_DSA_SHAKE_256sSimple_Crypto_Sign(uint8 *const sm, uint32 *const smlen, const uint8 *const m, uint32 mlen,
                                               const uint8 *const sk)
{
  uint32 siglen = 0;

  (void)SLH_DSA_SHAKE_256sSimple_Crypto_Sign_Signature(sm, &siglen, m, mlen, sk);

  FsmSw_CommonLib_MemMove(&sm[SLH_DSA_SHAKE_256SSIMPLE_BYTES], m, mlen);
  *smlen = siglen + mlen;

  return;
} // end: SLH_DSA_SHAKE_256sSimple_Crypto_Sign

/*====================================================================================================================*/
/**
 * \brief Verifies a given signature-message pair under a given public key.
 *
 * \param[out] uint8        *m : t.b.d.
 * \param[out] uint32    *mlen : t.b.d.
 * \param[in]  const uint8 *sm : t.b.d.
 * \param[in]  uint32    smlen : t.b.d.
 * \param[in]  const uint8 *pk : t.b.d.
 *
 * \returns ERR_OK on success, ERR_NOT_OK on error.
 *
 */
/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity
and avoids confusion with other functions. Therefore, this warning is a false positive." */
uint8 SLH_DSA_SHAKE_256sSimple_Crypto_Sign_Open(uint8 *const m, uint32 *const mlen, const uint8 *const sm,
                                                     uint32 smlen, const uint8 *const pk)
{
  uint8 retVal = ERR_OK;

  /* The API caller does not necessarily know what size a signature should be but SLH-DSA signatures are always
   * exactly SLH_DSA_SHAKE_256SSIMPLE_BYTES. */
  if (smlen < SLH_DSA_SHAKE_256SSIMPLE_BYTES)
  {
    FsmSw_CommonLib_MemSet(m, 0, smlen);
    *mlen  = 0;
    retVal = ERR_NOT_OK;
  }
  else
  {
    *mlen = smlen - SLH_DSA_SHAKE_256SSIMPLE_BYTES;

    if (0 != SLH_DSA_SHAKE_256sSimple_Crypto_Sign_Verify(sm, SLH_DSA_SHAKE_256SSIMPLE_BYTES,
                                                              &sm[SLH_DSA_SHAKE_256SSIMPLE_BYTES], *mlen, pk))
    {
      FsmSw_CommonLib_MemSet(m, 0, smlen);
      *mlen  = 0;
      retVal = ERR_NOT_OK;
    }

    /* If verification was successful, move the message to the right place. */
    FsmSw_CommonLib_MemMove(m, &sm[SLH_DSA_SHAKE_256SSIMPLE_BYTES], *mlen);
  }
  return retVal;
} // end: SLH_DSA_SHAKE_256sSimple_Crypto_Sign_Open

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */