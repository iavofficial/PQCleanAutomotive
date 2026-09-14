/***********************************************************************************************************************
 *
 * Original implementation: PQClean, ML-DSA
 *
 * Copyright 2026 IAV GmbH
 *
 * Original portions are marked as Public Domain by PQClean.
 * See the NOTICE file in the repository root for the upstream
 * license reference and attribution information.
 * IAV modifications are licensed under the Apache License, Version 2.0.
 *
 * SPDX-License-Identifier: LicenseRef-PQClean-Public-Domain AND Apache-2.0
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup ML_DSA_44
*    includes the modules for ML_DSA_44
 ** @{ */
/** \addtogroup ML_DSA_44_packing
 ** @{ */

/*====================================================================================================================*/
/** \file ML_DSA_44_packing.c
* \brief  description of ML_DSA_44_packing.c
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
#include "ML_DSA_44_params.h"
#include "ML_DSA_44_poly.h"
#include "ML_DSA_44_polyvec.h"
#include "Std_Types.h"

#include "ML_DSA_44_packing.h"
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
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/
/*====================================================================================================================*/
/** 
* \brief Bit-pack public key pk = (rho, t1).
*
* \param[out]  uint8 pk[]             : output byte array
* \param[in]   const uint8 rho[]      : byte array containing rho
* \param[in]   const polyveck_D2 *t1  : pointer to vector t1
*/
void ML_DSA_44_PackPk(uint8 pk[FSMSW_ML_DSA_44_CRYPTO_PUBLICKEYBYTES], const uint8 rho[SEEDBYTES_ML_DSA],
                             const polyveck_D2 *const t1)
{
  uint16 i = 0;
  /* pk_temp is used to avoid modifying the input. */
  uint8 *pk_temp = pk;

  for (i = 0; i < SEEDBYTES_ML_DSA; ++i)
  {
    pk_temp[i] = rho[i];
  }
  pk_temp = &pk_temp[SEEDBYTES_ML_DSA];

  for (i = 0; i < K_ML_DSA_44; ++i)
  {
    ML_DSA_44_Poly_T1Pack(&pk_temp[i * POLYT1_PACKEDBYTES_ML_DSA], &t1->vec[i]);
  }
} // end: ML_DSA_44_PackPk
/*====================================================================================================================*/
/** 
* \brief Unpack public key pk = (rho, t1).
*
* \param[out] uint8      rho[] : output byte array for rho
* \param[out] polyveck_D2  *t1 : pointer to output vector t1
* \param[in]  const uint8 pk[] : byte array containing bit-packed pk
*/
void ML_DSA_44_UnpackPk(uint8 rho[SEEDBYTES_ML_DSA], polyveck_D2 *t1,
                               const uint8 pk[FSMSW_ML_DSA_44_CRYPTO_PUBLICKEYBYTES])
{
  uint16 i = 0;
  /* pk_temp is used to avoid modifying the input. */
  const uint8 *pk_temp = pk;

  for (i = 0; i < SEEDBYTES_ML_DSA; ++i)
  {
    rho[i] = pk_temp[i];
  }
  pk_temp = &pk_temp[SEEDBYTES_ML_DSA];

  for (i = 0; i < K_ML_DSA_44; ++i)
  {
    ML_DSA_44_Poly_T1Unpack(&t1->vec[i], &pk_temp[i * POLYT1_PACKEDBYTES_ML_DSA]);
  }
} // end: ML_DSA_44_UnpackPk
/*====================================================================================================================*/
/** 
* \brief Bit-pack secret key sk = (rho, tr, key, t0, s1, s2).
*
* \param[out] uint8            sk[] : output byte array
* \param[in]  const uint8     rho[] : byte array containing rho
* \param[in]  const uint8      tr[] : byte array containing tr
* \param[in]  const uint8     key[] : byte array containing key
* \param[in]  const polyveck_D2 *t0 : pointer to vector t0
* \param[in]  const polyvecl_D2 *s1 : pointer to vector s1
* \param[in]  const polyveck_D2 *s2 : pointer to vector s2
*/
void ML_DSA_44_PackSk(uint8 sk[FSMSW_ML_DSA_44_CRYPTO_SECRETKEYBYTES], const uint8 rho[SEEDBYTES_ML_DSA],
                             const uint8 tr[TRBYTES_ML_DSA], const uint8 key[SEEDBYTES_ML_DSA],
                             const polyveck_D2 *const t0, const polyvecl_D2 *const s1, const polyveck_D2 *const s2)
{
  uint16 i = 0;
  /* sk_temp is used to avoid modifying the input. */
  uint8 *sk_temp = sk;

  for (i = 0; i < SEEDBYTES_ML_DSA; ++i)
  {
    sk_temp[i] = rho[i];
  }
  sk_temp = &sk_temp[SEEDBYTES_ML_DSA];

  for (i = 0; i < SEEDBYTES_ML_DSA; ++i)
  {
    sk_temp[i] = key[i];
  }
  sk_temp = &sk_temp[SEEDBYTES_ML_DSA];

  for (i = 0; i < TRBYTES_ML_DSA; ++i)
  {
    sk_temp[i] = tr[i];
  }
  sk_temp = &sk_temp[TRBYTES_ML_DSA];

  for (i = 0; i < L_ML_DSA_44; ++i)
  {
    ML_DSA_44_Polyeta_EtaPack(&sk_temp[i * POLYETA_PACKEDBYTES_ML_DSA_44], &s1->vec[i]);
  }
  sk_temp = &sk_temp[L_ML_DSA_44 * POLYETA_PACKEDBYTES_ML_DSA_44];

  for (i = 0; i < K_ML_DSA_44; ++i)
  {
    ML_DSA_44_Polyeta_EtaPack(&sk_temp[i * POLYETA_PACKEDBYTES_ML_DSA_44], &s2->vec[i]);
  }
  sk_temp = &sk_temp[K_ML_DSA_44 * POLYETA_PACKEDBYTES_ML_DSA_44];

  for (i = 0; i < K_ML_DSA_44; ++i)
  {
    ML_DSA_44_Poly_T0Pack(&sk_temp[i * POLYT0_PACKEDBYTES_ML_DSA], &t0->vec[i]);
  }
} // end: ML_DSA_44_PackSk
/*====================================================================================================================*/
/** 
* Description: Unpack secret key sk = (rho, tr, key, t0, s1, s2).
*
* \param[out] uint8      rho[] : output byte array for rho
* \param[out] uint8       tr[] : output byte array for tr
* \param[out] uint8      key[] : output byte array for key
* \param[out] polyveck_D2  *t0 : pointer to output vector t0
* \param[out] polyvecl_D2  *s1 : pointer to output vector s1
* \param[out] polyveck_D2  *s2 : pointer to output vector s2
* \param[in]  const uint8 sk[] : byte array containing bit-packed sk
*/
void ML_DSA_44_UnpackSk(uint8 rho[SEEDBYTES_ML_DSA], uint8 tr[TRBYTES_ML_DSA],
                               uint8 key[SEEDBYTES_ML_DSA], polyveck_D2 *t0, polyvecl_D2 *s1, polyveck_D2 *s2,
                               const uint8 sk[FSMSW_ML_DSA_44_CRYPTO_SECRETKEYBYTES])
{
  uint16 i = 0;
  /* pk_temp is used to avoid modifying the input. */
  const uint8 *sk_temp = sk;

  for (i = 0; i < SEEDBYTES_ML_DSA; ++i)
  {
    rho[i] = sk_temp[i];
  }
  sk_temp = &sk_temp[SEEDBYTES_ML_DSA];

  for (i = 0; i < SEEDBYTES_ML_DSA; ++i)
  {
    key[i] = sk_temp[i];
  }
  sk_temp = &sk_temp[SEEDBYTES_ML_DSA];

  for (i = 0; i < TRBYTES_ML_DSA; ++i)
  {
    tr[i] = sk_temp[i];
  }
  sk_temp = &sk_temp[TRBYTES_ML_DSA];

  for (i = 0; i < L_ML_DSA_44; ++i)
  {
    ML_DSA_44_Polyeta_EtaUnpack(&s1->vec[i], &sk_temp[i * POLYETA_PACKEDBYTES_ML_DSA_44]);
  }
  sk_temp = &sk_temp[L_ML_DSA_44 * POLYETA_PACKEDBYTES_ML_DSA_44];

  for (i = 0; i < K_ML_DSA_44; ++i)
  {
    ML_DSA_44_Polyeta_EtaUnpack(&s2->vec[i], &sk_temp[i * POLYETA_PACKEDBYTES_ML_DSA_44]);
  }
  sk_temp = &sk_temp[K_ML_DSA_44 * POLYETA_PACKEDBYTES_ML_DSA_44];

  for (i = 0; i < K_ML_DSA_44; ++i)
  {
    ML_DSA_44_Poly_T0Unpack(&t0->vec[i], &sk_temp[i * POLYT0_PACKEDBYTES_ML_DSA]);
  }
} // end: ML_DSA_44_UnpackSk
/*====================================================================================================================*/
/** 
* \brief Bit-pack signature sig = (c, z, h).
*
* \param[out] uint8          sig[] : output byte array
* \param[in]  const uint8       *c : pointer to hash length SEEDBYTES_ML_DSA
* \param[in]  const polyvecl_D2 *z : pointer to vector z
* \param[in]  const polyveck_D2 *h : pointer to hint vector h
*/
void ML_DSA_44_PackSig(uint8 sig[FSMSW_ML_DSA_44_CRYPTO_BYTES], const uint8 c[CTILDEBYTES_ML_DSA_44],
                              const polyvecl_D2 *const z, const polyveck_D2 *const h)
{
  uint16 i = 0;
  uint16 j = 0;
  uint16 k = 0;
  /* sig_temp is used to avoid modifying the input. */
  uint8 *sig_temp = sig;

  for (i = 0; i < CTILDEBYTES_ML_DSA_44; ++i)
  {
    sig_temp[i] = c[i];
  }
  sig_temp = &sig_temp[CTILDEBYTES_ML_DSA_44];

  for (i = 0; i < L_ML_DSA_44; ++i)
  {
    ML_DSA_44_Poly_ZPack(&sig_temp[i * POLYZ_PACKEDBYTES_ML_DSA_44], &z->vec[i]);
  }
  sig_temp = &sig_temp[L_ML_DSA_44 * POLYZ_PACKEDBYTES_ML_DSA_44];

  /* Encode h */
  for (i = 0; i < (OMEGA_ML_DSA_44 + K_ML_DSA_44); ++i)
  {
    sig_temp[i] = 0;
  }

  k = 0;
  for (i = 0; i < K_ML_DSA_44; ++i)
  {
    for (j = 0; j < N_ML_DSA; ++j)
    {
      if (h->vec[i].coeffs[j] != 0)
      {
        sig_temp[k] = (uint8)j;
        k++;
      }
    }

    sig_temp[OMEGA_ML_DSA_44 + i] = (uint8)k;
  }
} // end: ML_DSA_44_PackSig
/*====================================================================================================================*/
/** 
* \brief Unpack signature sig = (c, z, h).
*
* \param[out]  uint8          *c : pointer to output hash
* \param[in]   polyvecl_D2    *z : pointer to output vector z
* \param[in]   polyveck_D2    *h : pointer to output hint vector h
* \param[in]   const uint8 sig[] : byte array containing bit-packed signature
*
* Returns 1 in case of malformed signature; otherwise 0.
*/
sint8 ML_DSA_44_UnpackSig(uint8 c[CTILDEBYTES_ML_DSA_44], polyvecl_D2 *z, polyveck_D2 *const h,
                                 const uint8 sig[FSMSW_ML_DSA_44_CRYPTO_BYTES])
{
  uint16 i     = 0;
  uint16 j     = 0;
  uint16 k     = 0;
  sint8 retVal = 0;
  /* sig_temp is used to avoid modifying the input. */
  const uint8 *sig_temp = sig;

  for (i = 0; i < CTILDEBYTES_ML_DSA_44; ++i)
  {
    c[i] = sig_temp[i];
  }
  sig_temp = &sig_temp[CTILDEBYTES_ML_DSA_44];

  for (i = 0; i < L_ML_DSA_44; ++i)
  {
    ML_DSA_44_Poly_ZUnpack(&z->vec[i], &sig_temp[i * POLYZ_PACKEDBYTES_ML_DSA_44]);
  }
  sig_temp = &sig_temp[L_ML_DSA_44 * POLYZ_PACKEDBYTES_ML_DSA_44];

  /* Decode h */
  k = 0;
  for (i = 0; i < K_ML_DSA_44; ++i)
  {
    for (j = 0; j < N_ML_DSA; ++j)
    {
      h->vec[i].coeffs[j] = 0;
    }

    if ((sig_temp[OMEGA_ML_DSA_44 + i] < k) || (sig_temp[OMEGA_ML_DSA_44 + i] > OMEGA_ML_DSA_44))
    {
      retVal = 1;
    }

    for (j = k; j < sig_temp[OMEGA_ML_DSA_44 + i]; ++j)
    {
      /* Coefficients are ordered for strong unforgeability */
      if ((j > k) && (sig_temp[j] <= sig_temp[j - 1u]))
      {
        retVal = 1;
      }
      h->vec[i].coeffs[sig_temp[j]] = 1;
    }

    k = sig_temp[OMEGA_ML_DSA_44 + i];
  }

  /* Extra indices are zero for strong unforgeability */
  for (j = k; j < OMEGA_ML_DSA_44; ++j)
  {
    if (0u < sig_temp[j])
    {
      retVal = 1;
    }
  }

  return retVal;
} // end: ML_DSA_44_UnpackSig

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
