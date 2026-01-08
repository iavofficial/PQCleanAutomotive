/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsShake_128sSimple
*    includes the modules for SphincsShake_128sSimple
 ** @{ */
/** \addtogroup SphincsShake_128sSimple_sign
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsShake_128sSimple_sign.c
* \brief  description of FsmSw_SphincsShake_128sSimple_sign.c
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
#include "FsmSw_SphincsShake_128sSimple_FctWrapper.h"
#include "FsmSw_SphincsShake_128sSimple_context.h"
#include "FsmSw_SphincsShake_128sSimple_fors.h"
#include "FsmSw_SphincsShake_128sSimple_hash.h"
#include "FsmSw_SphincsShake_128sSimple_merkle.h"
#include "FsmSw_SphincsShake_128sSimple_params.h"
#include "FsmSw_SphincsShake_128sSimple_thash.h"
#include "FsmSw_SphincsShake_128sSimple_utils.h"
#include "FsmSw_SphincsShake_128sSimple_wots.h"
#include "FsmSw_Sphincs_shake_address.h"

#include "FsmSw_SphincsShake_128sSimple_sign.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
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
static void fsmsw_sphincsshake_128ssimple_crypto_sign_SeedKeyPair(uint8 *const pk, uint8 *const sk,
                                                                  const uint8 *const seed);
/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
 * \brief Generates an SPX key pair given a seed of length
 *        Format sk: [SK_SEED || SK_PRF || PUB_SEED || root]
 *        Format pk: [PUB_SEED || root]
 *
 * \param[out] uint8         *pk: t.b.d.
 * \param[out] uint8         *sk: t.b.d.
 * \param[in]  const uint8 *seed: t.b.d.
 *
 */
static void fsmsw_sphincsshake_128ssimple_crypto_sign_SeedKeyPair(uint8 *const pk, uint8 *const sk,
                                                                  const uint8 *const seed)
{
  sphincs_shake_128s_ctx ctx = {{0}};

  /* Initialize SK_SEED, SK_PRF and PUB_SEED from seed. */
  FsmSw_CommonLib_MemCpy(sk, seed, FSMSW_SPHINCSSHAKE_128SSIMPLE_CRYPTO_SEEDBYTES);

  FsmSw_CommonLib_MemCpy(pk, &sk[2u * FSMSW_SPHINCSSHAKE_128SSIMPLE_N], FSMSW_SPHINCSSHAKE_128SSIMPLE_N);

  FsmSw_CommonLib_MemCpy(ctx.pub_seed, pk, FSMSW_SPHINCSSHAKE_128SSIMPLE_N);
  FsmSw_CommonLib_MemCpy(ctx.sk_seed, sk, FSMSW_SPHINCSSHAKE_128SSIMPLE_N);

  /* This hook allows the hash function instantiation to do whatever preparation or computation it needs, based on
   * the public seed. */
  FsmSw_SphincsShake_128sSimple_InitializeHashFunction(&ctx);

  /* Compute root node of the top-most subtree. */
  FsmSw_SphincsShake_128sSimple_Merkle_GenRoot(&sk[3u * FSMSW_SPHINCSSHAKE_128SSIMPLE_N], &ctx);

  /* cleanup */
  FsmSw_SphincsShake_128sSimple_1FreeHashFunction(&ctx);

  FsmSw_CommonLib_MemCpy(&pk[FSMSW_SPHINCSSHAKE_128SSIMPLE_N], &sk[3u * FSMSW_SPHINCSSHAKE_128SSIMPLE_N],
                         FSMSW_SPHINCSSHAKE_128SSIMPLE_N);

  return;
} // end: fsmsw_sphincsshake_128ssimple_crypto_sign_SeedKeyPair
/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
 * \brief Generates an SPX key pair.
 *        Format sk: [SK_SEED || SK_PRF || PUB_SEED || root]
 *        Format pk: [PUB_SEED || root]
 *
 * \param[out] uint8 *pk : t.b.d.
 * \param[out] uint8 *sk : t.b.d.
 *
 */
void FsmSw_SphincsShake_128sSimple_Crypto_Sign_KeyPair(uint8 *const pk, uint8 *const sk)
{
  uint8 seed[FSMSW_SPHINCSSHAKE_128SSIMPLE_CRYPTO_SEEDBYTES] = {0};
  (void)FsmSw_CommonLib_RandomBytes(seed, FSMSW_SPHINCSSHAKE_128SSIMPLE_CRYPTO_SEEDBYTES);
  fsmsw_sphincsshake_128ssimple_crypto_sign_SeedKeyPair(pk, sk, seed);

  return;
} // end: FsmSw_SphincsShake_128sSimple_Crypto_Sign_KeyPair

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
designed for use by other systems that aim to integrate the Sphincs." */
/* polyspace +8 CERT-C:DCL19-C [Justified:]"This is an interface function
designed for use by other systems that aim to integrate the Sphincs." */
/* polyspace +6 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:8.7 [Justified:]"This is an interface function
designed for use by other systems that aim to integrate the Sphincs." */
void FsmSw_SphincsShake_128sSimple_Crypto_Sign_Signature(uint8 *const sig, uint32 *const siglen, const uint8 *const m,
                                                         uint32 mlen, const uint8 *const sk)
{
  sphincs_shake_128s_ctx ctx = {{0}};

  const uint8 *const sk_prf = &sk[FSMSW_SPHINCSSHAKE_128SSIMPLE_N];
  const uint8 *const pk     = &sk[2u * FSMSW_SPHINCSSHAKE_128SSIMPLE_N];

  uint8 optrand[FSMSW_SPHINCSSHAKE_128SSIMPLE_N];
  uint8 mhash[FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_MSG_BYTES];
  uint8 root[FSMSW_SPHINCSSHAKE_128SSIMPLE_N];
  uint32 i;
  uint64 tree;
  uint32 idx_leaf;
  uint32 wots_addr[FSMSW_SPHINCS_ADDR_SIZE] = {0};
  uint32 tree_addr[FSMSW_SPHINCS_ADDR_SIZE] = {0};

  /* sig_temp is used to avoid modifying the input. */
  uint8 *sig_temp = sig;

  FsmSw_CommonLib_MemCpy(ctx.sk_seed, sk, FSMSW_SPHINCSSHAKE_128SSIMPLE_N);
  FsmSw_CommonLib_MemCpy(ctx.pub_seed, pk, FSMSW_SPHINCSSHAKE_128SSIMPLE_N);

  /* This hook allows the hash function instantiation to do whatever preparation or computation it needs,
   * based on the public seed. */
  FsmSw_SphincsShake_128sSimple_InitializeHashFunction(&ctx);

  FsmSw_SphincsShake_SetType(wots_addr, FSMSW_SPHINCS_ADDR_TYPE_WOTS);
  FsmSw_SphincsShake_SetType(tree_addr, FSMSW_SPHINCS_ADDR_TYPE_HASHTREE);

  /* Optionally, signing can be made non-deterministic using optrand. This can help counter side-channel attacks
   * that would benefit from getting a large number of traces when the signer uses the same nodes. */
  (void)FsmSw_CommonLib_RandomBytes(optrand, FSMSW_SPHINCSSHAKE_128SSIMPLE_N);
  /* Compute the digest randomization value. */
  FsmSw_SphincsShake_128sSimple_GenMessageRandom(sig_temp, sk_prf, optrand, m, mlen, &ctx);

  /* Derive the message digest and leaf index from R, PK and M. */
  FsmSw_SphincsShake_128sSimple_HashMessage(mhash, &tree, &idx_leaf, sig_temp, pk, m, mlen, &ctx);
  sig_temp = &sig_temp[FSMSW_SPHINCSSHAKE_128SSIMPLE_N];

  FsmSw_SphincsShake_SetTreeAddr(wots_addr, tree);
  FsmSw_SphincsShake_128sSimple_set_keypair_addr(wots_addr, idx_leaf);

  /* Sign the message hash using FORS. */
  FsmSw_SphincsShake_128sSimple_Fors_Sign(sig_temp, root, mhash, &ctx, wots_addr);
  sig_temp = &sig_temp[FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_BYTES];

  for (i = 0; i < FSMSW_SPHINCSSHAKE_128SSIMPLE_D; i++)
  {
    FsmSw_SphincsShake_SetLayerAddr(tree_addr, i);
    FsmSw_SphincsShake_SetTreeAddr(tree_addr, tree);

    FsmSw_SphincsShake_CopySubTreeAddr(wots_addr, tree_addr);
    FsmSw_SphincsShake_128sSimple_set_keypair_addr(wots_addr, idx_leaf);

    FsmSw_SphincsShake_128sSimple_Merkle_Sign(sig_temp, root, &ctx, wots_addr, tree_addr, idx_leaf);
    sig_temp = &sig_temp[FSMSW_SPHINCSSHAKE_128SSIMPLE_WOTS_BYTES +
                         (FSMSW_SPHINCSSHAKE_128SSIMPLE_TREE_HEIGHT * FSMSW_SPHINCSSHAKE_128SSIMPLE_N)];

    /* Update the indices for the next layer. */
    idx_leaf = (uint32)(tree & (((uint64)((uint64)1u << FSMSW_SPHINCSSHAKE_128SSIMPLE_TREE_HEIGHT)) - 1u));
    tree     = tree >> FSMSW_SPHINCSSHAKE_128SSIMPLE_TREE_HEIGHT;
  }

  FsmSw_SphincsShake_128sSimple_1FreeHashFunction(&ctx);

  *siglen = FSMSW_SPHINCSSHAKE_128SSIMPLE_BYTES;

  return;
} // end: FsmSw_SphincsShake_128sSimple_Crypto_Sign_Signature

/*====================================================================================================================*/
/**
 * \brief Verifies a detached signature and message under a given public key.
 *
 * \param[in] const uint8 *sig : t.b.d.
 * \param[in] uint32    siglen : t.b.d.
 * \param[in] const uint8   *m : t.b.d.
 * \param[in] uint32      mlen : t.b.d.
 * \param[in] const uint8  *pk : t.b.d.
 *
 * \returns ERR_OK on success, ERR_NOT_OK on error.
 *
 */
/* polyspace +12 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +10 CERT-C:DCL15-C [Justified:]"This is an interface function
designed for use by other systems that aim to integrate the Sphincs." */
/* polyspace +8 CERT-C:DCL19-C [Justified:]"This is an interface function
designed for use by other systems that aim to integrate the Sphincs." */
/* polyspace +6 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:8.7 [Justified:]"This is an interface function
designed for use by other systems that aim to integrate the Sphincs." */
uint8 FsmSw_SphincsShake_128sSimple_Crypto_Sign_Verify(const uint8 *const sig, uint32 siglen, const uint8 *const m,
                                                       uint32 mlen, const uint8 *const pk)
{
  sphincs_shake_128s_ctx ctx                                = {{0}};
  const uint8 *const pub_root                               = &pk[FSMSW_SPHINCSSHAKE_128SSIMPLE_N];
  uint8 mhash[FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_MSG_BYTES] = {0};
  uint8 wots_pk[FSMSW_SPHINCSSHAKE_128SSIMPLE_WOTS_BYTES]   = {0};
  uint8 root[FSMSW_SPHINCSSHAKE_128SSIMPLE_N]               = {0};
  uint8 leaf[FSMSW_SPHINCSSHAKE_128SSIMPLE_N]               = {0};
  uint32 i                                                  = 0;
  uint64 tree                                               = 0;
  uint32 idx_leaf                                           = 0;
  uint32 wots_addr[FSMSW_SPHINCS_ADDR_SIZE]                 = {0};
  uint32 tree_addr[FSMSW_SPHINCS_ADDR_SIZE]                 = {0};
  uint32 wots_pk_addr[FSMSW_SPHINCS_ADDR_SIZE]              = {0};
  uint8 retVal                                              = ERR_OK;

  /* sig_temp is used to avoid modifying the input. */
  const uint8 *sig_temp = sig;

  if (siglen != FSMSW_SPHINCSSHAKE_128SSIMPLE_BYTES)
  {
    retVal = ERR_NOT_OK;
  }

  FsmSw_CommonLib_MemCpy(ctx.pub_seed, pk, FSMSW_SPHINCSSHAKE_128SSIMPLE_N);

  /* This hook allows the hash function instantiation to do whatever preparation or computation it needs, based on
   * the public seed. */
  FsmSw_SphincsShake_128sSimple_InitializeHashFunction(&ctx);

  FsmSw_SphincsShake_SetType(wots_addr, FSMSW_SPHINCS_ADDR_TYPE_WOTS);
  FsmSw_SphincsShake_SetType(tree_addr, FSMSW_SPHINCS_ADDR_TYPE_HASHTREE);
  FsmSw_SphincsShake_SetType(wots_pk_addr, FSMSW_SPHINCS_ADDR_TYPE_WOTSPK);

  /* Derive the message digest and leaf index from R || PK || M. */
  /* The additional FSMSW_SPHINCSSHAKE_128SSIMPLE_N is a result of the hash domain separator. */
  FsmSw_SphincsShake_128sSimple_HashMessage(mhash, &tree, &idx_leaf, sig_temp, pk, m, mlen, &ctx);
  sig_temp = &sig_temp[FSMSW_SPHINCSSHAKE_128SSIMPLE_N];

  /* Layer correctly defaults to 0, so no need to FsmSw_Sphincs_set_layer_addr */
  FsmSw_SphincsShake_SetTreeAddr(wots_addr, tree);
  FsmSw_SphincsShake_128sSimple_set_keypair_addr(wots_addr, idx_leaf);

  FsmSw_SphincsShake_128sSimple_Fors_PkFromSig(root, sig_temp, mhash, &ctx, wots_addr);
  sig_temp = &sig_temp[FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_BYTES];

  /* For each subtree.. */
  for (i = 0; i < FSMSW_SPHINCSSHAKE_128SSIMPLE_D; i++)
  {
    FsmSw_SphincsShake_SetLayerAddr(tree_addr, i);
    FsmSw_SphincsShake_SetTreeAddr(tree_addr, tree);

    FsmSw_SphincsShake_CopySubTreeAddr(wots_addr, tree_addr);
    FsmSw_SphincsShake_128sSimple_set_keypair_addr(wots_addr, idx_leaf);

    FsmSw_SphincsShake_128sSimple_copy_keypair_addr(wots_pk_addr, wots_addr);

    /* The WOTS public key is only correct if the signature was correct. Initially, root is the FORS pk, but on
     * subsequent iterations it is the root of the subtree below the currently processed subtree. */
    FsmSw_SphincsShake_128sSimple_Wots_PkFromSig(wots_pk, sig_temp, root, &ctx, wots_addr);
    sig_temp = &sig_temp[FSMSW_SPHINCSSHAKE_128SSIMPLE_WOTS_BYTES];

    /* Compute the leaf node using the WOTS public key. */
    FsmSw_SphincsShake_128sSimple_Thash(leaf, wots_pk, FSMSW_SPHINCSSHAKE_128SSIMPLE_WOTS_LEN, &ctx, wots_pk_addr);

    /* Compute the root node of this subtree. */
    FsmSw_SphincsShake_128sSimple_ComputeRoot(root, leaf, idx_leaf, 0, sig_temp,
                                              FSMSW_SPHINCSSHAKE_128SSIMPLE_TREE_HEIGHT, &ctx, tree_addr);
    sig_temp = &sig_temp[FSMSW_SPHINCSSHAKE_128SSIMPLE_TREE_HEIGHT * FSMSW_SPHINCSSHAKE_128SSIMPLE_N];

    /* Update the indices for the next layer. */
    idx_leaf = (uint32)(tree & (((uint64)((uint64)1u << FSMSW_SPHINCSSHAKE_128SSIMPLE_TREE_HEIGHT)) - 1u));
    tree     = tree >> FSMSW_SPHINCSSHAKE_128SSIMPLE_TREE_HEIGHT;
  }

  /* cleanup */
  FsmSw_SphincsShake_128sSimple_1FreeHashFunction(&ctx);

  /* Check if the root node equals the root node in the public key. */
  if (FsmSw_CommonLib_MemCmp(root, pub_root, FSMSW_SPHINCSSHAKE_128SSIMPLE_N) != 0u)
  {
    retVal = ERR_NOT_OK;
  }

  return retVal;
} // end: FsmSw_SphincsShake_128sSimple_Crypto_Sign_Verify

/*====================================================================================================================*/
/**
 * \brief Returns an array containing the signature followed by the message.
 *
 * \param[out] uint8        *sm : t.b.d.
 * \param[out] uint32     smlen : t.b.d.
 * \param[in]  const uint8   *m : t.b.d.
 * \param[in]  uint32      mlen : t.b.d.
 * \param[in]  const uint8  *sk : t.b.d.
 *
 */
/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsShake_128sSimple_Crypto_Sign(uint8 *const sm, uint32 *const smlen, const uint8 *const m, uint32 mlen,
                                               const uint8 *const sk)
{
  uint32 siglen = 0;

  (void)FsmSw_SphincsShake_128sSimple_Crypto_Sign_Signature(sm, &siglen, m, mlen, sk);

  FsmSw_CommonLib_MemMove(&sm[FSMSW_SPHINCSSHAKE_128SSIMPLE_BYTES], m, mlen);
  *smlen = siglen + mlen;

  return;
} // end: FsmSw_SphincsShake_128sSimple_Crypto_Sign

/*====================================================================================================================*/
/**
 * \brief Verifies a given signature-message pair under a given public key.
 *
 * \param[in] uint8  *m:     t.b.d.
 * \param[in] uint32 *mlen:  t.b.d.
 * \param[in] const uint8  *sm:    t.b.d.
 *              -       uint32  smlen: t.b.d.
 *              - const uint8  *pk:    t.b.d.
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
uint8 FsmSw_SphincsShake_128sSimple_Crypto_Sign_Open(uint8 *const m, uint32 *const mlen, const uint8 *const sm,
                                                     uint32 smlen, const uint8 *const pk)
{
  uint8 retVal = ERR_OK;

  /* The API caller does not necessarily know what size a signature should be but SPHINCS+ signatures are always
   * exactly FSMSW_SPHINCSSHAKE_128SSIMPLE_BYTES. */
  if (smlen < FSMSW_SPHINCSSHAKE_128SSIMPLE_BYTES)
  {
    FsmSw_CommonLib_MemSet(m, 0, smlen);
    *mlen  = 0;
    retVal = ERR_NOT_OK;
  }

  *mlen = smlen - FSMSW_SPHINCSSHAKE_128SSIMPLE_BYTES;

  if (0 != FsmSw_SphincsShake_128sSimple_Crypto_Sign_Verify(sm, FSMSW_SPHINCSSHAKE_128SSIMPLE_BYTES,
                                                            &sm[FSMSW_SPHINCSSHAKE_128SSIMPLE_BYTES], *mlen, pk))
  {
    FsmSw_CommonLib_MemSet(m, 0, smlen);
    *mlen  = 0;
    retVal = ERR_NOT_OK;
  }

  /* If verification was successful, move the message to the right place. */
  FsmSw_CommonLib_MemMove(m, &sm[FSMSW_SPHINCSSHAKE_128SSIMPLE_BYTES], *mlen);

  return retVal;
} // end: FsmSw_SphincsShake_128sSimple_Crypto_Sign_Open

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */