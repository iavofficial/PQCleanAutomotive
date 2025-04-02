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
#include "FsmSw_Dilithium_ntt.h"
#include "FsmSw_Dilithium5_params.h"
#include "FsmSw_Dilithium5_poly.h"

#include "FsmSw_Types.h"
#include "FsmSw_Dilithium_reduce.h"
#include "FsmSw_Dilithium5_rounding.h"
#include "FsmSw_Dilithium_symmetric.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define POLY_UNIFORM_NBLOCKS        ((768u + STREAM128_BLOCKBYTES - 1u)/STREAM128_BLOCKBYTES)
#define POLY_UNIFORM_ETA_NBLOCKS    ((136u + STREAM256_BLOCKBYTES - 1u)/STREAM256_BLOCKBYTES)
#define POLY_UNIFORM_GAMMA1_NBLOCKS ((POLYZ_PACKEDBYTES_DILITHIUM5 + STREAM256_BLOCKBYTES - 1u)/STREAM256_BLOCKBYTES)

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
static uint32 fsmsw_dilithium5_RejEta(sint32 *a, uint32 len, const uint8 *buf, uint32 buflen);
static uint32 fsmsw_dilithium5_RejUniform(sint32 *a, uint32 len, const uint8 *buf, uint32 buflen);

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        fsmsw_dilithium5_RejEta
*
* Description: Sample uniformly random coefficients in [-ETA, ETA] by
*              performing rejection sampling on array of random bytes.
*
* Arguments:   -       sint32 *a:      pointer to output array (allocated)
*              -       uint32  len:    number of coefficients to be sampled
*              - const uint8  *buf:    array of random bytes
*              -       uint32  buflen: length of array of random bytes
*
* Returns number of sampled coefficients. Can be smaller than len if not enough random bytes were given.
***********************************************************************************************************************/
static uint32 fsmsw_dilithium5_RejEta(sint32 *a, uint32 len, const uint8 *buf, uint32 buflen)
{
    uint32 ctr = 0;
    uint32 pos = 0;
    uint32 t0 = 0;
    uint32 t1 = 0;

    ctr = 0;
    pos = 0;
    while ((ctr < len) && (pos < buflen))
    {
        t0 = (uint32)(buf[pos]) & 0x0Fu;
        t1 = (uint32)(buf[pos]) >> 4;
        pos++;

        if (t0 < 15u)
        {
            t0 = t0 - (uint32)(((205u * t0) >> 10) * 5u);
            a[ctr] = (sint32)((uint32)(2u - t0));
            ctr++;
        }

        if ((t1 < 15u) && (ctr < len))
        {
            t1 = t1 - (((205u * t1) >> 10) * 5u);
            a[ctr] = (sint32)((uint32)(2u - t1));
            ctr++;
        }
    }

    return ctr;
}

/***********************************************************************************************************************
* Name:        fsmsw_dilithium5_RejUniform
*
* Description: Sample uniformly random coefficients in [0, Q-1] by
*              performing rejection sampling on array of random bytes.
*
* Arguments:   -       sint32 *a:      pointer to output array (allocated)
*              -       uint32  len:    number of coefficients to be sampled
*              - const uint8  *buf:    array of random bytes
*              -       uint32  buflen: length of array of random bytes
*
* Returns number of sampled coefficients. Can be smaller than len if not enough
* random bytes were given.
***********************************************************************************************************************/
static uint32 fsmsw_dilithium5_RejUniform(sint32 *a, uint32 len, const uint8 *buf, uint32 buflen)
{
    uint32 ctr = 0;
    uint32 pos = 0;
    uint32 t = 0;

    ctr = 0;
    pos = 0;
    while ((ctr < len) && ((pos + 3u) <= buflen))
    {
        t  = buf[pos];
        pos++;
        t |= (uint32)buf[pos] << 8;
        pos++;
        t |= (uint32)buf[pos] << 16;
        pos++;
        t &= 0x7FFFFFu;

        if (t < (uint32)Q_DILITHIUM)
        {
            a[ctr] = (sint32)t;
            ctr++;
        }
    }

    return ctr;
}


/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_poly_reduce
*
* Description: Inplace reduction of all coefficients of polynomial to representative in [-6283009,6283007].
*
* Arguments:   - poly_D5 *a: pointer to input/output polynomial
***********************************************************************************************************************/
void FsmSw_Dilithium5_poly_reduce(poly_D5 *a)
{
    uint16 i = 0;

    for (i = 0; i < N_DILITHIUM; ++i)
    {
        a->coeffs[i] = FsmSw_Dilithium_reduce32(a->coeffs[i]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_poly_caddq
*
* Description: For all coefficients of in/out polynomial add Q if coefficient is negative.
*
* Arguments:   - poly_D5 *a: pointer to input/output polynomial
***********************************************************************************************************************/
void FsmSw_Dilithium5_poly_caddq(poly_D5 *a)
{
    uint16 i;

    for (i = 0; i < N_DILITHIUM; ++i)
    {
        a->coeffs[i] = FsmSw_Dilithium_caddq(a->coeffs[i]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_poly_add
*
* Description: Add polynomials. No modular reduction is performed.
*
* Arguments:   -       poly_D5 *c: pointer to output polynomial
*              - const poly_D5 *a: pointer to first summand
*              - const poly_D5 *b: pointer to second summand
***********************************************************************************************************************/
void FsmSw_Dilithium5_poly_add(poly_D5 *c, const poly_D5 *a, const poly_D5 *b)
{
    uint16 i = 0;

    for (i = 0; i < N_DILITHIUM; ++i)
    {
        c->coeffs[i] = a->coeffs[i] + b->coeffs[i];
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_poly_sub
*
* Description: Subtract polynomials. No modular reduction is performed.
*
* Arguments:   -       poly_D5 *c: pointer to output polynomial
*              - const poly_D5 *a: pointer to first input polynomial
*              - const poly_D5 *b: pointer to second input polynomial to be subtraced from first input polynomial
***********************************************************************************************************************/
void FsmSw_Dilithium5_poly_sub(poly_D5 *c, const poly_D5 *a, const poly_D5 *b)
{
    uint16 i = 0;

    for (i = 0; i < N_DILITHIUM; ++i)
    {
        c->coeffs[i] = a->coeffs[i] - b->coeffs[i];
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_poly_shiftl
*
* Description: Multiply polynomial by 2^D without modular reduction.
*              Assumes input coefficients to be less than 2^{31-D} in absolute value.
*
* Arguments:   - poly_D5 *a: pointer to input/output polynomial
***********************************************************************************************************************/
void FsmSw_Dilithium5_poly_shiftl(poly_D5 *a)
{
    uint16 i = 0;

    for (i = 0; i < N_DILITHIUM; ++i)
    {
        a->coeffs[i] = (sint32)((uint32)((uint32)(a->coeffs[i]) << D_DILITHIUM));
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_poly_ntt
*
* Description: Inplace forward NTT. Coefficients can grow by 8*Q in absolute value.
*
* Arguments:   - poly_D5 *a: pointer to input/output polynomial
***********************************************************************************************************************/
void FsmSw_Dilithium5_poly_ntt(poly_D5 *a)
{
    FsmSw_Dilithium_ntt(a->coeffs);
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_poly_invntt_tomont
*
* Description: Inplace inverse NTT and multiplication by 2^{32}. Input coefficients need to be less than Q in absolute
*              value and output coefficients are again bounded by Q.
*
* Arguments:   - poly_D5 *a: pointer to input/output polynomial
***********************************************************************************************************************/
 void FsmSw_Dilithium5_poly_invntt_tomont(poly_D5 *a)
 {
    FsmSw_Dilithium_invntt_tomont(a->coeffs);
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_poly_pointwise_montgomery
*
* Description: Pointwise multiplication of polynomials in NTT domain representation and multiplication of
*              resulting polynomial by 2^{-32}.
*
* Arguments:   -       poly_D5 *c: pointer to output polynomial
*              - const poly_D5 *a: pointer to first input polynomial
*              - const poly_D5 *b: pointer to second input polynomial
***********************************************************************************************************************/
void FsmSw_Dilithium5_poly_pointwise_montgomery(poly_D5 *c, const poly_D5 *a, const poly_D5 *b)
{
    uint16 i = 0;

    for (i = 0; i < N_DILITHIUM; ++i)
    {
        c->coeffs[i] = FsmSw_Dilithium_montgomery_reduce((sint64)a->coeffs[i] * b->coeffs[i]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_poly_power2round
*
* Description: For all coefficients c of the input polynomial, compute c0, c1 such that c mod Q = c1*2^D + c0
*              with -2^{D-1} < c0 <= 2^{D-1}. Assumes coefficients to be standard representatives.
*
* Arguments:   -       poly_D5 *a1: pointer to output polynomial with coefficients c1
*              -       poly_D5 *a0: pointer to output polynomial with coefficients c0
*              - const poly_D5 *a:  pointer to input polynomial
***********************************************************************************************************************/
void FsmSw_Dilithium5_poly_power2round(poly_D5 *a1, poly_D5 *a0, const poly_D5 *a)
{
    uint16 i = 0;

    for (i = 0; i < N_DILITHIUM; ++i)
    {
        a1->coeffs[i] = FsmSw_Dilithium5_power2round(&a0->coeffs[i], a->coeffs[i]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_poly_decompose
*
* Description: For all coefficients c of the input polynomial, compute high and low bits c0, c1 such c mod
*              Q = c1*ALPHA + c0 with -ALPHA/2 < c0 <= ALPHA/2 except c1 = (Q-1)/ALPHA where we set c1 = 0 and
*              -ALPHA/2 <= c0 = c mod Q - Q < 0. Assumes coefficients to be standard representatives.
*
* Arguments:   -       poly_D5 *a1: pointer to output polynomial with coefficients c1
*              -       poly_D5 *a0: pointer to output polynomial with coefficients c0
*              - const poly_D5 *a:  pointer to input polynomial
***********************************************************************************************************************/
void FsmSw_Dilithium5_poly_decompose(poly_D5 *a1, poly_D5 *a0, const poly_D5 *a)
{
    uint16 i = 0;

    for (i = 0; i < N_DILITHIUM; ++i)
    {
        a1->coeffs[i] = FsmSw_Dilithium5_decompose(&a0->coeffs[i], a->coeffs[i]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_poly_make_hint
*
* Description: Compute hint polynomial. The coefficients of which indicate whether the low bits of the corresponding
*              coefficient of the input polynomial overflow into the high bits.
*
* Arguments:   -       poly_D5 *h:  pointer to output hint polynomial
*              - const poly_D5 *a0: pointer to low part of input polynomial
*              - const poly_D5 *a1: pointer to high part of input polynomial
*
* Returns number of 1 bits.
***********************************************************************************************************************/
uint32 FsmSw_Dilithium5_poly_make_hint(poly_D5 *h, const poly_D5 *a0, const poly_D5 *a1)
{
    uint16 i = 0;
    uint32 s = 0;

    for (i = 0; i < N_DILITHIUM; ++i)
    {
        h->coeffs[i] = (sint32)FsmSw_Dilithium5_make_hint(a0->coeffs[i], a1->coeffs[i]);
        s = s + (uint32)(h->coeffs[i]);
    }

    return s;
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_poly_use_hint
*
* Description: Use hint polynomial to correct the high bits of a polynomial.
*
* Arguments:   -       poly_D5 *b: pointer to output polynomial with corrected high bits
*              - const poly_D5 *a: pointer to input polynomial
*              - const poly_D5 *h: pointer to input hint polynomial
***********************************************************************************************************************/
void FsmSw_Dilithium5_poly_use_hint(poly_D5 *b, const poly_D5 *a, const poly_D5 *h)
{
    uint16 i = 0;

    for (i = 0; i < N_DILITHIUM; ++i)
    {
        b->coeffs[i] = FsmSw_Dilithium5_use_hint(a->coeffs[i], (uint32)h->coeffs[i]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_poly_chknorm
*
* Description: Check infinity norm of polynomial against given bound.
*              Assumes input coefficients were reduced by FsmSw_Dilithium5_reduce32().
*
* Arguments:   - const poly_D5 *a: pointer to polynomial
*              - sint32 B:         norm bound
*
* Returns 0 if norm is strictly smaller than B <= (Q-1)/8 and 1 otherwise.
***********************************************************************************************************************/
sint8 FsmSw_Dilithium5_poly_chknorm(const poly_D5 *a, sint32 B)
{
    uint16 i = 0;
    sint32 t = 0;
    sint8 retVal = 0;

    if (B > (Q_DILITHIUM - 1) / 8)
    {
        retVal = 1;
    }

    /* It is ok to leak which coefficient violates the bound since
       the probability for each coefficient is independent of secret
       data but we must not leak the sign of the centralized representative. */
    for (i = 0; i < N_DILITHIUM; ++i)
    {
        /* Absolute value */
        t = (sint32)((uint32)((uint64)a->coeffs[i] >> 31));
        t = (sint32)((uint32)((uint32)a->coeffs[i] - ((uint32)t & (2u * (uint32)a->coeffs[i]))));

        if (t >= B)
        {
            retVal = 1;
        }
    }

    return retVal;
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_poly_uniform
*
* Description: Sample polynomial with uniformly random coefficients in [0,Q-1] by performing rejection sampling on the
*              output stream of SHAKE256(seed|nonce)
*
* Arguments:   -       poly_D5 *a:      pointer to output polynomial
*              - const uint8    seed[]: byte array with seed of length SEEDBYTES
*              -       uint16   nonce:  2-byte nonce
***********************************************************************************************************************/
void FsmSw_Dilithium5_poly_uniform(poly_D5 *a, const uint8 seed[SEEDBYTES_DILITHIUM], uint16 nonce)
{
    uint32 ctr = 0;
    uint32 off = 0;
    uint32 buflen = POLY_UNIFORM_NBLOCKS * STREAM128_BLOCKBYTES;
    uint8 buf[(POLY_UNIFORM_NBLOCKS * STREAM128_BLOCKBYTES) + 2u] = {0};
    FsmSw_Dilithium_stream128_state state = {0};

    FsmSw_Dilithium_shake128_stream_init(&state, seed, nonce);
    FsmSw_Fips202_shake128_inc_squeeze(buf, POLY_UNIFORM_NBLOCKS * SHAKE128_RATE, &state);

    ctr = fsmsw_dilithium5_RejUniform(a->coeffs, N_DILITHIUM, buf, buflen);

    while (ctr < N_DILITHIUM)
    {
        off = buflen % 3u;
        /* "As part of resolving the MISRA 2 warnings, a for-loop was removed here. If it is needed in the future, it must be reinserted." */

        FsmSw_Fips202_shake128_inc_squeeze(&buf[off], SHAKE128_RATE, &state);
        buflen = (uint32)(STREAM128_BLOCKBYTES + off);
        ctr += fsmsw_dilithium5_RejUniform(&a->coeffs[ctr], N_DILITHIUM - ctr, buf, buflen);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_poly_uniform_eta
*
* Description: Sample polynomial with uniformly random coefficients in [-ETA,ETA] by performing rejection sampling
*              on the output stream from SHAKE256(seed|nonce)
*
* Arguments:   -       poly_D5 *a:      pointer to output polynomial
*              - const uint8    seed[]: byte array with seed of length CRHBYTES_DILITHIUM
*              -       uint16   nonce:  2-byte nonce
***********************************************************************************************************************/
void FsmSw_Dilithium5_poly_uniform_eta(poly_D5 *a, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce)
{
    uint32 ctr = 0;
    /* polyspace +1 MISRA2012:2.2 [Justified:]"Calculation of buflen is important for generic implementation" */
    uint32 buflen = POLY_UNIFORM_ETA_NBLOCKS * STREAM256_BLOCKBYTES;
    /* polyspace +1 MISRA2012:2.2 [Justified:]"Calculation of buffer length is important for generic implementation" */
    uint8 buf[POLY_UNIFORM_ETA_NBLOCKS * STREAM256_BLOCKBYTES] = {0};
    FsmSw_Dilithium_stream256_state state = {0};

    FsmSw_Dilithium_shake256_stream_init(&state, seed, nonce);
    FsmSw_Fips202_shake256_inc_squeeze(buf, SHAKE256_RATE, &state);

    ctr = fsmsw_dilithium5_RejEta(a->coeffs, N_DILITHIUM, buf, buflen);

    while (ctr < N_DILITHIUM)
    {
        FsmSw_Fips202_shake256_inc_squeeze(buf, SHAKE256_RATE, &state);
        ctr += fsmsw_dilithium5_RejEta(&(a->coeffs[ctr]), N_DILITHIUM - ctr, buf, STREAM256_BLOCKBYTES);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_poly_uniform_gamma1
*
* Description: Sample polynomial with uniformly random coefficients in [-(GAMMA1 - 1), GAMMA1] by unpacking
*              output stream of SHAKE256(seed|nonce)
*
* Arguments:   -       poly_D5 *a:      pointer to output polynomial
*              - const uint8    seed[]: byte array with seed of length CRHBYTES_DILITHIUM
*              -       uint16   nonce:  16-bit nonce
***********************************************************************************************************************/
void FsmSw_Dilithium5_poly_uniform_gamma1(poly_D5 *a, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce)
{
    uint8 buf[POLY_UNIFORM_GAMMA1_NBLOCKS * STREAM256_BLOCKBYTES] = {0};
    FsmSw_Dilithium_stream256_state state = {0};

    FsmSw_Dilithium_shake256_stream_init(&state, seed, nonce);
    FsmSw_Fips202_shake256_inc_squeeze(buf, POLY_UNIFORM_GAMMA1_NBLOCKS * SHAKE256_RATE, &state);
    FsmSw_Dilithium5_polyz_unpack(a, buf);
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_challenge
*
* Description: Implementation of H. Samples polynomial with TAU nonzero coefficients in {-1,1} using the output stream
*              of SHAKE256(seed).
*
* Arguments:   -       poly_D5 *c:      pointer to output polynomial
*              - const uint8    seed[]: byte array containing seed of length SEEDBYTES_DILITHIUM
***********************************************************************************************************************/
void FsmSw_Dilithium5_poly_challenge(poly_D5 *c, const uint8 seed[SEEDBYTES_DILITHIUM])
{
    uint32 i = 0;
    uint32 b = 0;
    uint32 pos = 0;
    uint64 signs = 0;
    uint8 buf[SHAKE256_RATE] = {0};
    shake256incctx state = {0};

    FsmSw_Fips202_shake256_inc_init(&state);
    FsmSw_Fips202_shake256_inc_absorb(&state, seed, SEEDBYTES_DILITHIUM);
    FsmSw_Fips202_shake256_inc_finalize(&state);
    FsmSw_Fips202_shake256_inc_squeeze(buf, sizeof(buf), &state);

    
    signs = 0;
    for (i = 0; i < 8u; ++i)
    {
		signs |= (uint64)buf[i] << (8u * i);
    }
    
    pos = 8;

    for (i = 0; i < N_DILITHIUM; ++i)
    {
        c->coeffs[i] = 0;
    }

    for (i = N_DILITHIUM - TAU_DILITHIUM5; i < N_DILITHIUM; ++i)
    {
        do
        {
            if (pos >= SHAKE256_RATE)
            {
                FsmSw_Fips202_shake256_inc_squeeze(buf, sizeof(buf), &state);
                pos = 0;
            }

            b = buf[pos];
            pos++;
        } while (b > i);

        c->coeffs[i] = c->coeffs[b];
        c->coeffs[b] = (sint32)((uint32)((uint64)(1u - (2u * (signs & 1u)))));
        signs >>= 1;
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_polyeta_pack
*
* Description: Bit-pack polynomial with coefficients in [-ETA,ETA].
*
* Arguments:   -       uint8   *r: pointer to output byte array with at least POLYETA_PACKEDBYTES bytes
*              - const poly_D5 *a: pointer to input polynomial
***********************************************************************************************************************/
void FsmSw_Dilithium5_polyeta_pack(uint8 *r, const poly_D5 *a)
{
    uint16 i = 0;
    uint8  t[8] = {0};

    for (i = 0; i < (N_DILITHIUM / 8u); ++i)
    {
        t[0] = (uint8) (ETA_DILITHIUM5 - (uint8)a->coeffs[8u * i]);
        t[1] = (uint8) (ETA_DILITHIUM5 - (uint8)a->coeffs[(8u * i) + 1u]);
        t[2] = (uint8) (ETA_DILITHIUM5 - (uint8)a->coeffs[(8u * i) + 2u]);
        t[3] = (uint8) (ETA_DILITHIUM5 - (uint8)a->coeffs[(8u * i) + 3u]);
        t[4] = (uint8) (ETA_DILITHIUM5 - (uint8)a->coeffs[(8u * i) + 4u]);
        t[5] = (uint8) (ETA_DILITHIUM5 - (uint8)a->coeffs[(8u * i) + 5u]);
        t[6] = (uint8) (ETA_DILITHIUM5 - (uint8)a->coeffs[(8u * i) + 6u]);
        t[7] = (uint8) (ETA_DILITHIUM5 - (uint8)a->coeffs[(8u * i) + 7u]);

        r[3u * i]  = (t[0] >> 0) | (t[1] << 3) | (t[2] << 6);
        r[(3u * i) + 1u]  = (t[2] >> 2) | (t[3] << 1) | (t[4] << 4) | (t[5] << 7);
        r[(3u * i) + 2u]  = (t[5] >> 1) | (t[6] << 2) | (t[7] << 5);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_polyeta_unpack
*
* Description: Unpack polynomial with coefficients in [-ETA,ETA].
*
* Arguments:   -       poly_D5 *r: pointer to output polynomial
*              - const uint8   *a: byte array with bit-packed polynomial
***********************************************************************************************************************/
void FsmSw_Dilithium5_polyeta_unpack(poly_D5 *r, const uint8 *a)
{
    uint16 i = 0;

    for (i = 0; i < (N_DILITHIUM / 8u); ++i)
    {
        r->coeffs[8u * i] = (sint32)((uint32)((uint32)((uint32)(a[3u * i]) >> 0) & 7u));
        r->coeffs[(8u * i) + 1u] = (sint32)((uint32)((uint32)((uint32)(a[3u * i]) >> 3) & 7u));
        r->coeffs[(8u * i) + 2u] = (sint32)((uint32)(
                                                   (((uint32)(a[3u * i]) >> 6) |
                                                    ((uint32)(a[(3u * i) + 1u]) << 2u)
                                                   ) & 7u));
        r->coeffs[(8u * i) + 3u] = (sint32)((uint32)((uint32)((uint32)(a[(3u * i) + 1u]) >> 1) & 7u));
        r->coeffs[(8u * i) + 4u] = (sint32)((uint32)((uint32)((uint32)(a[(3u * i) + 1u]) >> 4) & 7u));
        r->coeffs[(8u * i) + 5u] = (sint32)((uint32)(
                                                   (((uint32)(a[(3u * i) + 1u]) >> 7) |
                                                    ((uint32)(a[(3u * i) + 2u]) << 1u)
                                                   ) & 7u));
        r->coeffs[(8u * i) + 6u] = (sint32)((uint32)((uint32)((uint32)(a[(3u * i) + 2u]) >> 2) & 7u));
        r->coeffs[(8u * i) + 7u] = (sint32)((uint32)((uint32)((uint32)(a[(3u * i) + 2u]) >> 5) & 7u));

        r->coeffs[8u * i] = (sint32)ETA_DILITHIUM5 - r->coeffs[8u * i];
        r->coeffs[(8u * i) + 1u] = (sint32)ETA_DILITHIUM5 - r->coeffs[(8u * i) + 1u];
        r->coeffs[(8u * i) + 2u] = (sint32)ETA_DILITHIUM5 - r->coeffs[(8u * i) + 2u];
        r->coeffs[(8u * i) + 3u] = (sint32)ETA_DILITHIUM5 - r->coeffs[(8u * i) + 3u];
        r->coeffs[(8u * i) + 4u] = (sint32)ETA_DILITHIUM5 - r->coeffs[(8u * i) + 4u];
        r->coeffs[(8u * i) + 5u] = (sint32)ETA_DILITHIUM5 - r->coeffs[(8u * i) + 5u];
        r->coeffs[(8u * i) + 6u] = (sint32)ETA_DILITHIUM5 - r->coeffs[(8u * i) + 6u];
        r->coeffs[(8u * i) + 7u] = (sint32)ETA_DILITHIUM5 - r->coeffs[(8u * i) + 7u];
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_polyt1_pack
*
* Description: Bit-pack polynomial t1 with coefficients fitting in 10 bits.
*              Input coefficients are assumed to be standard representatives.
*
* Arguments:   -       uint8   *r: pointer to output byte array with at least POLYT1_PACKEDBYTES bytes
*              - const poly_D5 *a: pointer to input polynomial
***********************************************************************************************************************/
void FsmSw_Dilithium5_polyt1_pack(uint8 *r, const poly_D5 *a)
{
    uint16 i = 0;

    for (i = 0; i < (N_DILITHIUM / 4u); ++i)
    {
        r[5u * i] = (uint8)((uint16)a->coeffs[4u * i] >> 0);
        r[(5u * i) + 1u] = (uint8)((uint16)(((uint16)a->coeffs[4u * i] >> 8)
                                        | ((uint16)a->coeffs[(4u * i) + 1u] << 2u)));
        r[(5u * i) + 2u] = (uint8)((uint16)(((uint16)a->coeffs[(4u * i) + 1u] >> 6)
                                        | ((uint16)a->coeffs[(4u * i) + 2u] << 4u)));
        r[(5u * i) + 3u] = (uint8)((uint16)(((uint16)a->coeffs[(4u * i) + 2u] >> 4)
                                        | ((uint16)a->coeffs[(4u * i) + 3u] << 6u)));
        r[(5u * i) + 4u] = (uint8)((uint16)a->coeffs[(4u * i) + 3u] >> 2);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_polyt1_unpack
*
* Description: Unpack polynomial t1 with 10-bit coefficients.
*              Output coefficients are standard representatives.
*
* Arguments:   -       poly_D5 *r: pointer to output polynomial
*              - const uint8   *a: byte array with bit-packed polynomial
***********************************************************************************************************************/
void FsmSw_Dilithium5_polyt1_unpack(poly_D5 *r, const uint8 *a)
{
    uint16 i = 0;

    for (i = 0; i < (N_DILITHIUM / 4u); ++i)
    {
        r->coeffs[4u * i] = (sint32)((uint32)(((uint32)(((uint32)a[5u * i] >> 0) |
                                                             ((uint32)a[(5u * i) + 1u] << 8u))) & 0x3FFu));
        r->coeffs[(4u * i) + 1u] = (sint32)((uint32)(((uint32)(((uint32)a[(5u * i) + 1u] >> 2) |
                                                             ((uint32)a[(5u * i) + 2u] << 6u))) & 0x3FFu));
        r->coeffs[(4u * i) + 2u] = (sint32)((uint32)(((uint32)(((uint32)a[(5u * i) + 2u] >> 4) |
                                                             ((uint32)a[(5u * i) + 3u] << 4u))) & 0x3FFu));
        r->coeffs[(4u * i) + 3u] = (sint32)((uint32)(((uint32)(((uint32)a[(5u * i) + 3u] >> 6) |
                                                             ((uint32)a[(5u * i) + 4u] << 2u))) & 0x3FFu));
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_polyt0_pack
*
* Description: Bit-pack polynomial t0 with coefficients in [-2^{D-1}, 2^{D-1}].
*
* Arguments:   -       uint8   *r: pointer to output byte array with at least POLYT0_PACKEDBYTES_DILITHIUM bytes
*              - const poly_D5 *a: pointer to input polynomial
***********************************************************************************************************************/
void FsmSw_Dilithium5_polyt0_pack(uint8 *r, const poly_D5 *a)
{
    uint16 i = 0;
    uint32 t[8] = {0};

    for (i = 0; i < (N_DILITHIUM / 8u); ++i)
    {
        t[0] = ((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)a->coeffs[8u * i]);
        t[1] = ((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)a->coeffs[(8u * i) + 1u]);
        t[2] = ((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)a->coeffs[(8u * i) + 2u]);
        t[3] = ((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)a->coeffs[(8u * i) + 3u]);
        t[4] = ((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)a->coeffs[(8u * i) + 4u]);
        t[5] = ((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)a->coeffs[(8u * i) + 5u]);
        t[6] = ((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)a->coeffs[(8u * i) + 6u]);
        t[7] = ((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)a->coeffs[(8u * i) + 7u]);

        r[13u * i]  = (uint8) t[0];
        r[(13u * i) +  1u]  = (uint8) (t[0] >>  8);
        r[(13u * i) +  1u] |= (uint8) (t[1] <<  5);
        r[(13u * i) +  2u]  = (uint8) (t[1] >>  3);
        r[(13u * i) +  3u]  = (uint8) (t[1] >> 11);
        r[(13u * i) +  3u] |= (uint8) (t[2] <<  2);
        r[(13u * i) +  4u]  = (uint8) (t[2] >>  6);
        r[(13u * i) +  4u] |= (uint8) (t[3] <<  7);
        r[(13u * i) +  5u]  = (uint8) (t[3] >>  1);
        r[(13u * i) +  6u]  = (uint8) (t[3] >>  9);
        r[(13u * i) +  6u] |= (uint8) (t[4] <<  4);
        r[(13u * i) +  7u]  = (uint8) (t[4] >>  4);
        r[(13u * i) +  8u]  = (uint8) (t[4] >> 12);
        r[(13u * i) +  8u] |= (uint8) (t[5] <<  1);
        r[(13u * i) +  9u]  = (uint8) (t[5] >>  7);
        r[(13u * i) +  9u] |= (uint8) (t[6] <<  6);
        r[(13u * i) + 10u]  = (uint8) (t[6] >>  2);
        r[(13u * i) + 11u]  = (uint8) (t[6] >> 10);
        r[(13u * i) + 11u] |= (uint8) (t[7] <<  3);
        r[(13u * i) + 12u]  = (uint8) (t[7] >>  5);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_polyt0_unpack
*
* Description: Unpack polynomial t0 with coefficients in ]-2^{D-1}, 2^{D-1}].
*
* Arguments:   -       poly_D5 *r: pointer to output polynomial
*              - const uint8   *a: byte array with bit-packed polynomial
***********************************************************************************************************************/
void FsmSw_Dilithium5_polyt0_unpack(poly_D5 *r, const uint8 *a)
{
    uint16 i = 0;

    for (i = 0; i < (N_DILITHIUM / 8u); ++i)
    {

        r->coeffs[8u * i] = (sint32)(a[13u * i]);
        r->coeffs[8u * i] = (sint32)((uint32)(((uint32)r->coeffs[8u * i]) |
                                                   ((uint32)a[(13u * i) + 1u] << 8u)));
        r->coeffs[8u * i] = (sint32)((uint32)(((uint32)r->coeffs[8u * i]) & 0x1FFFu));

        r->coeffs[(8u * i) + 1u] = (sint32)((uint32)((uint32)a[(13u * i) + 1u] >> 5));
        r->coeffs[(8u * i) + 1u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 1u]) |
                                                   ((uint32)a[(13u * i) + 2u] <<  3u)));
        r->coeffs[(8u * i) + 1u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 1u]) |
                                                   ((uint32)a[(13u * i) + 3u] << 11u)));
        r->coeffs[(8u * i) + 1u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 1u]) & 0x1FFFu));

        r->coeffs[(8u * i) + 2u] = (sint32)((uint32)((uint32)a[(13u * i) + 3u] >> 2));
        r->coeffs[(8u * i) + 2u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 2u]) |
                                                  ((uint32)a[(13u * i) + 4u] << 6u)));
        r->coeffs[(8u * i) + 2u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 2u]) & 0x1FFFu));

        r->coeffs[(8u * i) + 3u] = (sint32)((uint32)((uint32)a[(13u * i) + 4u] >> 7));
        r->coeffs[(8u * i) + 3u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 3u]) |
                                                   ((uint32)a[(13u * i) + 5u] << 1u)));
        r->coeffs[(8u * i) + 3u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 3u]) |
                                                   ((uint32)a[(13u * i) + 6u] << 9u)));
        r->coeffs[(8u * i) + 3u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 3u]) & 0x1FFFu));

        r->coeffs[(8u * i) + 4u] = (sint32)((uint32)((uint32)a[(13u * i) + 6u] >> 4));
        r->coeffs[(8u * i) + 4u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 4u]) |
                                                   ((uint32)a[(13u * i) + 7u] <<  4u)));
        r->coeffs[(8u * i) + 4u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 4u]) |
                                                   ((uint32)a[(13u * i) + 8u] << 12u)));
        r->coeffs[(8u * i) + 4u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 4u]) & 0x1FFFu));

        r->coeffs[(8u * i) + 5u] = (sint32)((uint32)((uint32)a[(13u * i) + 8u] >> 1));
        r->coeffs[(8u * i) + 5u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 5u]) |
                                                   ((uint32)a[(13u * i) + 9u] << 7u)));
        r->coeffs[(8u * i) + 5u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 5u]) & 0x1FFFu));

        r->coeffs[(8u * i) + 6u] = (sint32)((uint32)((uint32)a[(13u * i) + 9u] >> 6));
        r->coeffs[(8u * i) + 6u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 6u]) |
                                                   ((uint32)a[(13u * i) + 10u] <<  2u)));
        r->coeffs[(8u * i) + 6u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 6u]) |
                                                   ((uint32)a[(13u * i) + 11u] << 10u)));
        r->coeffs[(8u * i) + 6u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 6u]) & 0x1FFFu));

        r->coeffs[(8u * i) + 7u] = (sint32)((uint32)((uint32)a[(13u * i) + 11u] >> 3));
        r->coeffs[(8u * i) + 7u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 7u]) |
                                                   ((uint32)a[(13u * i) + 12u] << 5u)));
        r->coeffs[(8u * i) + 7u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 7u]) & 0x1FFFu));

        r->coeffs[8u * i] = (sint32)((uint32)(((uint32)1u << (D_DILITHIUM - 1u)) -
                                                   ((uint32)r->coeffs[8u * i])));
        r->coeffs[(8u * i) + 1u] = (sint32)((uint32)(((uint32)1u << (D_DILITHIUM - 1u)) -
                                                   ((uint32)r->coeffs[(8u * i) + 1u])));
        r->coeffs[(8u * i) + 2u] = (sint32)((uint32)(((uint32)1u << (D_DILITHIUM - 1u)) -
                                                   ((uint32)r->coeffs[(8u * i) + 2u])));
        r->coeffs[(8u * i) + 3u] = (sint32)((uint32)(((uint32)1u << (D_DILITHIUM - 1u)) -
                                                   ((uint32)r->coeffs[(8u * i) + 3u])));
        r->coeffs[(8u * i) + 4u] = (sint32)((uint32)(((uint32)1u << (D_DILITHIUM - 1u)) -
                                                   ((uint32)r->coeffs[(8u * i) + 4u])));
        r->coeffs[(8u * i) + 5u] = (sint32)((uint32)(((uint32)1u << (D_DILITHIUM - 1u)) -
                                                   ((uint32)r->coeffs[(8u * i) + 5u])));
        r->coeffs[(8u * i) + 6u] = (sint32)((uint32)(((uint32)1u << (D_DILITHIUM - 1u)) -
                                                   ((uint32)r->coeffs[(8u * i) + 6u])));
        r->coeffs[(8u * i) + 7u] = (sint32)((uint32)(((uint32)1u << (D_DILITHIUM - 1u)) -
                                                   ((uint32)r->coeffs[(8u * i) + 7u])));
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_polyz_pack
*
* Description: Bit-pack polynomial with coefficients in [-(GAMMA1 - 1), GAMMA1].
*
* Arguments:   -       uint8   *r: pointer to output byte array with at least POLYZ_PACKEDBYTES_DILITHIUM5 bytes
*              - const poly_D5 *a: pointer to input polynomial
***********************************************************************************************************************/
void FsmSw_Dilithium5_polyz_pack(uint8 *r, const poly_D5 *a)
{
    uint16 i = 0;
    uint32 t[4] = {0};

    for (i = 0; i < (N_DILITHIUM / 2u); ++i)
    {
        t[0] = GAMMA1_DILITHIUM5 - (uint32)a->coeffs[2u * i];
        t[1] = GAMMA1_DILITHIUM5 - (uint32)a->coeffs[(2u * i) + 1u];

        r[5u * i]  = (uint8) t[0];
        r[(5u * i) + 1u]  = (uint8) (t[0] >> 8);
        r[(5u * i) + 2u]  = (uint8) (t[0] >> 16);
        r[(5u * i) + 2u] |= (uint8) (t[1] << 4);
        r[(5u * i) + 3u]  = (uint8) (t[1] >> 4);
        r[(5u * i) + 4u]  = (uint8) (t[1] >> 12);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_polyz_unpack
*
* Description: Unpack polynomial z with coefficients in [-(GAMMA1 - 1), GAMMA1].
*
* Arguments:   -       poly_D5 *r: pointer to output polynomial
*              - const uint8   *a: byte array with bit-packed polynomial
***********************************************************************************************************************/
void FsmSw_Dilithium5_polyz_unpack(poly_D5 *r, const uint8 *a)
{
    uint16 i = 0;

    for (i = 0; i < (N_DILITHIUM / 2u); ++i)
    {
        r->coeffs[2u * i] = (sint32)a[5u * i];
        r->coeffs[2u * i] = (sint32)((uint32)((uint32)r->coeffs[2u * i] | ((uint32)a[(5u * i) + 1u] <<   8u)));
        r->coeffs[2u * i] = (sint32)((uint32)((uint32)r->coeffs[2u * i] | ((uint32)a[(5u * i) + 2u] <<  16u)));
        r->coeffs[2u * i] = (sint32)((uint32)((uint32)r->coeffs[2u * i] & 0xFFFFFu));

        r->coeffs[(2u * i) + 1u] = (sint32)((uint32)((uint32)a[(5u * i) + 2u] >> 4));
        r->coeffs[(2u * i) + 1u] = (sint32)((uint32)((uint32)r->coeffs[(2u * i) + 1u] | ((uint32)a[(5u * i) + 3u] <<    4u)));
        r->coeffs[(2u * i) + 1u] = (sint32)((uint32)((uint32)r->coeffs[(2u * i) + 1u] | ((uint32)a[(5u * i) + 4u] <<   12u)));
        /* polyspace +1 MISRA2012:3.1 [Justified:]"The comment is a link and therefore contains a slash" */
        /* Known issue: This row is maybe not correct. See https://github.com/pq-crystals/dilithium/issues/63 */
        r->coeffs[2u * i] = (sint32)((uint32)((uint32)r->coeffs[2u * i] & 0xFFFFFu));

        r->coeffs[2u * i] = (sint32)((uint32)(GAMMA1_DILITHIUM5 - (uint32)r->coeffs[2u * i]));
        r->coeffs[(2u * i) + 1u] = (sint32)((uint32)(GAMMA1_DILITHIUM5 - (uint32)r->coeffs[(2u * i) + 1u]));
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_polyw1_pack
*
* Description: Bit-pack polynomial w1 with coefficients in [0,15] or [0,43].
*              Input coefficients are assumed to be standard representatives.
*
* Arguments:   -       uint8   *r: pointer to output byte array with at least POLYW1_PACKEDBYTES_DILITHIUM5  bytes
*              - const poly_D5 *a: pointer to input polynomial
***********************************************************************************************************************/
void FsmSw_Dilithium5_polyw1_pack(uint8 *r, const poly_D5 *a)
{
    uint16 i = 0;

    for (i = 0; i < (N_DILITHIUM / 2u); ++i)
    {
        r[i] = (uint8) ((uint32)((uint32)a->coeffs[2u * i] | ((uint32)a->coeffs[(2u * i) + 1u] << 4u)));
    }
}
