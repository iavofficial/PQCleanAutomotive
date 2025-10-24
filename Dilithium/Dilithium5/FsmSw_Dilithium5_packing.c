/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Dilithium5
*    includes the modules for Dilithium5
 ** @{ */
/** \addtogroup FsmSw_Dilithium5_packing
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Dilithium5_packing.c
* \brief  description of FsmSw_Dilithium5_packing.c
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
#include "FsmSw_Dilithium5_params.h"
#include "FsmSw_Dilithium5_poly.h"
#include "FsmSw_Dilithium5_polyvec.h"
#include "FsmSw_Types.h"

#include "FsmSw_Dilithium5_packing.h"
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
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/*====================================================================================================================*/
/**
 * \brief Bit-pack public key pk = (rho, t1).
 *
 * \param[out] uint8            pk[] : output byte array
 * \param[in]  const uint8     rho[] : byte array containing rho
 * \param[in]  const polyveck_D5 *t1 : pointer to vector t1
 */
void FsmSw_Dilithium5_PackPk(uint8 pk[FSMSW_DILITHIUM5_CRYPTO_PUBLICKEYBYTES], const uint8 rho[SEEDBYTES_DILITHIUM],
                             const polyveck_D5 *const t1)
{
  uint16 i = 0;
  /* pk_temp is used to avoid modifying the input. */
  uint8 *pk_temp = pk;

  for (i = 0; i < SEEDBYTES_DILITHIUM; ++i)
  {
    pk_temp[i] = rho[i];
  }
  pk_temp = &pk_temp[SEEDBYTES_DILITHIUM];

  for (i = 0; i < K_DILITHIUM5; ++i)
  {
    FsmSw_Dilithium5_Poly_T1Pack(&pk_temp[i * POLYT1_PACKEDBYTES_DILITHIUM], &t1->vec[i]);
  }
} // end: FsmSw_Dilithium5_PackPk
/*====================================================================================================================*/
/**
 * \brief Unpack public key pk = (rho, t1).
 *
 * \param[out] uint8         rho[] : output byte array for rho
 * \param[out] polyveck_D5     *t1 : pointer to output vector t1
 * \param[in,out] const uint8 pk[] : byte array containing bit-packed pk
 */
void FsmSw_Dilithium5_UnpackPk(uint8 rho[SEEDBYTES_DILITHIUM], polyveck_D5 *t1,
                               const uint8 pk[FSMSW_DILITHIUM5_CRYPTO_PUBLICKEYBYTES])
{
  uint16 i = 0;
  /* pk_temp is used to avoid modifying the input. */
  const uint8 *pk_temp = pk;

  for (i = 0; i < SEEDBYTES_DILITHIUM; ++i)
  {
    rho[i] = pk_temp[i];
  }
  pk_temp = &pk_temp[SEEDBYTES_DILITHIUM];

  for (i = 0; i < K_DILITHIUM5; ++i)
  {
    FsmSw_Dilithium5_Poly_T1Unpack(&t1->vec[i], &pk_temp[i * POLYT1_PACKEDBYTES_DILITHIUM]);
  }
} // end: FsmSw_Dilithium5_UnpackPk
/*====================================================================================================================*/
/**
 * \brief Bit-pack secret key sk = (rho, tr, key, t0, s1, s2).
 *
 * \param[out] uint8            sk[] : output byte array
 * \param[in]  const uint8     rho[] : byte array containing rho
 * \param[in]  const uint8      tr[] : byte array containing tr
 * \param[in]  const uint8     key[] : byte array containing key
 * \param[in]  const polyveck_D5 *t0 : pointer to vector t0
 * \param[in]  const polyvecl_D5 *s1 : pointer to vector s1
 * \param[in]  const polyveck_D5 *s2 : pointer to vector s2
 */
void FsmSw_Dilithium5_PackSk(uint8 sk[FSMSW_DILITHIUM5_CRYPTO_SECRETKEYBYTES], const uint8 rho[SEEDBYTES_DILITHIUM],
                             const uint8 tr[TRBYTES_DILITHIUM], const uint8 key[SEEDBYTES_DILITHIUM],
                             const polyveck_D5 *const t0, const polyvecl_D5 *const s1, const polyveck_D5 *const s2)
{
  uint16 i = 0;
  /* sk_temp is used to avoid modifying the input. */
  uint8 *sk_temp = sk;

  for (i = 0; i < SEEDBYTES_DILITHIUM; ++i)
  {
    sk_temp[i] = rho[i];
  }
  sk_temp = &sk_temp[SEEDBYTES_DILITHIUM];

  for (i = 0; i < SEEDBYTES_DILITHIUM; ++i)
  {
    sk_temp[i] = key[i];
  }
  sk_temp = &sk_temp[SEEDBYTES_DILITHIUM];

  for (i = 0; i < TRBYTES_DILITHIUM; ++i)
  {
    sk_temp[i] = tr[i];
  }
  sk_temp = &sk_temp[TRBYTES_DILITHIUM];

  for (i = 0; i < L_DILITHIUM5; ++i)
  {
    FsmSw_Dilithium5_Polyeta_EtaPack(&sk_temp[i * POLYETA_PACKEDBYTES_DILITHIUM5], &s1->vec[i]);
  }
  sk_temp = &sk_temp[L_DILITHIUM5 * POLYETA_PACKEDBYTES_DILITHIUM5];

  for (i = 0; i < K_DILITHIUM5; ++i)
  {
    FsmSw_Dilithium5_Polyeta_EtaPack(&sk_temp[i * POLYETA_PACKEDBYTES_DILITHIUM5], &s2->vec[i]);
  }
  sk_temp = &sk_temp[K_DILITHIUM5 * POLYETA_PACKEDBYTES_DILITHIUM5];

  for (i = 0; i < K_DILITHIUM5; ++i)
  {
    FsmSw_Dilithium5_Poly_T0Pack(&sk_temp[i * POLYT0_PACKEDBYTES_DILITHIUM], &t0->vec[i]);
  }
} // end: FsmSw_Dilithium5_PackSk
/*====================================================================================================================*/
/**
 * \brief Unpack secret key sk = (rho, tr, key, t0, s1, s2).
 *
 * \param[out] uint8      rho[] : output byte array for rho
 * \param[out] uint8       tr[] : output byte array for tr
 * \param[out] uint8      key[] : output byte array for key
 * \param[out] polyveck_D5  *t0 : pointer to output vector t0
 * \param[out] polyveck_D5  *s2 : pointer to output vector s2
 * \param[out] polyvecl_D5  *s1 : pointer to output vector s1
 * \param[in]  const uint8 sk[] : byte array containing bit-packed sk
 */
void FsmSw_Dilithium5_UnpackSk(uint8 rho[SEEDBYTES_DILITHIUM], uint8 tr[TRBYTES_DILITHIUM],
                               uint8 key[SEEDBYTES_DILITHIUM], polyveck_D5 *t0, polyvecl_D5 *s1, polyveck_D5 *s2,
                               const uint8 sk[FSMSW_DILITHIUM5_CRYPTO_SECRETKEYBYTES])
{
  uint16 i = 0;
  /* sk_temp is used to avoid modifying the input. */
  const uint8 *sk_temp = sk;

  for (i = 0; i < SEEDBYTES_DILITHIUM; ++i)
  {
    rho[i] = sk_temp[i];
  }
  sk_temp = &sk_temp[SEEDBYTES_DILITHIUM];

  for (i = 0; i < SEEDBYTES_DILITHIUM; ++i)
  {
    key[i] = sk_temp[i];
  }
  sk_temp = &sk_temp[SEEDBYTES_DILITHIUM];

  for (i = 0; i < TRBYTES_DILITHIUM; ++i)
  {
    tr[i] = sk_temp[i];
  }
  sk_temp = &sk_temp[TRBYTES_DILITHIUM];

  for (i = 0; i < L_DILITHIUM5; ++i)
  {
    FsmSw_Dilithium5_Polyeta_EtaUnpack(&s1->vec[i], &sk_temp[i * POLYETA_PACKEDBYTES_DILITHIUM5]);
  }
  sk_temp = &sk_temp[L_DILITHIUM5 * POLYETA_PACKEDBYTES_DILITHIUM5];

  for (i = 0; i < K_DILITHIUM5; ++i)
  {
    FsmSw_Dilithium5_Polyeta_EtaUnpack(&s2->vec[i], &sk_temp[i * POLYETA_PACKEDBYTES_DILITHIUM5]);
  }
  sk_temp = &sk_temp[K_DILITHIUM5 * POLYETA_PACKEDBYTES_DILITHIUM5];

  for (i = 0; i < K_DILITHIUM5; ++i)
  {
    FsmSw_Dilithium5_Poly_T0Unpack(&t0->vec[i], &sk_temp[i * POLYT0_PACKEDBYTES_DILITHIUM]);
  }
} // end: FsmSw_Dilithium5_UnpackSk
/*====================================================================================================================*/
/**
 * Description: Bit-pack signature sig = (c, z, h).
 *
 * \param[out] uint8           sig[] : output byte array
 * \param[in]  const  uint8       *c : pointer to FsmSw_Dilithium5_Challenge hash length SEEDBYTES_DILITHIUM
 * \param[in]  const  polyvecl_D5 *z : pointer to vector z
 * \param[in]  const  polyveck_D5 *h : pointer to hint vector h
 */
void FsmSw_Dilithium5_PackSig(uint8 sig[FSMSW_DILITHIUM5_CRYPTO_BYTES], const uint8 c[CTILDEBYTES_DILITHIUM5],
                              const polyvecl_D5 *const z, const polyveck_D5 *const h)
{
  uint16 i = 0;
  uint16 j = 0;
  uint16 k = 0;
  /* sig_temp is used to avoid modifying the input. */
  uint8 *sig_temp = sig;

  for (i = 0; i < CTILDEBYTES_DILITHIUM5; ++i)
  {
    sig_temp[i] = c[i];
  }
  sig_temp = &sig_temp[CTILDEBYTES_DILITHIUM5];

  for (i = 0; i < L_DILITHIUM5; ++i)
  {
    FsmSw_Dilithium5_Poly_ZPack(&sig_temp[i * POLYZ_PACKEDBYTES_DILITHIUM5], &z->vec[i]);
  }
  sig_temp = &sig_temp[L_DILITHIUM5 * POLYZ_PACKEDBYTES_DILITHIUM5];

  /* Encode h */
  for (i = 0; i < (OMEGA_DILITHIUM5 + K_DILITHIUM5); ++i)
  {
    sig_temp[i] = 0;
  }

  k = 0;
  for (i = 0; i < K_DILITHIUM5; ++i)
  {
    for (j = 0; j < N_DILITHIUM; ++j)
    {
      if (h->vec[i].coeffs[j] != 0)
      {
        sig_temp[k] = (uint8)j;
        k++;
      }
    }

    sig_temp[OMEGA_DILITHIUM5 + i] = (uint8)k;
  }
} // end: FsmSw_Dilithium5_PackSig
/*====================================================================================================================*/
/**
 * \brief Unpack signature sig = (c, z, h).
 *
 * \param[out] uint8          *c : pointer to output FsmSw_Dilithium5_Challenge hash
 * \param[out] polyvecl_D5    *z : pointer to output vector z
 * \param[out] polyveck_D5    *h : pointer to output hint vector h
 * \param[in]  const uint8 sig[] : byte array containing bit-packed signature
 *
 * \returns 1 in case of malformed signature; otherwise 0.
 */
sint8 FsmSw_Dilithium5_UnpackSig(uint8 c[CTILDEBYTES_DILITHIUM5], polyvecl_D5 *z, polyveck_D5 *const h,
                                 const uint8 sig[FSMSW_DILITHIUM5_CRYPTO_BYTES])
{
  uint16 i     = 0;
  uint16 j     = 0;
  uint16 k     = 0;
  sint8 retVal = 0;

  /* sig_temp is used to avoid modifying the input. */
  const uint8 *sig_temp = sig;

  for (i = 0; i < CTILDEBYTES_DILITHIUM5; ++i)
  {
    c[i] = sig_temp[i];
  }
  sig_temp = &sig_temp[CTILDEBYTES_DILITHIUM5];

  for (i = 0; i < L_DILITHIUM5; ++i)
  {
    FsmSw_Dilithium5_Poly_ZUnpack(&z->vec[i], &sig_temp[i * POLYZ_PACKEDBYTES_DILITHIUM5]);
  }
  sig_temp = &sig_temp[L_DILITHIUM5 * POLYZ_PACKEDBYTES_DILITHIUM5];

  /* Decode h */
  k = 0;
  for (i = 0; i < K_DILITHIUM5; ++i)
  {
    for (j = 0; j < N_DILITHIUM; ++j)
    {
      h->vec[i].coeffs[j] = 0;
    }

    if ((sig_temp[OMEGA_DILITHIUM5 + i] < k) || (sig_temp[OMEGA_DILITHIUM5 + i] > OMEGA_DILITHIUM5))
    {
      retVal = 1;
    }

    for (j = k; j < sig_temp[OMEGA_DILITHIUM5 + i]; ++j)
    {
      /* Coefficients are ordered for strong unforgeability */
      if ((j > k) && (sig_temp[j] <= sig_temp[j - 1u]))
      {
        retVal = 1;
      }
      h->vec[i].coeffs[sig_temp[j]] = 1;
    }

    k = sig_temp[OMEGA_DILITHIUM5 + i];
  }

  /* Extra indices are zero for strong unforgeability */
  for (j = k; j < OMEGA_DILITHIUM5; ++j)
  {
    if (0u < sig_temp[j])
    {
      retVal = 1;
    }
  }

  return retVal;
} // end: FsmSw_Dilithium5_UnpackSig

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */