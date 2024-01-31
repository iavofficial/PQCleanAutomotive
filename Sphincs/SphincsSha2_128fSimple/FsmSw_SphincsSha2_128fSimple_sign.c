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
 **********************************************************************************************************************/

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_CommonLib.h"
#include "FsmSw_Sphincs_sha2_address.h"
#include "FsmSw_SphincsSha2_128fSimple_FctWrapper.h"
#include "FsmSw_SphincsSha2_128fSimple_context.h"
#include "FsmSw_SphincsSha2_128fSimple_fors.h"
#include "FsmSw_SphincsSha2_128fSimple_hash.h"
#include "FsmSw_SphincsSha2_128fSimple_merkle.h"
#include "FsmSw_SphincsSha2_128fSimple_params.h"
#include "FsmSw_SphincsSha2_128fSimple_thash.h"
#include "FsmSw_SphincsSha2_128fSimple_utils.h"
#include "FsmSw_SphincsSha2_128fSimple_wots.h"
#include "FsmSw_SphincsSha2_128fSimple_sign.h"

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
int crypto_sign_seed_keypair(uint8 *pk, uint8 *sk, const uint8 *seed);
/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        crypto_sign_seed_keypair
*
* Description: Generates an SPX key pair given a seed of length
*              Format sk: [SK_SEED || SK_PRF || PUB_SEED || root]
*              Format pk: [PUB_SEED || root]
*
* Arguments:   -       uint8 *pk:   t.b.d.
*              -       uint8 *sk:   t.b.d.
*              - const uint8 *seed: t.b.d.
*
* Returns 0.
***********************************************************************************************************************/
int crypto_sign_seed_keypair(uint8 *pk, uint8 *sk, const uint8 *seed)
{
    sphincs_sha2_128f_ctx ctx;

    /* Initialize SK_SEED, SK_PRF and PUB_SEED from seed. */
    FsmSw_CommonLib_memcpy(sk, seed, FSMSW_SPHINCSSHA2_128FSIMPLE_CRYPTO_SEEDBYTES);

    FsmSw_CommonLib_memcpy(pk, &sk[2u * FSMSW_SPHINCSSHA2_128FSIMPLE_N], FSMSW_SPHINCSSHA2_128FSIMPLE_N);

    FsmSw_CommonLib_memcpy(ctx.pub_seed, pk, FSMSW_SPHINCSSHA2_128FSIMPLE_N);
    FsmSw_CommonLib_memcpy(ctx.sk_seed, sk, FSMSW_SPHINCSSHA2_128FSIMPLE_N);

    /* This hook allows the hash function instantiation to do whatever
       preparation or computation it needs, based on the public seed. */
    FsmSw_SphincsSha2_128fSimple_initialize_hash_function(&ctx);

    /* Compute root node of the top-most subtree. */
    FsmSw_SphincsSha2_128fSimple_merkle_gen_root(&sk[3u * FSMSW_SPHINCSSHA2_128FSIMPLE_N], &ctx);

    FsmSw_CommonLib_memcpy(&pk[FSMSW_SPHINCSSHA2_128FSIMPLE_N],
                           &sk[3u * FSMSW_SPHINCSSHA2_128FSIMPLE_N], FSMSW_SPHINCSSHA2_128FSIMPLE_N);

    return 0;
}
/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_128fSimple_crypto_sign_keypair
*
* Description: Generates an SPX key pair.
*              Format sk: [SK_SEED || SK_PRF || PUB_SEED || root]
*              Format pk: [PUB_SEED || root]
*
* Arguments:   -       uint8 *pk:   t.b.d.
*              -       uint8 *sk:   t.b.d.
*
* Returns 0.
***********************************************************************************************************************/
int FsmSw_SphincsSha2_128fSimple_crypto_sign_keypair(uint8 *pk, uint8 *sk)
{
    uint8 seed[FSMSW_SPHINCSSHA2_128FSIMPLE_CRYPTO_SEEDBYTES];
    (void)FsmSw_CommonLib_randombytes(seed, FSMSW_SPHINCSSHA2_128FSIMPLE_CRYPTO_SEEDBYTES);
    (void)crypto_sign_seed_keypair(pk, sk, seed);

    return 0;
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_128fSimple_crypto_sign_signature
*
* Description: Returns an array containing a detached signature.
*
* Arguments:   -       uint8  *sig:    t.b.d.
*              -       uint32 *siglen: t.b.d.
*              - const uint8  *m:      t.b.d.
*              -       uint32  mlen:   t.b.d.
*              - const uint8  *sk:     t.b.d.
*
* Returns 0.
***********************************************************************************************************************/
int FsmSw_SphincsSha2_128fSimple_crypto_sign_signature(uint8 *sig, uint32 *siglen, const uint8 *m, uint32 mlen,
                                                       const uint8 *sk)
{
    sphincs_sha2_128f_ctx ctx;

    const uint8 *sk_prf = &sk[FSMSW_SPHINCSSHA2_128FSIMPLE_N];
    const uint8 *pk     = &sk[2u * FSMSW_SPHINCSSHA2_128FSIMPLE_N];

    uint8 optrand[FSMSW_SPHINCSSHA2_128FSIMPLE_N];
    uint8 mhash[FSMSW_SPHINCSSHA2_128FSIMPLE_FORS_MSG_BYTES];
    uint8 root[FSMSW_SPHINCSSHA2_128FSIMPLE_N];
    uint32 i;
    uint64 tree;
    uint32 idx_leaf;
    uint32 wots_addr[8] = {0};
    uint32 tree_addr[8] = {0};

    FsmSw_CommonLib_memcpy(ctx.sk_seed, sk, FSMSW_SPHINCSSHA2_128FSIMPLE_N);
    FsmSw_CommonLib_memcpy(ctx.pub_seed, pk, FSMSW_SPHINCSSHA2_128FSIMPLE_N);

    /* This hook allows the hash function instantiation to do whatever
       preparation or computation it needs, based on the public seed. */
    FsmSw_SphincsSha2_128fSimple_initialize_hash_function(&ctx);

    FsmSw_SphincsSha2_set_type(wots_addr, FSMSW_SPHINCS_ADDR_TYPE_WOTS);
    FsmSw_SphincsSha2_set_type(tree_addr, FSMSW_SPHINCS_ADDR_TYPE_HASHTREE);

    /* Optionally, signing can be made non-deterministic using optrand.
       This can help counter side-channel attacks that would benefit from
       getting a large number of traces when the signer uses the same nodes. */
    (void)FsmSw_CommonLib_randombytes(optrand, FSMSW_SPHINCSSHA2_128FSIMPLE_N);
    /* Compute the digest randomization value. */
    FsmSw_SphincsSha2_128fSimple_gen_message_random(sig, sk_prf, optrand, m, mlen, &ctx);

    /* Derive the message digest and leaf index from R, PK and M. */
    FsmSw_SphincsSha2_128fSimple_hash_message(mhash, &tree, &idx_leaf, sig, pk, m, mlen, &ctx);
    sig = &sig[FSMSW_SPHINCSSHA2_128FSIMPLE_N];

    FsmSw_SphincsSha2_set_tree_addr(wots_addr, tree);
    FsmSw_SphincsSha2_128fSimple_set_keypair_addr(wots_addr, idx_leaf);

    /* Sign the message hash using FORS. */
    FsmSw_SphincsSha2_128fSimple_fors_sign(sig, root, mhash, &ctx, wots_addr);
    sig = &sig[FSMSW_SPHINCSSHA2_128FSIMPLE_FORS_BYTES];

    for (i = 0; i < FSMSW_SPHINCSSHA2_128FSIMPLE_D; i++)
    {
        FsmSw_SphincsSha2_set_layer_addr(tree_addr, i);
        FsmSw_SphincsSha2_set_tree_addr(tree_addr, tree);

        FsmSw_SphincsSha2_copy_subtree_addr(wots_addr, tree_addr);
        FsmSw_SphincsSha2_128fSimple_set_keypair_addr(wots_addr, idx_leaf);

        FsmSw_SphincsSha2_128fSimple_merkle_sign(sig, root, &ctx, wots_addr, tree_addr, idx_leaf);
        sig = &sig[FSMSW_SPHINCSSHA2_128FSIMPLE_WOTS_BYTES +
                   FSMSW_SPHINCSSHA2_128FSIMPLE_TREE_HEIGHT * FSMSW_SPHINCSSHA2_128FSIMPLE_N];

        /* Update the indices for the next layer. */
        idx_leaf = (uint32)(tree & ((1u << FSMSW_SPHINCSSHA2_128FSIMPLE_TREE_HEIGHT) - 1u));
        tree = tree >> FSMSW_SPHINCSSHA2_128FSIMPLE_TREE_HEIGHT;
    }

    *siglen = FSMSW_SPHINCSSHA2_128FSIMPLE_BYTES;

    return 0;
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_128fSimple_crypto_sign_verify
*
* Description: Verifies a detached signature and message under a given public key.
*
* Arguments:   -       uint8  *sig:    t.b.d.
*              -       uint32  siglen: t.b.d.
*              - const uint8  *m:      t.b.d.
*              -       uint32  mlen:   t.b.d.
*              - const uint8  *pk:     t.b.d.
*
* Returns 0.
***********************************************************************************************************************/
int FsmSw_SphincsSha2_128fSimple_crypto_sign_verify(const uint8 *sig, uint32 siglen, const uint8 *m, uint32 mlen,
                                                    const uint8 *pk)
{
    sphincs_sha2_128f_ctx ctx;
    const uint8 *pub_root = &pk[FSMSW_SPHINCSSHA2_128FSIMPLE_N];
    uint8 mhash[FSMSW_SPHINCSSHA2_128FSIMPLE_FORS_MSG_BYTES];
    uint8 wots_pk[FSMSW_SPHINCSSHA2_128FSIMPLE_WOTS_BYTES];
    uint8 root[FSMSW_SPHINCSSHA2_128FSIMPLE_N];
    uint8 leaf[FSMSW_SPHINCSSHA2_128FSIMPLE_N];
    uint32 i;
    uint64 tree;
    uint32 idx_leaf;
    uint32 wots_addr[8] = {0};
    uint32 tree_addr[8] = {0};
    uint32 wots_pk_addr[8] = {0};

    if (siglen != FSMSW_SPHINCSSHA2_128FSIMPLE_BYTES)
    {
        return -1;
    }

    FsmSw_CommonLib_memcpy(ctx.pub_seed, pk, FSMSW_SPHINCSSHA2_128FSIMPLE_N);

    /* This hook allows the hash function instantiation to do whatever
       preparation or computation it needs, based on the public seed. */
    FsmSw_SphincsSha2_128fSimple_initialize_hash_function(&ctx);

    FsmSw_SphincsSha2_set_type(wots_addr, FSMSW_SPHINCS_ADDR_TYPE_WOTS);
    FsmSw_SphincsSha2_set_type(tree_addr, FSMSW_SPHINCS_ADDR_TYPE_HASHTREE);
    FsmSw_SphincsSha2_set_type(wots_pk_addr, FSMSW_SPHINCS_ADDR_TYPE_WOTSPK);

    /* Derive the message digest and leaf index from R || PK || M. */
    /* The additional FSMSW_SPHINCSSHA2_128FSIMPLE_N is a result of the hash domain separator. */
    FsmSw_SphincsSha2_128fSimple_hash_message(mhash, &tree, &idx_leaf, sig, pk, m, mlen, &ctx);
    sig = &sig[FSMSW_SPHINCSSHA2_128FSIMPLE_N];

    /* Layer correctly defaults to 0, so no need to FsmSw_SphincsSha2_set_layer_addr */
    FsmSw_SphincsSha2_set_tree_addr(wots_addr, tree);
    FsmSw_SphincsSha2_128fSimple_set_keypair_addr(wots_addr, idx_leaf);

    FsmSw_SphincsSha2_128fSimple_fors_pk_from_sig(root, sig, mhash, &ctx, wots_addr);
    sig = &sig[FSMSW_SPHINCSSHA2_128FSIMPLE_FORS_BYTES];

    /* For each subtree.. */
    for (i = 0; i < FSMSW_SPHINCSSHA2_128FSIMPLE_D; i++)
    {
        FsmSw_SphincsSha2_set_layer_addr(tree_addr, i);
        FsmSw_SphincsSha2_set_tree_addr(tree_addr, tree);

        FsmSw_SphincsSha2_copy_subtree_addr(wots_addr, tree_addr);
        FsmSw_SphincsSha2_128fSimple_set_keypair_addr(wots_addr, idx_leaf);

        FsmSw_SphincsSha2_128fSimple_copy_keypair_addr(wots_pk_addr, wots_addr);

        /* The WOTS public key is only correct if the signature was correct. */
        /* Initially, root is the FORS pk, but on subsequent iterations it is
           the root of the subtree below the currently processed subtree. */
        FsmSw_SphincsSha2_128fSimple_wots_pk_from_sig(wots_pk, sig, root, &ctx, wots_addr);
        sig = &sig[FSMSW_SPHINCSSHA2_128FSIMPLE_WOTS_BYTES];

        /* Compute the leaf node using the WOTS public key. */
        FsmSw_SphincsSha2_128fSimple_thash(leaf, wots_pk, FSMSW_SPHINCSSHA2_128FSIMPLE_WOTS_LEN, &ctx, wots_pk_addr);

        /* Compute the root node of this subtree. */
        FsmSw_SphincsSha2_128fSimple_compute_root(root, leaf, idx_leaf, 0, sig,
                                                  FSMSW_SPHINCSSHA2_128FSIMPLE_TREE_HEIGHT, &ctx, tree_addr);
        sig = &sig[FSMSW_SPHINCSSHA2_128FSIMPLE_TREE_HEIGHT * FSMSW_SPHINCSSHA2_128FSIMPLE_N];

        /* Update the indices for the next layer. */
        idx_leaf = (uint32)(tree & ((1u << FSMSW_SPHINCSSHA2_128FSIMPLE_TREE_HEIGHT) - 1u));
        tree = tree >> FSMSW_SPHINCSSHA2_128FSIMPLE_TREE_HEIGHT;
    }

    /* Check if the root node equals the root node in the public key. */
    if (FsmSw_CommonLib_memcmp(root, pub_root, FSMSW_SPHINCSSHA2_128FSIMPLE_N) != 0u)
    {
        return -1;
    }

    return 0;
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_128fSimple_crypto_sign
*
* Description: Returns an array containing the signature followed by the message.
*
* Arguments:   -       uint8  *sm:    t.b.d.
*              -       uint32 *smlen: t.b.d.
*              - const uint8  *m:     t.b.d.
*              -       uint32  mlen:  t.b.d.
*              - const uint8  *sk:    t.b.d.
*
* Returns 0.
***********************************************************************************************************************/
int FsmSw_SphincsSha2_128fSimple_crypto_sign(uint8 *sm, uint32 *smlen, const uint8 *m, uint32 mlen, const uint8 *sk)
{
    uint32 siglen;

    (void)FsmSw_SphincsSha2_128fSimple_crypto_sign_signature(sm, &siglen, m, mlen, sk);

    FsmSw_CommonLib_memmove(&sm[FSMSW_SPHINCSSHA2_128FSIMPLE_BYTES], m, mlen);
    *smlen = siglen + mlen;

    return 0;
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_128fSimple_crypto_sign_open
*
* Description: Verifies a given signature-message pair under a given public key.
*
* Arguments:   -       uint8  *m:     t.b.d.
*              -       uint32 *mlen:  t.b.d.
*              - const uint8  *sm:    t.b.d.
*              -       uint32  smlen: t.b.d.
*              - const uint8  *pk:    t.b.d.
*
* Returns 0.
***********************************************************************************************************************/
int FsmSw_SphincsSha2_128fSimple_crypto_sign_open(uint8 *m, uint32 *mlen, const uint8 *sm, uint32 smlen,
                                                  const uint8 *pk)
{
    /* The API caller does not necessarily know what size a signature should be
       but SPHINCS+ signatures are always exactly FSMSW_SPHINCSSHA2_128FSIMPLE_BYTES. */
    if (smlen < FSMSW_SPHINCSSHA2_128FSIMPLE_BYTES)
    {
        FsmSw_CommonLib_memset(m, 0, smlen);
        *mlen = 0;
        return -1;
    }

    *mlen = smlen - FSMSW_SPHINCSSHA2_128FSIMPLE_BYTES;

    if (FsmSw_SphincsSha2_128fSimple_crypto_sign_verify(sm, FSMSW_SPHINCSSHA2_128FSIMPLE_BYTES,
                                                       &sm[FSMSW_SPHINCSSHA2_128FSIMPLE_BYTES], *mlen, pk) != 0)
    {
        FsmSw_CommonLib_memset(m, 0, smlen);
        *mlen = 0;
        return -1;
    }

    /* If verification was successful, move the message to the right place. */
    FsmSw_CommonLib_memmove(m, &sm[FSMSW_SPHINCSSHA2_128FSIMPLE_BYTES], *mlen);

    return 0;
}
