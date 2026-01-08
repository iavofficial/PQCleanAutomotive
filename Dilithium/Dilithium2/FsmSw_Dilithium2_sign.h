/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Dilithium2
*    includes the modules for Dilithium2
 ** @{ */
/** \addtogroup FsmSw_Dilithium2_sign
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Dilithium2_sign.h
* \brief  Description of the FsmSw_Dilithium2_sign.h
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
#ifndef FSMSW_DILITHIUM2_SIGN_H
#define FSMSW_DILITHIUM2_SIGN_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Dilithium2_params.h"
#include "FsmSw_Dilithium2_poly.h"
#include "FsmSw_Dilithium2_polyvec.h"
#include "FsmSw_Types.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
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
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/

void FsmSw_Dilithium2_Challenge(poly_D2 *c, const uint8 seed[SEEDBYTES_DILITHIUM]);
void FsmSw_Dilithium2_Crypto_Sign_KeyPair(uint8 *const pk, uint8 *const sk);
void FsmSw_Dilithium2_Crypto_Sign_Signature(uint8 *const sig, uint32 *const siglen, const uint8 *const m, uint32 mlen,
                                            const uint8 *const sk);
void FsmSw_Dilithium2_Crypto_Sign(uint8 *const sm, uint32 *const smlen, const uint8 *const m, uint32 mlen,
                                  const uint8 *const sk);
uint8 FsmSw_Dilithium2_Crypto_Sign_Verify(const uint8 *const sig, uint32 siglen, const uint8 *const m, uint32 mlen,
                                          const uint8 *const pk);
uint8 FsmSw_Dilithium2_Crypto_Sign_Open(uint8 *const m, uint32 *const mlen, const uint8 *const sm, uint32 smlen,
                                        const uint8 *const pk);

#endif /* FSMSW_DILITHIUM2_SIGN_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */