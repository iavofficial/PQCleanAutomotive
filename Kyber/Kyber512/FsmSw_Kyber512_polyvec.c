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
/** \addtogroup Kyber512_polyvec
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Kyber512_polyvec.c
* \brief  description of FsmSw_Kyber512_polyvec.c
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
#include "FsmSw_Kyber512_params.h"
#include "FsmSw_Kyber512_poly.h"
#include "FsmSw_Kyber_poly.h"
#include "FsmSw_Types.h"

#include "FsmSw_Kyber512_polyvec.h"
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
* \brief Compress and serialize vector of polynomials
*
* \param[out] uint8         *r : pointer to output byte array
* \param[in]  const polyvec *a : pointer to input vector of polynomials
*/
void FsmSw_Kyber512_Polyvec_Compress(uint8 r[KYBER512_POLYVECCOMPRESSEDBYTES], const polyvec512 *const a)
{
  uint8 i     = 0;
  uint8 k     = 0;
  uint16 j    = 0;
  uint16 t[4] = {0};

  /* r_temp is used to avoid modifying the input. */
  uint8 *r_temp = r;

  for (i = 0; i < KYBER512_K; i++)
  {
    for (j = 0; j < (KYBER_N / 4u); j++)
    {
      for (k = 0; (k < 4u); k++)
      {
        t[k] = (uint16)(a->vec[i].coeffs[(4u * j) + k]);
        /* Shift to get the first bit */
        if ((t[k] >> 15u) != 0u)
        {
          t[k] = t[k] + KYBER_Q;
        }
        t[k] = (uint16)(((t[k] << 10u) + (KYBER_Q / 2u)) / KYBER_Q) & 0x3ffu;
      }

      r_temp[0] = (uint8)(t[0] >> 0);
      r_temp[1] = (uint8)((t[0] >> 8) | (t[1] << 2));
      r_temp[2] = (uint8)((t[1] >> 6) | (t[2] << 4));
      r_temp[3] = (uint8)((t[2] >> 4) | (t[3] << 6));
      r_temp[4] = (uint8)(t[3] >> 2);
      r_temp    = &(r_temp[5]);
    }
  }
} // end: FsmSw_Kyber512_Polyvec_Compress

/*====================================================================================================================*/
/**
* \brief De-serialize and decompress vector of polynomials;
*        approximate inverse of FsmSw_Kyber512_Polyvec_Compress
*
* \param[out] polyvec512  *r : pointer to output vector of polynomials
* \param[in]  const uint8 *a : pointer to input byte array of length KYBER512_POLYVECCOMPRESSEDBYTES
*/
void FsmSw_Kyber512_Polyvec_Decompress(polyvec512 *const r, const uint8 a[KYBER512_POLYVECCOMPRESSEDBYTES])
{
  uint8 i     = 0;
  uint8 k     = 0;
  uint16 j    = 0;
  uint16 t[4] = {0};

  /* a_temp is used to avoid modifying the input. */
  const uint8 *a_temp = a;

  for (i = 0; i < KYBER512_K; i++)
  {
    for (j = 0; j < (KYBER_N / 4u); j++)
    {
      t[0] = ((uint16)a_temp[0] >> 0) | ((uint16)a_temp[1] << 8);
      t[1] = ((uint16)a_temp[1] >> 2) | ((uint16)a_temp[2] << 6);
      t[2] = ((uint16)a_temp[2] >> 4) | ((uint16)a_temp[3] << 4);
      t[3] = ((uint16)a_temp[3] >> 6) | ((uint16)a_temp[4] << 2);
      /* Set address from pointer a[4] to address a[5] */
      a_temp = &(a_temp[5]);

      for (k = 0; k < 4u; k++)
      {
        r->vec[i].coeffs[(4u * j) + k] = (sint16)((uint16)((((t[k] & 0x3FFu) * KYBER_Q) + 512u) >> 10u));
      }
    }
  }
} // end: FsmSw_Kyber512_Polyvec_Decompress

/*====================================================================================================================*/
/**
* \brief Serialize vector of polynomials
*
* \param[out] uint8            *r : pointer to output byte array  of length KYBER512_POLYVECBYTES
* \param[in]  const polyvec512 *a : pointer to input vector of polynomials
*/
void FsmSw_Kyber512_Polyvec_ToBytes(uint8 r[KYBER512_POLYVECBYTES], const polyvec512 *const a)
{
  uint8 i = 0;

  for (i = 0; i < KYBER512_K; i++)
  {
    FsmSw_Kyber_Poly_ToBytes(&(r[i * KYBER_POLYBYTES]), &a->vec[i]);
  }
} // end: FsmSw_Kyber512_Polyvec_ToBytes

