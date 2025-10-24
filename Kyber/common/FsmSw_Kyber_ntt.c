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
/** \addtogroup Kyber_ntt
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Kyber_ntt.c
* \brief  description of FsmSw_Kyber_ntt.c
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
#include "FsmSw_Kyber_params.h"
#include "FsmSw_Kyber_reduce.h"
#include "FsmSw_Types.h"

#include "FsmSw_Kyber_ntt.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/
/* polyspace +6 CERT-C:DCL15-C [Justified:]"The variable is used across multiple translation units, 
and external linkage is required for proper functionality." */
/* polyspace +4 CERT-C:DCL19-C [Justified:]"The variable is used across multiple translation units, 
and external linkage is required for proper functionality." */
/* polyspace +2 MISRA2012:8.7 [Justified:]"The variable is used across multiple translation units, 
and external linkage is required for proper functionality." */
const sint16 FsmSw_Kyber_zetas[128] = {
    -1044, -758,  -359,  -1517, 1493,  1422,  287,   202,   -171,  622,  1577,  182,   962,   -1202, -1474, 1468,
    573,   -1325, 264,   383,   -829,  1458,  -1602, -130,  -681,  1017, 732,   608,   -1542, 411,   -205,  -1571,
    1223,  652,   -552,  1015,  -1293, 1491,  -282,  -1544, 516,   -8,   -320,  -666,  -1618, -1162, 126,   1469,
    -853,  -90,   -271,  830,   107,   -1421, -247,  -951,  -398,  961,  -1508, -725,  448,   -1065, 677,   -1275,
    -1103, 430,   555,   843,   -1251, 871,   1550,  105,   422,   587,  177,   -235,  -291,  -460,  1574,  1653,
    -246,  778,   1159,  -147,  -777,  1483,  -602,  1119,  -1590, 644,  -872,  349,   418,   329,   -156,  -75,
    817,   1097,  603,   610,   1322,  -1285, -1465, 384,   -1215, -136, 1218,  -1335, -874,  220,   -1187, -1659,
    -1185, -1530, -1278, 794,   -1510, -854,  -870,  478,   -108,  -308, 996,   991,   958,   -1460, 1522,  1628};
/**********************************************************************************************************************/
/* GLOBAL CONSTANTS                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/
static sint16 fsmsw_kyber_Fqmul(sint16 a, sint16 b);

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
* \brief Multiplication followed by Montgomery reduction
*
* \param[in] int16_t a : first factor
* \param[in] int16_t b : second factor
*
* \returns 16-bit integer congruent to a*b*R^{-1} mod q
*/
static sint16 fsmsw_kyber_Fqmul(sint16 a, sint16 b)
{
  return FsmSw_Kyber_MontgomeryReduce((sint32)a * b);
} // end: fsmsw_kyber_Fqmul
/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
* \brief Inplace number-theoretic transform (NTT) in Rq.
*        input is in standard order, output is in bitreversed order
*
* \param[in,out] int16_t r[256] : pointer to input/output vector of elements of Zq
*/
void FsmSw_Kyber_Ntt(sint16 r[256])
{
  uint16 len   = 0;
  uint16 start = 0;
  uint16 j     = 0;
  uint16 k     = 1u;
  sint16 t     = 0;
  sint16 zeta  = 0;

  for (len = 128u; len >= 2u; len >>= 1u)
  {
    for (start = 0; start < 256u; start = j + len)
    {
      zeta = FsmSw_Kyber_zetas[k];
      k++;
      for (j = start; j < (start + len); j++)
      {
        t          = fsmsw_kyber_Fqmul(zeta, r[j + len]);
        r[j + len] = r[j] - t;
        r[j]       = r[j] + t;
      }
    }
  }
} // end: FsmSw_Kyber_Ntt

/*====================================================================================================================*/
/**
* \brief Inplace inverse number-theoretic transform in Rq and
*        multiplication by Montgomery factor 2^16.
*        Input is in bitreversed order, output is in standard order
*
* \param[in,out] int16_t r[256] : pointer to input/output vector of elements of Zq
*/
void FsmSw_Kyber_Invntt(sint16 r[256])
{
  uint16 start   = 0;
  uint16 len     = 0;
  uint16 j       = 0;
  uint16 k       = 127u;
  sint16 t       = 0;
  sint16 zeta    = 0;
  const sint16 f = 1441; /* mont^2/128 */

  for (len = 2u; len <= 128u; len <<= 1u)
  {
    for (start = 0; start < 256u; start = j + len)
    {
      zeta = FsmSw_Kyber_zetas[k];
      k--;
      for (j = start; j < (start + len); j++)
      {
        t          = r[j];
        r[j]       = FsmSw_Kyber_BarrettReduce(t + r[j + len]);
        r[j + len] = r[j + len] - t;
        r[j + len] = fsmsw_kyber_Fqmul(zeta, r[j + len]);
      }
    }
  }

  for (j = 0; j < 256u; j++)
  {
    r[j] = fsmsw_kyber_Fqmul(r[j], f);
  }
} // end: FsmSw_Kyber_Invntt

/*====================================================================================================================*/
/**
* \brief Multiplication of polynomials in Zq[X]/(X^2-zeta)
*        used for multiplication of elements in Rq in NTT domain
*
* \param[out] int16_t       r[2] : pointer to the output polynomial
* \param[in]  const int16_t a[2] : pointer to the first factor
* \param[in]  const int16_t b[2] : pointer to the second factor
* \param[in]  int16_t       zeta : integer defining the reduction polynomial
*/
void FsmSw_Kyber_Basemul(sint16 r[2], const sint16 a[2], const sint16 b[2], sint16 zeta)
{
  r[0] = fsmsw_kyber_Fqmul(a[1], b[1]);
  r[0] = fsmsw_kyber_Fqmul(r[0], zeta);
  r[0] += fsmsw_kyber_Fqmul(a[0], b[0]);
  r[1] = fsmsw_kyber_Fqmul(a[0], b[1]);
  r[1] += fsmsw_kyber_Fqmul(a[1], b[0]);
} // end: FsmSw_Kyber_Basemul

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
