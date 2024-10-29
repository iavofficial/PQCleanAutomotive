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
***********************************************************************************************************************/
/* Wrapper for implementing the PQClean API. */
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_CommonLib.h"
#include "FsmSw_Falcon_common.h"
#include "FsmSw_Falcon_codec.h"
#include "FsmSw_Falcon_sign.h"
#include "FsmSw_Falcon_vrfy.h"
#include "FsmSw_Falcon_keygen.h"
#include "FsmSw_Falcon_fpr.h"

#include "FsmSw_Falcon1024_api.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define FSMSW_FALCON1024_NONCELEN   40u

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct
{
    uint8  b[FALCON_KEYGEN_TEMP_10];

} tmp1_struct;

typedef struct
{
    uint8 b[72 * 1024];
} tmp2_struct;

typedef struct
{
    uint8 b[2 * 1024];

} tmp3_struct;

typedef struct
{
    sint16 sig[1024];
    uint16 hm[1024];
} r_struct;
/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/
static tmp1_struct tmp1;
static tmp2_struct tmp2;
static tmp3_struct tmp3;
static r_struct r;

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/
static sint32 do_sign(uint8 *nonce, uint8 *sigbuf, uint32 *sigbuflen, const uint8 *m, uint32 mlen, const uint8 *sk);
static sint32 do_verify(const uint8 *nonce, const uint8 *sigbuf, uint32 sigbuflen, const uint8 *m, uint32 mlen,
                        const uint8 *pk);

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        do_sign
*
* Description: If a signature could be computed but not encoded because it would exceed the output buffer size, then a
*              new signature is computed. If the provided buffer size is too low, this could loop indefinitely, so the
*              caller must provide a size that can accommodate signatures with a large enough probability.
*
* Arguments:   -       uint8  *nonce:     receives the nonce and must have length FSMSW_FALCON1024_NONCELEN bytes
*              -       uint8  *sigbuf:    receives the signature value (without nonce or header byte)
*              -       uint32 *sigbuflen: providing the maximum value length and receiving the actual value length
*              - const uint8  *m:         t.b.d.
*              -       uint32  mlen:      t.b.d.
*              - const uint8  *sk:        t.b.d.
*
* Returns 0 on success, -1 on error.
*
***********************************************************************************************************************/
static sint32 do_sign(uint8 *nonce, uint8 *sigbuf, uint32 *sigbuflen, const uint8 *m, uint32 mlen, const uint8 *sk)
{
    sint8 f[1024], g[1024], F[1024], G[1024];
    uint8 seed[48];
    inner_shake256_context sc;
    uint32 u, v;
    boolean bStopFunc = FALSE;
    sint8 retVal = -1;

    /* Decode the private key. */
    if(sk[0] != (0x50u + 10u))
    {
        bStopFunc = TRUE;
    }

    u = 1;
    v = FsmSw_Falcon_trim_i8_decode(f, 10, FsmSw_Falcon_max_small_fg_bits[10], &sk[u],
                                    FSMSW_FALCON1024_CRYPTO_SECRETKEYBYTES - u);
    if(v == 0u)
    {
        bStopFunc = TRUE;
    }
    u += v;
    v = FsmSw_Falcon_trim_i8_decode(g, 10, FsmSw_Falcon_max_small_fg_bits[10], &sk[u],
                                    FSMSW_FALCON1024_CRYPTO_SECRETKEYBYTES - u);
    if(v == 0u)
    {
        bStopFunc = TRUE;
    }
    u += v;
    v = FsmSw_Falcon_trim_i8_decode(F, 10, FsmSw_Falcon_max_big_FG_bits[10], &sk[u],
                                    FSMSW_FALCON1024_CRYPTO_SECRETKEYBYTES - u);
    if(v == 0u)
    {
        bStopFunc = TRUE;
    }
    u += v;
    if(u != FSMSW_FALCON1024_CRYPTO_SECRETKEYBYTES)
    {
        bStopFunc = TRUE;
    }
    if(0 == FsmSw_Falcon_complete_private(G, f, g, F, 10, tmp2.b))
    {
        bStopFunc = TRUE;
    }

    if (bStopFunc == FALSE)
    {
        /* Create a random nonce (40 bytes). */
        (void)FsmSw_CommonLib_randombytes(nonce, FSMSW_FALCON1024_NONCELEN);

        /* Hash message nonce + message into a vector. */
        FsmSw_Fips202_shake256_inc_init(&sc);
        FsmSw_Fips202_shake256_inc_absorb(&sc, nonce, FSMSW_FALCON1024_NONCELEN);
        FsmSw_Fips202_shake256_inc_absorb(&sc, m, mlen);
        FsmSw_Fips202_shake256_inc_finalize(&sc);
        FsmSw_Falcon_hash_to_point_ct(&sc, r.hm, 10, tmp2.b);

        /* Initialize a RNG. */
        (void)FsmSw_CommonLib_randombytes(seed, sizeof(seed));
        FsmSw_Fips202_shake256_inc_init(&sc);
        FsmSw_Fips202_shake256_inc_absorb(&sc, seed, sizeof(seed));
        FsmSw_Fips202_shake256_inc_finalize(&sc);

        /* Compute and return the signature. This loops until a signature value is found that fits in the provided
        * buffer. */
        for (uint32 i = 0; i < 0xFFFFFFFFu; i++)
        {
            FsmSw_Falcon_sign_dyn(r.sig, &sc, f, g, F, G, r.hm, 10u, tmp2.b);
            v = FsmSw_Falcon_comp_encode(sigbuf, *sigbuflen, r.sig, 10u);
            if(v != 0u)
            {
                *sigbuflen = v;
                retVal = 0;
                break;
            }
        }
    }

    return retVal;

}

/***********************************************************************************************************************
* Name:        do_verify
*
* Description: Verify a signature.
*
* Arguments:   -       uint8  *nonce:     The nonce has size FSMSW_FALCON1024_NONCELEN bytes
*              -       uint8  *sigbuf:    contains the signature value
*              -       uint32  sigbuflen: providing the maximum value length
*              - const uint8  *m:         t.b.d.
*              -       uint32  mlen:      t.b.d.
*              - const uint8  *pk:        t.b.d.
*
* Returns 0 on success, -1 on error.
*
***********************************************************************************************************************/
static sint32 do_verify(const uint8 *nonce, const uint8 *sigbuf, uint32 sigbuflen, const uint8 *m, uint32 mlen,
                        const uint8 *pk)
{
    uint16 h[1024], hm[1024];
    sint16 sig[1024];
    inner_shake256_context sc;
    boolean bStopFunc = FALSE;
    sint8 retVal = 0;

    /* Decode public key. */
    if(pk[0] != (10u))
    {
        retVal = -1;
        bStopFunc = TRUE;
    }
    if(FsmSw_Falcon_modq_decode(h, 10, &pk[1], FSMSW_FALCON1024_CRYPTO_PUBLICKEYBYTES - 1u) !=
        FSMSW_FALCON1024_CRYPTO_PUBLICKEYBYTES - 1u)
    {
        retVal = -1;
        bStopFunc = TRUE;
    }

    if (FALSE == bStopFunc)
    {
        FsmSw_Falcon_to_ntt_monty(h, 10u);

        /* Decode signature. */
        if(sigbuflen == 0u)
        {
            retVal = -1;
            bStopFunc = TRUE;
        }
        if(FsmSw_Falcon_comp_decode(sig, 10, sigbuf, sigbuflen) != sigbuflen)
        {
            retVal = -1;
            bStopFunc = TRUE;
        }

        if (FALSE == bStopFunc)
        {
		    /* Hash nonce + message into a vector. */
		    FsmSw_Fips202_shake256_inc_init(&sc);
		    FsmSw_Fips202_shake256_inc_absorb(&sc, nonce, FSMSW_FALCON1024_NONCELEN);
		    FsmSw_Fips202_shake256_inc_absorb(&sc, m, mlen);
		    FsmSw_Fips202_shake256_inc_finalize(&sc);
		    FsmSw_Falcon_hash_to_point_ct(&sc, hm, 10u, tmp3.b);

            /* Verify signature. */
            if(0 == FsmSw_Falcon_verify_raw(hm, sig, h, 10u, tmp3.b))
            {
                retVal = -1;
            }
        }
    }
    return retVal;
}
/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/*
 * Encoding formats (nnnn = log of degree, 9 for Falcon-512, 10 for Falcon-1024)
 *
 *   private key:
 *      header byte: 0101nnnn
 *      private f  (6 or 5 bits by element, depending on degree)
 *      private g  (6 or 5 bits by element, depending on degree)
 *      private F  (8 bits by element)
 *
 *   public key:
 *      header byte: 0000nnnn
 *      public h   (14 bits by element)
 *
 *   signature:
 *      header byte: 0011nnnn
 *      nonce     40 bytes
 *      value     (12 bits by element)
 *
 *   message + signature:
 *      signature length   (2 bytes, big-endian)
 *      nonce              40 bytes
 *      message
 *      header byte:       0010nnnn
 *      value              (12 bits by element)
 *      (signature length is 1+len(value), not counting the nonce)
 */

