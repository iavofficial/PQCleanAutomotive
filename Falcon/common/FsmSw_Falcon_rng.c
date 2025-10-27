/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup common
*    includes the modules for common
 ** @{ */
/** \addtogroup Falcon_rng
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Falcon_rng.c
* \brief  description of FsmSw_Falcon_rng.c
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
/* A PRNG based on ChaCha20 is implemented; it is seeded from a SHAKE256 context (flipped) and is used for bulk
 * pseudorandom generation. A system-dependent seed generator is also provided. */
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_CommonLib.h"
#include "FsmSw_Falcon_common.h"

#include "FsmSw_Falcon_rng.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
/* polyspace +4 CERT-C:PRE00-C [Justified:]"No refactoring of macros, as converting to, for example, 
inline functions would not provide significant benefits." */
/* polyspace +2 MISRA2012:D4.9 [Justified:]"No refactoring of macros, as converting to, for example, 
inline functions would not provide significant benefits." */
#define QROUND(a, b, c, d)                                                                                             \
  do                                                                                                                   \
  {                                                                                                                    \
    state[a] += state[b];                                                                                              \
    state[d] ^= state[a];                                                                                              \
    state[d] = (state[d] << 16) | (state[d] >> 16);                                                                    \
    state[c] += state[d];                                                                                              \
    state[b] ^= state[c];                                                                                              \
    state[b] = (state[b] << 12) | (state[b] >> 20);                                                                    \
    state[a] += state[b];                                                                                              \
    state[d] ^= state[a];                                                                                              \
    state[d] = (state[d] << 8) | (state[d] >> 24);                                                                     \
    state[c] += state[d];                                                                                              \
    state[b] ^= state[c];                                                                                              \
    state[b] = (state[b] << 7) | (state[b] >> 25);                                                                     \
  } while (0)
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
static void fsmsw_falcon_prng_Refill(prng *p);
/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
* \brief fsmsw_falcon_prng_Refill based on ChaCha20.
*        State consists in key (32 bytes) then IV (16 bytes) and block counter (8 bytes). Normally, we should not
*        care about local endianness (this is for a PRNG), but for the NIST competition we need reproducible KAT
*        vectors that work across architectures, so we enforce little-endian interpretation where applicable.
*        Moreover, output words are "spread out" over the output buffer with the interleaving pattern that is
*        naturally obtained from the AVX2 implementation that runs eight ChaCha20 instances in parallel.
*        The block counter is XORed into the first 8 bytes of the IV.
*
* \param[out] prng *p : t.b.d.
*
*/
static void fsmsw_falcon_prng_Refill(prng *p)
{
  static const uint32 CW[] = {0x61707865, 0x3320646e, 0x79622d32, 0x6b206574};

  uint64 cc = 0;
  uint32 u  = 0;

  /* polyspace +6 CERT-C:EXP36-C [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  /* polyspace +4 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  /* State uses local endianness. Only the output bytes must be converted to little endian (if used on a
     * big-endian machine). */
  cc = *(uint64 *)((void *)(&p->state.d[48]));
  for (u = 0; u < 8u; u++)
  {
    uint32 state[16];
    uint32 v;
    sint32 i;

    FsmSw_CommonLib_MemCpy(&state[0], CW, sizeof(CW));
    FsmSw_CommonLib_MemCpy(&state[4], p->state.d, 48);
    state[14] ^= (uint32)cc;
    state[15] ^= (uint32)(cc >> 32);
    for (i = 0; i < 10; i++)
    {
      QROUND(0, 4, 8, 12);
      QROUND(1, 5, 9, 13);
      QROUND(2, 6, 10, 14);
      QROUND(3, 7, 11, 15);
      QROUND(0, 5, 10, 15);
      QROUND(1, 6, 11, 12);
      QROUND(2, 7, 8, 13);
      QROUND(3, 4, 9, 14);
    }

    for (v = 0; v < 4u; v++)
    {
      state[v] += CW[v];
    }

    for (v = 4; v < 14u; v++)
    {
      /* polyspace +4 CERT-C:EXP36-C [Justified:]"Necessary conversion from void* to object* for functionality. 
          Ensured proper alignment and validity." */
      /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality. 
          Ensured proper alignment and validity." */
      state[v] = state[v] + ((uint32 *)((void *)p->state.d))[v - 4u];
    }

    uint32 temp2[256];
    FsmSw_CommonLib_MemCpy(temp2, p->state.d, 256);
    state[14] += temp2[10] ^ (uint32)cc;
    state[15] += temp2[11] ^ (uint32)(cc >> 32);
    cc++;

    /* We mimic the interleaving that is used in the AVX2 implementation.*/
    for (v = 0; v < 16u; v++)
    {
      p->buf.d[(u << 2) + (v << 5)]      = (uint8)state[v];
      p->buf.d[(u << 2) + (v << 5) + 1u] = (uint8)(state[v] >> 8);
      p->buf.d[(u << 2) + (v << 5) + 2u] = (uint8)(state[v] >> 16);
      p->buf.d[(u << 2) + (v << 5) + 3u] = (uint8)(state[v] >> 24);
    }
  }

  /* polyspace +4 CERT-C:EXP36-C [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  *((uint64 *)((void *)(&p->state.d[48]))) = cc;

  p->ptr = 0;
} // end: fsmsw_falcon_prng_Refill

/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
* \brief Instantiate a PRNG. That PRNG will feed over the provided SHAKE256 context (in "flipped" state) to
*        obtain its initial state.
*
* \param[out] prng                     *p : t.b.d.
* \param[out] inner_shake256_context *src : t.b.d.
*
*/
void FsmSw_Falcon_Prng_Init(prng *p, inner_shake256_context *const src)
{
  /* To ensure reproducibility for a given seed, we must enforce little-endian interpretation of the state words. */
  uint8 tmp[56] = {0};
  uint64 th     = 0;
  uint64 tl     = 0;
  sint32 i      = 0;
  uint32 w      = 0;

  /* polyspace +5 CERT-C:EXP36-C [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  /* polyspace +3 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  uint32 *const d32 = (uint32 *)((void *)p->state.d);
  uint64 *const d64 = (uint64 *)((void *)p->state.d);

  FsmSw_Fips202_Shake256_IncSqueeze(tmp, 56, src);

  for (i = 0; i < 14; i++)
  {
    w = (uint32)tmp[((uint32)((uint32)i) << 2u)] | ((uint32)tmp[((uint32)((uint32)i) << 2u) + 1u] << 8u) |
        ((uint32)tmp[((uint32)((uint32)i) << 2u) + 2u] << 16u) | ((uint32)tmp[((uint32)((uint32)i) << 2u) + 3u] << 24u);
    d32[i] = w;
  }

  tl                        = d32[48u / sizeof(uint32)];
  th                        = d32[52u / sizeof(uint32)];
  d64[48u / sizeof(uint64)] = tl + (th << 32);
  fsmsw_falcon_prng_Refill(p);
} // end: FsmSw_Falcon_Prng_Init

/*====================================================================================================================*/
/**
* \brief Get some bytes from a PRNG.
*
* \param[out] prng    *p : t.b.d.
* \param[out] void  *dst : t.b.d.
* \param[out] uint32 len : t.b.d.
*
*/
void FsmSw_Falcon_Prng_GetBytes(prng *const p, void *const dst, uint32 len)
{
  uint8 *buf  = (uint8 *)NULL_PTR;
  uint32 clen = 0;

  /* len_temp is used to avoid modifying the input. */
  uint32 len_temp = len;

  /* polyspace +4 CERT-C:EXP36-C [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  buf = dst;
  while (len_temp > 0u)
  {
    clen = (sizeof(p->buf.d)) - p->ptr;

    if (clen > len_temp)
    {
      clen = len_temp;
    }

    FsmSw_CommonLib_MemCpy(buf, p->buf.d, clen);
    buf = &buf[clen];
    len_temp -= clen;
    p->ptr += clen;

    if (p->ptr == sizeof(p->buf.d))
    {
      fsmsw_falcon_prng_Refill(p);
    }
  }
} // end: FsmSw_Falcon_Prng_GetBytes

/*====================================================================================================================*/
/**
* \brief  Get a 64-bit random value from a PRNG.
*
* \param[out] prng *p : t.b.d.
*
*/
uint64 FsmSw_Falcon_Prng_GetU64(prng *const p)
{
  uint32 u = 0;

  /* If there are less than 9 bytes in the buffer, we refill it. This means that we may drop the last few bytes, but
   * this allows for faster extraction code. Also, it means that we never leave an empty buffer. */
  u = p->ptr;
  if (u >= ((sizeof(p->buf.d)) - 9u))
  {
    fsmsw_falcon_prng_Refill(p);
    u = 0;
  }
  p->ptr = u + 8u;

  return (uint64)((uint64)p->buf.d[u] | ((uint64)p->buf.d[u + 1u] << 8) | ((uint64)p->buf.d[u + 2u] << 16) |
                  ((uint64)p->buf.d[u + 3u] << 24) | ((uint64)p->buf.d[u + 4u] << 32) |
                  ((uint64)p->buf.d[u + 5u] << 40) | ((uint64)p->buf.d[u + 6u] << 48) |
                  ((uint64)p->buf.d[u + 7u] << 56));
} // end: FsmSw_Falcon_Prng_GetU64

/*====================================================================================================================*/
/**
* \brief  Get an 8-bit random value from a PRNG.
*
* \param[out] prng *p : t.b.d.
*
*/
uint32 FsmSw_Falcon_Prng_GetU8(prng *const p)
{
  uint32 v = 0;

  v = p->buf.d[p->ptr];
  p->ptr++;
  if (p->ptr == sizeof(p->buf.d))
  {
    fsmsw_falcon_prng_Refill(p);
  }
  return v;
} // end: FsmSw_Falcon_Prng_GetU8

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