/*====================================================================================================================*/
/**
* \brief De-serialize vector of polynomials;
*        inverse of FsmSw_Kyber512_Polyvec_ToBytes
*
* \param[out] uint8            *r : pointer to output byte array
* \param[in]  const polyvec512 *a : pointer to input vector of polynomials of length KYBER512_POLYVECBYTES
*/
void FsmSw_Kyber512_Polyvec_FromBytes(polyvec512 *r, const uint8 a[KYBER512_POLYVECBYTES])
{
  uint8 i = 0;

  for (i = 0; i < KYBER512_K; i++)
  {
    FsmSw_Kyber_Poly_FromBytes(&r->vec[i], &a[i * KYBER_POLYBYTES]);
  }
} // end: FsmSw_Kyber512_Polyvec_FromBytes

/*====================================================================================================================*/
/**
* \brief Apply forward NTT to all elements of a vector of polynomials
*
* \param[in,out] polyvec512 *r : pointer to in/output vector of polynomials
*/
void FsmSw_Kyber512_Polyvec_Ntt(polyvec512 *r)
{
  uint8 i = 0;

  for (i = 0; i < KYBER512_K; i++)
  {
    FsmSw_Kyber_Poly_Ntt(&r->vec[i]);
  }
} // end: FsmSw_Kyber512_Polyvec_Ntt

/*====================================================================================================================*/
/**
* \brief Apply inverse NTT to all elements of a vector of polynomials
*        and multiply by Montgomery factor 2^16
*
* \param[in,out] polyvec512 *r : pointer to in/output vector of polynomials
*/
void FsmSw_Kyber512_Polyvec_InvnttTomont(polyvec512 *r)
{
  uint8 i = 0;

  for (i = 0; i < KYBER512_K; i++)
  {
    FsmSw_Kyber_Poly_InvnttTomont(&r->vec[i]);
  }
} // end: FsmSw_Kyber512_Polyvec_InvnttTomont

/*====================================================================================================================*/
/**
* \brief Multiply elements of a and b in NTT domain, accumulate into r,
*        and multiply by 2^-16.
*
* \param[out] poly             *r : pointer to output polynomial
* \param[in]  const polyvec512 *a : pointer to first input vector of polynomials
* \param[in]  const polyvec512 *b : pointer to second input vector of polynomials
*/
void FsmSw_Kyber512_Polyvec_BasemulAccMontgomery(poly *const r, const polyvec512 *const a, const polyvec512 *const b)
{
  uint8 i = 0;
  poly t  = {{0}};

  FsmSw_Kyber_Poly_BasemulMontgomery(r, &a->vec[0], &b->vec[0]);

  for (i = 1; i < KYBER512_K; i++)
  {
    FsmSw_Kyber_Poly_BasemulMontgomery(&t, &a->vec[i], &b->vec[i]);
    FsmSw_Kyber_Poly_Add(r, r, &t);
  }

  FsmSw_Kyber_Poly_Reduce(r);
} // end: FsmSw_Kyber512_Polyvec_BasemulAccMontgomery

/*====================================================================================================================*/
/**
* \brief Applies Barrett reduction to each coefficient
*        of each element of a vector of polynomials;
*        for details of the Barrett reduction see comments in reduce.c
*
* \param[in,out] polyvec512 *r : pointer to input/output polynomial
*/
void FsmSw_Kyber512_Polyvec_Reduce(polyvec512 *r)
{
  uint8 i = 0;

  for (i = 0; i < KYBER512_K; i++)
  {
    FsmSw_Kyber_Poly_Reduce(&r->vec[i]);
  }
} // end: FsmSw_Kyber512_Polyvec_Reduce

/*====================================================================================================================*/
/**
* \brief Add vectors of polynomials
*
* \param[out] polyvec512       *r : pointer to output vector of polynomials
* \param[in]  const polyvec512 *a : pointer to first input vector of polynomials
* \param[in]  const polyvec512 *b : pointer to second input vector of polynomials
*/
void FsmSw_Kyber512_Polyvec_Add(polyvec512 *r, const polyvec512 *const a, const polyvec512 *const b)
{
  uint8 i = 0;

  for (i = 0; i < KYBER512_K; i++)
  {
    FsmSw_Kyber_Poly_Add(&r->vec[i], &a->vec[i], &b->vec[i]);
  }
} // end: FsmSw_Kyber512_Polyvec_Add

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */