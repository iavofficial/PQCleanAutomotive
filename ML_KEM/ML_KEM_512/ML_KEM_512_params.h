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
/** \addtogroup Kyber512_params
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Kyber512_params.h
* \brief  description of FsmSw_Kyber512_params.h
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
#ifndef FSMSW_KYBER512_PARAMS_H
#define FSMSW_KYBER512_PARAMS_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Kyber512_api.h"
#include "FsmSw_Kyber_params.h"
#include "FsmSw_StaticAssert.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define KYBER512_K            2u
#define KYBER512_POLYVECBYTES (KYBER512_K * KYBER_POLYBYTES)

#define KYBER512_IMPLBYTES 512u

#define KYBER512_POLYCOMPRESSEDBYTES    128u
#define KYBER512_POLYVECCOMPRESSEDBYTES (KYBER512_K * 320u)

#define KYBER512_ETA1 3u
#define KYBER512_ETA2 2u

#define KYBER512_INDCPA_PUBLICKEYBYTES (KYBER512_POLYVECBYTES + KYBER_SYMBYTES)
#define KYBER512_INDCPA_SECRETKEYBYTES (KYBER512_POLYVECBYTES)

FSMSW_STATIC_ASSERT(KYBER512_PUBLICKEYBYTES == (KYBER512_INDCPA_PUBLICKEYBYTES));
/* 32 bytes of additional space to save H(pk) */
FSMSW_STATIC_ASSERT(KYBER512_SECRETKEYBYTES ==
                    (KYBER512_INDCPA_SECRETKEYBYTES + KYBER512_INDCPA_PUBLICKEYBYTES + (2u * KYBER_SYMBYTES)));
FSMSW_STATIC_ASSERT(KYBER512_INDCPA_BYTES == (KYBER512_POLYVECCOMPRESSEDBYTES + KYBER512_POLYCOMPRESSEDBYTES));
FSMSW_STATIC_ASSERT(KYBER512_CIPHERTEXTBYTES == (KYBER512_INDCPA_BYTES));
FSMSW_STATIC_ASSERT(KYBER512_INDCPA_MSGBYTES == (KYBER_SYMBYTES));

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

#endif /* FSMSW_KYBER512_PARAMS_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */