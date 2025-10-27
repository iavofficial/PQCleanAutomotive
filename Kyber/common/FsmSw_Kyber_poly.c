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
/** \addtogroup Kyber_poly
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Kyber_poly.c
* \brief  description of FsmSw_Kyber_poly.c
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
#include "FsmSw_Kyber_CommonLib.h"
#include "FsmSw_Kyber_ntt.h"
#include "FsmSw_Kyber_reduce.h"
#include "FsmSw_Kyber_symmetric.h"
#include "FsmSw_Types.h"

#include "FsmSw_Kyber_poly.h"
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
* \brief Serialization of a polynomial
*
* \param[out] uint8      *r : pointer to output byte array (needs space for KYBER_POLYBYTES bytes)
* \param[in]  const poly *a : pointer to input polynomial
*/
void FsmSw_Kyber_Poly_ToBytes(uint8 r[KYBER_POLYBYTES], const poly *a)
{
  uint16 i  = 0;
  uint16 t0 = 0;
  uint16 t1 = 0;

  for (i = 0; i < (KYBER_N / 2u); i++)
  {
    /* map to positive standard representatives */
    t0 = (uint16)(a->coeffs[2u * i]);

    /* Shift to get the first bit */
    if ((t0 >> 15u) != 0u)
    {
      t0 = t0 + KYBER_Q;
    }

    t1 = (uint16)(a->coeffs[(2u * i) + 1u]);
    /* Shift to get the first bit */
    if ((t1 >> 15u) != 0u)
    {
      t1 = t1 + KYBER_Q;
    }

    r[3u * i]        = (uint8)(t0 >> 0);
    r[(3u * i) + 1u] = (uint8)((t0 >> 8u) | (t1 << 4u));
    r[(3u * i) + 2u] = (uint8)(t1 >> 4u);
  }
} // end: FsmSw_Kyber_Poly_ToBytes

/*====================================================================================================================*/
/**
* \brief De-serialization of a polynomial;
*        inverse of FsmSw_Kyber_Poly_ToBytes
*
* \param[out] poly        *r : pointer to output polynomial
* \param[in]  const uint8 *a : pointer to input byte array (of KYBER_POLYBYTES bytes)
*/
void FsmSw_Kyber_Poly_FromBytes(poly *r, const uint8 a[KYBER_POLYBYTES])
{
  uint16 i = 0;

  for (i = 0; i < (KYBER_N / 2u); i++)
  {
    r->coeffs[2u * i] =
        (sint16)((uint16)((((((uint16)a[3u * i]) >> 0u) | (((uint16)a[((3u * i) + 1u)]) << 8u))) & 0xFFFu));

    r->coeffs[(2u * i) + 1u] =
        (sint16)((uint16)((((((uint16)a[(3u * i) + 1u]) >> 4u) | (((uint16)a[(3u * i) + 2u]) << 4u))) & 0xFFFu));
  }
} // end: FsmSw_Kyber_Poly_FromBytes

/*====================================================================================================================*/
/**
* \brief Computes negacyclic number-theoretic transform (NTT) of
*        a polynomial in place;
*        inputs assumed to be in normal order, output in bitreversed order
*
* \param[in,out] uint16 *r : pointer to in/output polynomial
*/
void FsmSw_Kyber_Poly_Ntt(poly *r)
{
  FsmSw_Kyber_Ntt(r->coeffs);
  FsmSw_Kyber_Poly_Reduce(r);
} // end: FsmSw_Kyber_Poly_Ntt

/*====================================================================================================================*/
/**
* \brief Computes inverse of negacyclic number-theoretic transform (NTT)
*        of a polynomial in place;
*        inputs assumed to be in bitreversed order, output in normal order
*
* \param[in,out] uint16 *a : pointer to in/output polynomial
*/
void FsmSw_Kyber_Poly_InvnttTomont(poly *r)
{
  FsmSw_Kyber_Invntt(r->coeffs);
} // end: FsmSw_Kyber_Poly_InvnttTomont

/*====================================================================================================================*/
/**
* \brief Multiplication of two polynomials in NTT domain
*
* \param[out] poly       *r : pointer to output polynomial
* \param[in]  const poly *a : pointer to first input polynomial
* \param[in]  const poly *b : pointer to second input polynomial
*/
void FsmSw_Kyber_Poly_BasemulMontgomery(poly *r, const poly *a, const poly *b)
{
  uint16 i = 0;

  for (i = 0; i < (KYBER_N / 4u); i++)
  {
    FsmSw_Kyber_Basemul(&r->coeffs[4u * i], &a->coeffs[4u * i], &b->coeffs[4u * i], FsmSw_Kyber_zetas[64u + i]);

    FsmSw_Kyber_Basemul(&r->coeffs[(4u * i) + 2u], &a->coeffs[(4u * i) + 2u], &b->coeffs[(4u * i) + 2u],
                        -FsmSw_Kyber_zetas[64u + i]);
  }
} // end: FsmSw_Kyber_Poly_BasemulMontgomery

/*====================================================================================================================*/
/**
* \brief Inplace conversion of all coefficients of a polynomial
*        from normal domain to Montgomery domain
*
* \param[in,out] poly *r : pointer to input/output polynomial
*/
void FsmSw_Kyber_Poly_Tomont(poly *r)
{
  uint16 i       = 0;
  const sint16 f = (sint16)((1ULL << 32u) % KYBER_Q);

  for (i = 0; i < KYBER_N; i++)
  {
    r->coeffs[i] = FsmSw_Kyber_MontgomeryReduce((sint32)r->coeffs[i] * (sint32)f);
  }
} // end: FsmSw_Kyber_Poly_Tomont

/*====================================================================================================================*/
/**
* \brief Applies Barrett reduction to all coefficients of a polynomial
*        for details of the Barrett reduction see comments in reduce.c
*
* \param[in,out] poly *r : pointer to input/output polynomial
*/
void FsmSw_Kyber_Poly_Reduce(poly *r)
{
  uint16 i = 0;

  for (i = 0; i < KYBER_N; i++)
  {
    r->coeffs[i] = FsmSw_Kyber_BarrettReduce(r->coeffs[i]);
  }
} // end: FsmSw_Kyber_Poly_Reduce

/*====================================================================================================================*/
/**
* \brief Add two polynomials; no modular reduction is performed
*
* \param[out] poly       *r : pointer to output polynomial
* \param[in]  const poly *a : pointer to first input polynomial
* \param[in]  const poly *b : pointer to second input polynomial
*/
void FsmSw_Kyber_Poly_Add(poly *r, const poly *a, const poly *b)
{
  uint16 i = 0;

  for (i = 0; i < KYBER_N; i++)
  {
    r->coeffs[i] = a->coeffs[i] + b->coeffs[i];
  }
} // end: FsmSw_Kyber_Poly_Add

/*====================================================================================================================*/
/**
* \brief Subtract two polynomials; no modular reduction is performed
*
* \param[out] poly       *r : pointer to output polynomial
* \param[in]  const poly *a : pointer to first input polynomial
* \param[in]  const poly *b : pointer to second input polynomial
*/
void FsmSw_Kyber_Poly_Sub(poly *r, const poly *a, const poly *b)
{
  uint16 i = 0;

  for (i = 0; i < KYBER_N; i++)
  {
    r->coeffs[i] = a->coeffs[i] - b->coeffs[i];
  }
} // end: FsmSw_Kyber_Poly_Sub

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */