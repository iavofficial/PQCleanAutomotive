/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Dilithium3
*    includes the modules for Dilithium3
 ** @{ */
/** \addtogroup FsmSw_Dilithium3_params
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Dilithium3_params.h
* \brief  Description of the FsmSw_Dilithium3_params.h
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
#ifndef FSMSW_DILITHIUM3_PARAMS_H
#define FSMSW_DILITHIUM3_PARAMS_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Dilithium_params.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define K_DILITHIUM3    6u
#define L_DILITHIUM3    5u
#define ETA_DILITHIUM3  4u
#define TAU_DILITHIUM3  49u
#define BETA_DILITHIUM3 196u
/* (1 << 19) = 524288u */
#define GAMMA1_DILITHIUM3 524288u
/* MISRA check for GAMMA2_DILITHIUM3 is ongoing. You get an error if GAMMA2_DILITHIUM3 get a type cast. */
#define GAMMA2_DILITHIUM3      ((Q_DILITHIUM - 1) / 32)
#define OMEGA_DILITHIUM3       55u
#define CTILDEBYTES_DILITHIUM3 48u

#define POLYVECH_PACKEDBYTES_DILITHIUM3 (OMEGA_DILITHIUM3 + K_DILITHIUM3)

#define POLYZ_PACKEDBYTES_DILITHIUM3   640u
#define POLYW1_PACKEDBYTES_DILITHIUM3  128u
#define POLYETA_PACKEDBYTES_DILITHIUM3 128u

#define FSMSW_DILITHIUM3_CRYPTO_PUBLICKEYBYTES (SEEDBYTES_DILITHIUM + (K_DILITHIUM3 * POLYT1_PACKEDBYTES_DILITHIUM))

#define FSMSW_DILITHIUM3_CRYPTO_SECRETKEYBYTES                                                                         \
  ((2u * SEEDBYTES_DILITHIUM) + TRBYTES_DILITHIUM + (L_DILITHIUM3 * POLYETA_PACKEDBYTES_DILITHIUM3) +                  \
   (K_DILITHIUM3 * POLYETA_PACKEDBYTES_DILITHIUM3) + (K_DILITHIUM3 * POLYT0_PACKEDBYTES_DILITHIUM))

#define FSMSW_DILITHIUM3_CRYPTO_BYTES                                                                                  \
  (CTILDEBYTES_DILITHIUM3 + (L_DILITHIUM3 * POLYZ_PACKEDBYTES_DILITHIUM3) + POLYVECH_PACKEDBYTES_DILITHIUM3)

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
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/

#endif /* FSMSW_DILITHIUM3_PARAMS_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */