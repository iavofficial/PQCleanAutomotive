/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Kyber512
*    includes the modules for Kyber512
 ** @{ */
/** \addtogroup Kyber512_poly
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Kyber512_poly.c
* \brief  description of FsmSw_Kyber512_poly.c
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
#include "FsmSw_Kyber512_cbd.h"
#include "FsmSw_Kyber512_params.h"
#include "FsmSw_Kyber_ntt.h"
#include "FsmSw_Kyber_poly.h"
#include "FsmSw_Kyber_reduce.h"
#include "FsmSw_Kyber_symmetric.h"
#include "FsmSw_Types.h"

#include "FsmSw_Kyber512_poly.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define FSMSW_KYBER512_POLY_BLOCK_SIZE 8u
#define FSMSW_KYBER512_POLY_T_SIZE     8
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
* \brief Compression and subsequent serialization of a polynomial
*
* \param[out] uint8      *r : pointer to output byte array (of length KYBER512_POLYCOMPRESSEDBYTES)
* \param[in]  const poly *a : pointer to input polynomial
*/
void FsmSw_Kyber512_Poly_Compress(uint8 r[KYBER512_POLYCOMPRESSEDBYTES], const poly *const a)
{
  uint16 i                            = 0;
  sint16 u                            = 0;
  uint8 j                             = 0;
  uint8 t[FSMSW_KYBER512_POLY_T_SIZE] = {0};

  /* r_temp is used to avoid modifying the input. */
  uint8 *r_temp = r;

  for (i = 0; i < (KYBER_N / 8u); i++)
  {
    for (j = 0; j < FSMSW_KYBER512_POLY_BLOCK_SIZE; j++)
    {
      /* map to positive standard representatives */
      u    = a->coeffs[(8u * i) + j];
      u    = (sint16)(u + (sint16)((uint16)(((uint32)u >> 15u) & KYBER_Q)));
      t[j] = (uint8)(((((uint16)u << 4u) + (KYBER_Q / 2u)) / KYBER_Q) & 15u);
    }

    r_temp[0] = t[0] | (t[1] << 4);
    r_temp[1] = t[2] | (t[3] << 4);
    r_temp[2] = t[4] | (t[5] << 4);
    r_temp[3] = t[6] | (t[7] << 4);
    r_temp    = &(r_temp[4]);
  }
} // end: FsmSw_Kyber512_Poly_Compress

/*====================================================================================================================*/
/**
* \brief De-serialization and subsequent decompression of a polynomial;
*        approximate inverse of FsmSw_Kyber512_Poly_Compress
*
* \param[out] poly        *r : pointer to output polynomial
* \param[in]  const uint8 *a : pointer to input byte array (of length KYBER512_POLYCOMPRESSEDBYTES bytes)
*/
void FsmSw_Kyber512_Poly_Decompress(poly *const r, const uint8 a[KYBER512_POLYCOMPRESSEDBYTES])
{
  uint16 i     = 0;
  uint16 temp1 = 0;
  uint8 temp2  = 0;

  /* a_temp is used to avoid modifying the input. */
  const uint8 *a_temp = a;

  for (i = 0; i < (KYBER_N / 2u); i++)
  {
    temp1                 = (uint16)a_temp[0] & 15u;
    r->coeffs[((2u * i))] = (sint16)(uint16)((((uint32)temp1 * KYBER_Q) + 8u) >> 4u);

    temp2                      = a_temp[0] >> 4u;
    r->coeffs[((2u * i) + 1u)] = (sint16)(uint16)((((uint32)temp2 * KYBER_Q) + 8u) >> 4u);

    a_temp = &(a_temp[1]);
  }
} // end: FsmSw_Kyber512_Poly_Decompress

/*====================================================================================================================*/
/**
* \brief Convert 32-byte message to polynomial
*
* \param[out] poly          *r : pointer to output polynomial
* \param[in]  const uint8 *msg : pointer to input message
*/
void FsmSw_Kyber512_Poly_FromMsg(poly *const r, const uint8 msg[KYBER512_INDCPA_MSGBYTES])
{
  uint8 j     = 0;
  uint16 i    = 0;
  sint16 mask = 0;

  for (i = 0; i < (KYBER_N / 8u); i++)
  {
    for (j = 0; j < FSMSW_KYBER512_POLY_BLOCK_SIZE; j++)
    {
      mask                    = -(sint16)((uint16)(((uint32)msg[i] >> j) & 1u));
      r->coeffs[(8u * i) + j] = (sint16)((uint16)((uint16)mask & ((uint16)((KYBER_Q + 1u) / 2u))));
    }
  }
} // end: FsmSw_Kyber512_Poly_FromMsg

/*====================================================================================================================*/
/**
* \brief Convert polynomial to 32-byte message
*
* \param[out] uint8    *msg : pointer to output message
* \param[in]  const poly *a : pointer to input polynomial
*/
void FsmSw_Kyber512_Poly_ToMsg(uint8 msg[KYBER512_INDCPA_MSGBYTES], const poly *const a)
{
  uint8 j  = 0;
  uint16 i = 0;
  uint16 t = 0;

  for (i = 0; i < (KYBER_N / 8u); i++)
  {
    msg[i] = 0;
    for (j = 0; j < FSMSW_KYBER512_POLY_BLOCK_SIZE; j++)
    {
      t = (uint16)(a->coeffs[(8u * i) + j]);
      /* Shift to get the first bit */
      if ((t >> 15u) != 0u)
      {
        t = t + KYBER_Q;
      }

      t      = (((t << 1u) + (KYBER_Q / 2u)) / KYBER_Q) & 1u;
      msg[i] = (uint8)((uint16)msg[i] | (t << j));
    }
  }
} // end: FsmSw_Kyber512_Poly_ToMsg

/*====================================================================================================================*/
/**
* \brief Sample a polynomial deterministically from a seed and a nonce,
*        with output polynomial close to centered binomial distribution
*        with parameter KYBER512_ETA1
*
* \param[out] poly           *r : pointer to output polynomial
* \param[in]  const uint8 *seed : pointer to input seed (of length KYBER_SYMBYTES bytes)
* \param[in]  uint8       nonce : one-byte input nonce
*/
void FsmSw_Kyber512_Poly_GetNoiseEta1(poly *const r, const uint8 seed[KYBER_SYMBYTES], uint8 nonce)
{
  uint8 buf[KYBER512_ETA1 * KYBER_N / 4u] = {0};

  FsmSw_Kyber_Shake256_Prf(buf, sizeof(buf), seed, nonce);
  FsmSw_Kyber512_Poly_Cbd_Eta1(r, buf);
} // end: FsmSw_Kyber512_Poly_GetNoiseEta1

/*====================================================================================================================*/
/**
* \brief Sample a polynomial deterministically from a seed and a nonce,
*        with output polynomial close to centered binomial distribution
*        with parameter KYBER512_ETA2
*
* \param[out]           poly *r : pointer to output polynomial
* \param[in]  const uint8 *seed : pointer to input seed (of length KYBER_SYMBYTES bytes)
* \param[in]  uint8       nonce : one-byte input nonce
*/
/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_Kyber512_Poly_GetNoiseEta2(poly *const r, const uint8 seed[KYBER_SYMBYTES], uint8 nonce)
{
  uint8 buf[KYBER512_ETA2 * KYBER_N / 4u] = {0};

  FsmSw_Kyber_Shake256_Prf(buf, sizeof(buf), seed, nonce);
  FsmSw_Kyber512_Poly_Cbd_Eta2(r, buf);
} // end: FsmSw_Kyber512_Poly_GetNoiseEta2

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */