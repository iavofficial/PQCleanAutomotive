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
#include "FsmSw_Fips202.h"
#include "FsmSw_CommonLib.h"
#include "FsmSw_Dilithium2_packing.h"
#include "FsmSw_Dilithium2_params.h"
#include "FsmSw_Dilithium2_poly.h"
#include "FsmSw_Dilithium2_polyvec.h"
#include "FsmSw_Dilithium2_sign.h"
#include "FsmSw_Dilithium_symmetric.h"

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

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        FsmSw_Dilithium2_crypto_sign_keypair
*
* Description: Generates public and private key.
*
* Arguments:   - uint8 *pk: pointer to output public key
*                           (allocated array of FSMSW_DILITHIUM2_CRYPTO_PUBLICKEYBYTES bytes)
*              - uint8 *sk: pointer to output private key (allocated
*                           array of FSMSW_DILITHIUM2_CRYPTO_SECRETKEYBYTES bytes)
*
* Returns 0 (success)
***********************************************************************************************************************/
sint8 FsmSw_Dilithium2_crypto_sign_keypair(uint8 *pk, uint8 *sk)
{
    uint8 seedbuf[(2u * SEEDBYTES_DILITHIUM) + CRHBYTES_DILITHIUM] = {0};
    uint8 tr[TRBYTES_DILITHIUM] = {0};
    const uint8 *rho = (uint8*)NULL_PTR;
    const uint8 *rhoprime = (uint8*)NULL_PTR;
    const uint8 *key = (uint8*)NULL_PTR;
    polyvecl_D2 mat[K_DILITHIUM2] = {0};
    polyvecl_D2 s1    = {0};
    polyvecl_D2 s1hat = {0};
    polyveck_D2 s2    = {0};
    polyveck_D2 t1    = {0};
    polyveck_D2 t0    = {0};

    /* Get randomness for rho, rhoprime and key */
    (void)FsmSw_CommonLib_randombytes(seedbuf, SEEDBYTES_DILITHIUM);
    FsmSw_Fips202_shake256(seedbuf, (2u * SEEDBYTES_DILITHIUM) + CRHBYTES_DILITHIUM, seedbuf, SEEDBYTES_DILITHIUM);
    rho      = seedbuf;
    rhoprime = &rho[SEEDBYTES_DILITHIUM];
    key      = &rhoprime[CRHBYTES_DILITHIUM];

    /* Expand matrix */
    FsmSw_Dilithium2_polyvec_matrix_expand(mat, rho);

    /* Sample short vectors s1 and s2 */
    FsmSw_Dilithium2_polyvecl_uniform_eta(&s1, rhoprime, 0u);
    FsmSw_Dilithium2_polyveck_uniform_eta(&s2, rhoprime, L_DILITHIUM2);

    /* Matrix-vector multiplication */
    s1hat = s1;
    FsmSw_Dilithium2_polyvecl_ntt(&s1hat);
    FsmSw_Dilithium2_polyvec_matrix_pointwise_montgomery(&t1, mat, &s1hat);
    FsmSw_Dilithium2_polyveck_reduce(&t1);
    FsmSw_Dilithium2_polyveck_invntt_tomont(&t1);

    /* Add error vector s2 */
    FsmSw_Dilithium2_polyveck_add(&t1, &t1, &s2);

    /* Extract t1 and write public key */
    FsmSw_Dilithium2_polyveck_caddq(&t1);
    FsmSw_Dilithium2_polyveck_power2round(&t1, &t0, &t1);
    FsmSw_Dilithium2_pack_pk(pk, rho, &t1);

    /* Compute H(rho, t1) and write secret key */
    FsmSw_Fips202_shake256(tr, TRBYTES_DILITHIUM, pk, FSMSW_DILITHIUM2_CRYPTO_PUBLICKEYBYTES);
    FsmSw_Dilithium2_pack_sk(sk, rho, tr, key, &t0, &s1, &s2);

    return 0;
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium2_crypto_sign_signature
*
* Description: Computes signature.
*
* Arguments:   - uint8  *sig:    pointer to output signature (of length FSMSW_DILITHIUM2_CRYPTO_BYTES)
*              - uint32 *siglen: pointer to output length of signature
*              - uint8  *m:      pointer to message to be signed
*              - uint32  mlen:   length of message
*              - uint8  *sk:     pointer to bit-packed secret key
*
* Returns 0 (success)
***********************************************************************************************************************/
/* polyspace +2 MISRA2012:8.7 [Justified:]"This is an interface function 
designed for use by other systems that aim to integrate the Dilithium." */
sint8 FsmSw_Dilithium2_crypto_sign_signature(uint8 *sig, uint32 *siglen, const uint8 *m, uint32 mlen, const uint8 *sk)
{
    uint32 n = 0;
    uint8 seedbuf[(2u * SEEDBYTES_DILITHIUM) + TRBYTES_DILITHIUM + RNDBYTES_DILITHIUM + 
                  (2u * CRHBYTES_DILITHIUM)] = {0};
    uint8 *rho = (uint8*)NULL_PTR;
    uint8 *tr = (uint8*)NULL_PTR;
    uint8 *key = (uint8*)NULL_PTR;
    uint8 *mu = (uint8*)NULL_PTR;
    uint8 *rhoprime = (uint8*)NULL_PTR;
    uint8 *rnd = (uint8*)NULL_PTR;
    uint16 nonce = 0;
    polyvecl_D2 mat[K_DILITHIUM2] = {0};
    polyvecl_D2 s1 = {0};
    polyvecl_D2 y  = {0};
    polyvecl_D2 z  = {0};
    polyveck_D2 t0 = {0};
    polyveck_D2 s2 = {0};
    polyveck_D2 w1 = {0};
    polyveck_D2 w0 = {0};
    polyveck_D2 h  = {0};
    poly_D2 cp     = {0};
    shake256incctx state = {0};
    boolean loop = TRUE;

    rho = seedbuf;
    tr  = &rho[SEEDBYTES_DILITHIUM];
    key = &tr[TRBYTES_DILITHIUM];
    rnd = &key[SEEDBYTES_DILITHIUM];
    mu  = &rnd[RNDBYTES_DILITHIUM];
    rhoprime = &mu[CRHBYTES_DILITHIUM];
    FsmSw_Dilithium2_unpack_sk(rho, tr, key, &t0, &s1, &s2, sk);

    /* Compute mu = CRH(tr, msg) */
    FsmSw_Fips202_shake256_inc_init(&state);
    FsmSw_Fips202_shake256_inc_absorb(&state, tr, TRBYTES_DILITHIUM);
    FsmSw_Fips202_shake256_inc_absorb(&state, m, mlen);
    FsmSw_Fips202_shake256_inc_finalize(&state);
    FsmSw_Fips202_shake256_inc_squeeze(mu, CRHBYTES_DILITHIUM, &state);
    for (n = 0; n < RNDBYTES_DILITHIUM; n++)
    {
        rnd[n] = 0;
    }
    FsmSw_Fips202_shake256(rhoprime, CRHBYTES_DILITHIUM, key,
                           SEEDBYTES_DILITHIUM + RNDBYTES_DILITHIUM + CRHBYTES_DILITHIUM);

    /* Expand matrix and transform vectors */
    FsmSw_Dilithium2_polyvec_matrix_expand(mat, rho);
    FsmSw_Dilithium2_polyvecl_ntt(&s1);
    FsmSw_Dilithium2_polyveck_ntt(&s2);
    FsmSw_Dilithium2_polyveck_ntt(&t0);

    while(TRUE == loop)
    {
        /* Sample intermediate vector y */
        FsmSw_Dilithium2_polyvecl_uniform_gamma1(&y, rhoprime, nonce);
        nonce++;

        /* Matrix-vector multiplication */
        z = y;
        FsmSw_Dilithium2_polyvecl_ntt(&z);
        FsmSw_Dilithium2_polyvec_matrix_pointwise_montgomery(&w1, mat, &z);
        FsmSw_Dilithium2_polyveck_reduce(&w1);
        FsmSw_Dilithium2_polyveck_invntt_tomont(&w1);

        /* Decompose w and call the random oracle */
        FsmSw_Dilithium2_polyveck_caddq(&w1);
        FsmSw_Dilithium2_polyveck_decompose(&w1, &w0, &w1);
        FsmSw_Dilithium2_polyveck_pack_w1(sig, &w1);

        FsmSw_Fips202_shake256_inc_init(&state);
        FsmSw_Fips202_shake256_inc_absorb(&state, mu, CRHBYTES_DILITHIUM);
        FsmSw_Fips202_shake256_inc_absorb(&state, sig, K_DILITHIUM2 * POLYW1_PACKEDBYTES_DILITHIUM2);
        FsmSw_Fips202_shake256_inc_finalize(&state);
        FsmSw_Fips202_shake256_inc_squeeze(sig, CTILDEBYTES_DILITHIUM2, &state);
        FsmSw_Dilithium2_poly_challenge(&cp, sig); /* uses only the first SEEDBYTES bytes of sig */
        FsmSw_Dilithium2_poly_ntt(&cp);

        /* Compute z, reject if it reveals secret */
        FsmSw_Dilithium2_polyvecl_pointwise_poly_montgomery(&z, &cp, &s1);
        FsmSw_Dilithium2_polyvecl_invntt_tomont(&z);
        FsmSw_Dilithium2_polyvecl_add(&z, &z, &y);
        FsmSw_Dilithium2_polyvecl_reduce(&z);
        if (0 < FsmSw_Dilithium2_polyvecl_chknorm(&z, (sint32)(GAMMA1_DILITHIUM2 - BETA_DILITHIUM2)))
        {
            continue;
        }

        /* Check that subtracting cs2 does not change high bits of w and low bits
         * do not reveal secret information */
        FsmSw_Dilithium2_polyveck_pointwise_poly_montgomery(&h, &cp, &s2);
        FsmSw_Dilithium2_polyveck_invntt_tomont(&h);
        FsmSw_Dilithium2_polyveck_sub(&w0, &w0, &h);
        FsmSw_Dilithium2_polyveck_reduce(&w0);
        if (0 < FsmSw_Dilithium2_polyveck_chknorm(&w0, (sint32)((uint32)((uint32)GAMMA2_DILITHIUM2 - BETA_DILITHIUM2))))
        {
            continue;
        }

        /* Compute hints for w1 */
        FsmSw_Dilithium2_polyveck_pointwise_poly_montgomery(&h, &cp, &t0);
        FsmSw_Dilithium2_polyveck_invntt_tomont(&h);
        FsmSw_Dilithium2_polyveck_reduce(&h);
        if (0 < FsmSw_Dilithium2_polyveck_chknorm(&h, (sint32)GAMMA2_DILITHIUM2))
        {
            continue;
        }

        FsmSw_Dilithium2_polyveck_add(&w0, &w0, &h);
        n = FsmSw_Dilithium2_polyveck_make_hint(&h, &w0, &w1);
        if (n > OMEGA_DILITHIUM2)
        {
            continue;
        }

        loop = FALSE;
    }
    /* Write signature */
    FsmSw_Dilithium2_pack_sig(sig, sig, &z, &h);
    *siglen = FSMSW_DILITHIUM2_CRYPTO_BYTES;
    return 0;
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium2_crypto_sign
*
* Description: Compute signed message.
*
* Arguments:   -       uint8  *sm:    pointer to output signed message (allocated
*                                     array with FSMSW_DILITHIUM2_CRYPTO_BYTES + mlen bytes), can be equal to m
*              -       uint32 *smlen: pointer to output length of signed message
*              - const uint8  *m:     pointer to message to be signed
*              -       uint32  mlen:  length of message
*              - const uint8  *sk:    pointer to bit-packed secret key
*
* Returns 0 (success)
***********************************************************************************************************************/
sint8 FsmSw_Dilithium2_crypto_sign(uint8 *sm, uint32 *smlen, const uint8 *m, uint32 mlen, const uint8 *sk)
{
    uint32 i = 0;

    for (i = 0; i < mlen; ++i)
    {
        sm[FSMSW_DILITHIUM2_CRYPTO_BYTES + mlen - 1u - i] = m[mlen - 1u - i];
    }
    (void)FsmSw_Dilithium2_crypto_sign_signature(sm, smlen, &sm[FSMSW_DILITHIUM2_CRYPTO_BYTES], mlen, sk);
    *smlen += mlen;
    return 0;
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium2_crypto_sign_verify
*
* Description: Verifies signature.
*
* Arguments:   -       uint8  *m:      pointer to input signature
*              -       uint32  siglen: length of signature
*              - const uint8  *m:      pointer to message
*              -       uint32  mlen:   length of message
*              - const uint8  *pk:     pointer to bit-packed public key
*
* Returns 0 if signature could be verified correctly and -1 otherwise
***********************************************************************************************************************/
/* polyspace +2 MISRA2012:8.7 [Justified:]"This is an interface function 
designed for use by other systems that aim to integrate the Dilithium." */
sint8 FsmSw_Dilithium2_crypto_sign_verify(const uint8 *sig, uint32 siglen, const uint8 *m, uint32 mlen, const uint8 *pk)
{
    uint16 i = 0;
    uint8 buf[K_DILITHIUM2 * POLYW1_PACKEDBYTES_DILITHIUM2] = {0};
    uint8 rho[SEEDBYTES_DILITHIUM] = {0};
    uint8 mu[CRHBYTES_DILITHIUM] = {0};
    uint8 c[CTILDEBYTES_DILITHIUM2] = {0};
    uint8 c2[CTILDEBYTES_DILITHIUM2] = {0};
    poly_D2 cp = {0};
    polyvecl_D2 mat[K_DILITHIUM2] = {0}; 
    polyvecl_D2 z = {0};
    polyveck_D2 t1 = {0};
    polyveck_D2 w1 = {0};
    polyveck_D2 h = {0};
    shake256incctx state = {0};

    sint8 retVal = 0;

    if (siglen != FSMSW_DILITHIUM2_CRYPTO_BYTES)
    {
        retVal = -1;
    }

    FsmSw_Dilithium2_unpack_pk(rho, &t1, pk);
    if (0 < FsmSw_Dilithium2_unpack_sig(c, &z, &h, sig))
    {
        retVal = -1;
    }
    if (0 < FsmSw_Dilithium2_polyvecl_chknorm(&z, (sint32)(GAMMA1_DILITHIUM2 - BETA_DILITHIUM2)))
    {
        retVal = -1;
    }

    /* Compute CRH(H(rho, t1), msg) */
    FsmSw_Fips202_shake256(mu, CRHBYTES_DILITHIUM, pk, FSMSW_DILITHIUM2_CRYPTO_PUBLICKEYBYTES);
    FsmSw_Fips202_shake256_inc_init(&state);
    FsmSw_Fips202_shake256_inc_absorb(&state, mu, CRHBYTES_DILITHIUM);
    FsmSw_Fips202_shake256_inc_absorb(&state, m, mlen);
    FsmSw_Fips202_shake256_inc_finalize(&state);
    FsmSw_Fips202_shake256_inc_squeeze(mu, CRHBYTES_DILITHIUM, &state);

    /* Matrix-vector multiplication; compute Az - c2^dt1 */
    FsmSw_Dilithium2_poly_challenge(&cp, c); /* uses only the first SEEDBYTES bytes of c */
    FsmSw_Dilithium2_polyvec_matrix_expand(mat, rho);

    FsmSw_Dilithium2_polyvecl_ntt(&z);
    FsmSw_Dilithium2_polyvec_matrix_pointwise_montgomery(&w1, mat, &z);

    FsmSw_Dilithium2_poly_ntt(&cp);
    FsmSw_Dilithium2_polyveck_shiftl(&t1);
    FsmSw_Dilithium2_polyveck_ntt(&t1);
    FsmSw_Dilithium2_polyveck_pointwise_poly_montgomery(&t1, &cp, &t1);

    FsmSw_Dilithium2_polyveck_sub(&w1, &w1, &t1);
    FsmSw_Dilithium2_polyveck_reduce(&w1);
    FsmSw_Dilithium2_polyveck_invntt_tomont(&w1);

    /* Reconstruct w1 */
    FsmSw_Dilithium2_polyveck_caddq(&w1);
    FsmSw_Dilithium2_polyveck_use_hint(&w1, &w1, &h);
    FsmSw_Dilithium2_polyveck_pack_w1(buf, &w1);

    /* Call random oracle and verify FsmSw_Dilithium2_challenge */
    FsmSw_Fips202_shake256_inc_init(&state);
    FsmSw_Fips202_shake256_inc_absorb(&state, mu, CRHBYTES_DILITHIUM);
    FsmSw_Fips202_shake256_inc_absorb(&state, buf, K_DILITHIUM2 * POLYW1_PACKEDBYTES_DILITHIUM2);
    FsmSw_Fips202_shake256_inc_finalize(&state);
    FsmSw_Fips202_shake256_inc_squeeze(c2, CTILDEBYTES_DILITHIUM2, &state);
    for (i = 0; i < CTILDEBYTES_DILITHIUM2; ++i)
    {
        if (c[i] != c2[i])
        {
            retVal = -1;
        }
    }
    return retVal;
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium2_crypto_sign_open
*
* Description: Verify signed message.
*
* Arguments:   -       uint8  *m:     pointer to output message (allocated array with smlen bytes), can be equal to sm
*              -       uint32 *mlen:  pointer to output length of message
*              - const uint8  *sm:    pointer to signed message
*              -       uint32  smlen: length of signed message
*              - const uint8  *pk:    pointer to bit-packed public key
*
* Returns 0 if signed message could be verified correctly and -1 otherwise
***********************************************************************************************************************/
sint8 FsmSw_Dilithium2_crypto_sign_open(uint8 *m, uint32 *mlen, const uint8 *sm, uint32 smlen, const uint8 *pk)
{
    uint32 i = 0;
    sint8 retVal = -1;

    if (smlen >= FSMSW_DILITHIUM2_CRYPTO_BYTES)
    {
        *mlen = smlen - FSMSW_DILITHIUM2_CRYPTO_BYTES;
        if (0 == FsmSw_Dilithium2_crypto_sign_verify(sm, FSMSW_DILITHIUM2_CRYPTO_BYTES,
                                                     &sm[FSMSW_DILITHIUM2_CRYPTO_BYTES], *mlen, pk))
        {
            /* All good, copy msg, return 0 */
            for (i = 0; i < *mlen; ++i)
            {
                m[i] = sm[FSMSW_DILITHIUM2_CRYPTO_BYTES + i];
            }
            retVal = 0;
        }
    }

    if((sint8)0u != retVal)
    {
        /* Signature verification failed */
        *mlen = UINT32_MAXVAL;
        for (i = 0; i < smlen; ++i)
        {
            m[i] = 0;
        }
    }

    return retVal;
}