/***********************************************************************************************************************
* Name:        FsmSw_Falcon1024_crypto_sign_keypair
*
* Description: Computes a root node given a leaf and an auth path. Expects address to be complete other than the
*              tree_height and tree_index.
*
* Arguments:   - uint8 *pk: public key with length FSMSW_FALCON1024_CRYPTO_PUBLICKEYBYTES
*              - uint8 *sk: private key with length FSMSW_FALCON1024_CRYPTO_PUBLICKEYBYTES
*
* Returns 0 on success, -1 on error.
*
***********************************************************************************************************************/
sint32 FsmSw_Falcon1024_crypto_sign_keypair(uint8 *pk, uint8 *sk)
{
    sint8 f[1024], g[1024], F[1024];
    uint16 h[1024];
    uint8 seed[48];
    inner_shake256_context rng;
    uint32 u, v;
    sint8 retVal = 0;
    boolean bStopFunc = FALSE;

    /* Generate key pair. */
    (void)FsmSw_CommonLib_randombytes(seed, sizeof(seed));
    FsmSw_Fips202_shake256_inc_init(&rng);
    FsmSw_Fips202_shake256_inc_absorb(&rng, seed, sizeof(seed));
    FsmSw_Fips202_shake256_inc_finalize(&rng);
    FsmSw_Falcon_keygen(&rng, f, g, F, ((void *)0), h, 10, tmp1.b);

    /* Encode private key. */
    sk[0] = 0x50u + 10u;
    u = 1u;
    v = FsmSw_Falcon_trim_i8_encode(&sk[u], FSMSW_FALCON1024_CRYPTO_SECRETKEYBYTES - u, f, 10u,
                                    FsmSw_Falcon_max_small_fg_bits[10]);
    if(v == 0u)
    {
        retVal = -1;
        bStopFunc = TRUE;
    }
    u += v;
    v = FsmSw_Falcon_trim_i8_encode(&sk[u], FSMSW_FALCON1024_CRYPTO_SECRETKEYBYTES - u, g, 10u,
                                    FsmSw_Falcon_max_small_fg_bits[10]);
    if(v == 0u)
    {
        retVal = -1;
        bStopFunc = TRUE;
    }
    u += v;
    v = FsmSw_Falcon_trim_i8_encode(&sk[u], FSMSW_FALCON1024_CRYPTO_SECRETKEYBYTES - u, F, 10u,
                                    (uint32)FsmSw_Falcon_max_big_FG_bits[10]);
    if(v == 0u)
    {
        retVal = -1;
        bStopFunc = TRUE;
    }
    u += v;
    if(u != FSMSW_FALCON1024_CRYPTO_SECRETKEYBYTES)
    {
        retVal = -1;
        bStopFunc = TRUE;
    }

    if (FALSE == bStopFunc)
    {
        /* Encode public key. */
        pk[0] = 10u;
        v = FsmSw_Falcon_modq_encode(&pk[1], FSMSW_FALCON1024_CRYPTO_PUBLICKEYBYTES - 1u, h, 10u);
        if(v != FSMSW_FALCON1024_CRYPTO_PUBLICKEYBYTES - 1u)
        {
            retVal = -1;
        }
    }

    return retVal;
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon1024_crypto_sign_signature
*
* Description: Compute a signature on a provided message (m, mlen), with a given private key (sk).
*              sig[], m[] and pk[] may overlap each other arbitrarily.
*
* Arguments:   -       uint8  *sig:    Signature is written in sig
*              -       uint32  siglen: maximum signature length (in bytes) is FSMSW_FALCON1024_CRYPTO_BYTES
*              - const uint8  *m:      t.b.d.
*              -       uint32  mlen:   t.b.d.
*              - const uint8  *sk:     t.b.d.
*
* Returns 0 on success, -1 on error.
*
***********************************************************************************************************************/
sint32 FsmSw_Falcon1024_crypto_sign_signature(uint8 *sig, uint32 *siglen, const uint8 *m, uint32 mlen, const uint8 *sk)
{
    uint32 vlen;
    sint8 retVal = 0;

    vlen = FSMSW_FALCON1024_CRYPTO_BYTES - FSMSW_FALCON1024_NONCELEN - 3u;

    if(do_sign(&sig[1], &sig[1u + FSMSW_FALCON1024_NONCELEN], &vlen, m, mlen, sk) < 0)
    {
        retVal = -1;
    }

    sig[0] = 0x30 + 10;
    *siglen = 1u + FSMSW_FALCON1024_NONCELEN + vlen;

    return retVal;
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon1024_crypto_sign_verify
*
* Description: Verify a signature (sig, siglen) on a message (m, mlen) with a given public key (pk).
*              sig[], m[] and pk[] may overlap each other arbitrarily.
*
* Arguments:   - const uint8  *sig:    t.b.d.
*              -       uint32  siglen: t.b.d.
*              - const uint8  *m:      t.b.d.
*              -       uint32  mlen:   t.b.d.
*              - const uint8  *pk:     t.b.d.
*
* Returns 0 on success, -1 on error.
*
***********************************************************************************************************************/
sint32 FsmSw_Falcon1024_crypto_sign_verify(const uint8 *sig, uint32 siglen, const uint8 *m, uint32 mlen,
                                           const uint8 *pk)
{
    sint32 retVal = do_verify(&sig[1], &sig[1u + FSMSW_FALCON1024_NONCELEN], siglen - 1u - FSMSW_FALCON1024_NONCELEN,
                     m, mlen, pk);

    if(siglen < (1u + FSMSW_FALCON1024_NONCELEN))
    {
         retVal = -1;
    }

    if(sig[0] != (0x30u + 10u))
    {
        retVal = -1;
    }

    return retVal;
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon1024_crypto_sign
*
* Description: Compute a signature on a message and pack the signature and message into a single object, written
*              into sm[]. The length of that output is written in *smlen; that length may be larger than the message
*              length (mlen) by up to FSMSW_FALCON1024_CRYPTO_BYTES.
*              sm[] and m[] may overlap each other arbitrarily; however, sm[] shall not overlap with sk[].
*
* Arguments:   -       uint8  *sm:    t.b.d.
*              -       uint32  smlen: t.b.d.
*              - const uint8  *m:     t.b.d.
*              -       uint32  mlen:  t.b.d.
*              - const uint8  *sk:    t.b.d.
*
* Returns 0 on success, -1 on error.
*
***********************************************************************************************************************/
sint32 FsmSw_Falcon1024_crypto_sign(uint8 *sm, uint32 *smlen, const uint8 *m, uint32 mlen, const uint8 *sk)
{
    uint8 *pm, *sigbuf;
    uint32 sigbuflen;
    sint8 retVal = 0;

    /* Move the message to its final location; this is a memmove() so it handles overlaps properly. */
    FsmSw_CommonLib_memmove(&sm[2u + FSMSW_FALCON1024_NONCELEN], m, mlen);
    pm = &sm[2u + FSMSW_FALCON1024_NONCELEN];
    sigbuf = &pm[1u + mlen];
    sigbuflen = FSMSW_FALCON1024_CRYPTO_BYTES - FSMSW_FALCON1024_NONCELEN - 3u;

    if(do_sign(&sm[2], sigbuf, &sigbuflen, pm, mlen, sk) < 0)
    {
        retVal = -1;
    }
    pm[mlen] = 0x20 + 10;
    sigbuflen++;
    sm[0] = (uint8) (sigbuflen >> 8);
    sm[1] = (uint8) sigbuflen;
    *smlen = mlen + 2u + FSMSW_FALCON1024_NONCELEN + sigbuflen;
    return retVal;
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon1024_crypto_sign_open
*
* Description: Open a signed message object (sm, smlen) and verify the signature; on success, the message itself is
*              written into m[] and its length into *mlen. The message is shorter than the signed message object, but
*              the size difference depends on the signature value; the difference may range up to
*              FSMSW_FALCON1024_CRYPTO_BYTES.
*              m[], sm[] and pk[] may overlap each other arbitrarily.
*
* Arguments:   -       uint8  *m:     t.b.d.
*              -       uint32  mlen:  t.b.d.
*              - const uint8  *sm:    t.b.d.
*              -       uint32  smlen: t.b.d.
*              - const uint8  *sk:    t.b.d.
*
* Returns 0 on success, -1 on error.
*
***********************************************************************************************************************/
sint32 FsmSw_Falcon1024_crypto_sign_open(uint8 *m, uint32 *mlen, const uint8 *sm, uint32 smlen, const uint8 *pk)
{
    const uint8 *sigbuf;
    uint32 pmlen, sigbuflen;
    sint8 retVal = 0;

    if(smlen < (3u + FSMSW_FALCON1024_NONCELEN))
    {
        retVal = -1;
    }

    sigbuflen = ((uint32) sm[0] << 8) | (uint32) sm[1];

    if((sigbuflen < 2u) || (sigbuflen > (smlen - FSMSW_FALCON1024_NONCELEN - 2u)))
    {
        retVal = -1;
    }

    sigbuflen--;
    pmlen = smlen - FSMSW_FALCON1024_NONCELEN - 3u - sigbuflen;

    if(sm[2u + FSMSW_FALCON1024_NONCELEN + pmlen] != (0x20u + 10u))
    {
        retVal = -1;
    }

    sigbuf = &sm[2u + FSMSW_FALCON1024_NONCELEN + pmlen + 1u];

    /* The 2-byte length header and the one-byte signature header have been verified. Nonce is at sm+2, followed by the
     * message itself. Message length is in pmlen. sigbuf/sigbuflen point to the signature value (excluding the header
     * byte). */
    if(do_verify(&sm[2], sigbuf, sigbuflen, &sm[2u + FSMSW_FALCON1024_NONCELEN], pmlen, pk) < 0)
    {
        retVal = -1;
    }

    /* Signature is correct, we just have to copy/move the message to its final destination.
     * The FsmSw_CommonLib_memmove() properly handles overlaps. */
    FsmSw_CommonLib_memmove(m, &sm[2u + FSMSW_FALCON1024_NONCELEN], pmlen);
    *mlen = pmlen;

    return retVal;
}
