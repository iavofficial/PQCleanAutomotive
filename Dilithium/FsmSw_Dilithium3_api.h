#ifndef FSMSW_DILITHIUM3_API_H
#define FSMSW_DILITHIUM3_API_H
/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *  \file
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Dilithium3
*    includes the modules for Dilithium3
 ** @{ */
/** \addtogroup FsmSw_Dilithium3_api
 ** @{ */

/***********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"

/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define FSMSW_DILITHIUM3_CRYPTO_PUBLICKEYBYTES 1952U
#define FSMSW_DILITHIUM3_CRYPTO_SECRETKEYBYTES 4032U
#define FSMSW_DILITHIUM3_CRYPTO_BYTES          3309U

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
void FsmSw_Dilithium3_Crypto_Sign_KeyPair(uint8 *const pk, uint8 *const sk);
uint8 FsmSw_Dilithium3_Crypto_Sign_Signature(uint8 *const sig, uint32 *const siglen, const uint8 *const m, uint32 mlen,
                                             const uint8 *const sk);
uint8 FsmSw_Dilithium3_Crypto_Sign(uint8 *const sm, uint32 *const smlen, const uint8 *const m, uint32 mlen,
                                   const uint8 *const sk);
uint8 FsmSw_Dilithium3_Crypto_Sign_Verify(const uint8 *const sig, uint32 siglen, const uint8 *const m, uint32 mlen,
                                          const uint8 *const pk);
uint8 FsmSw_Dilithium3_Crypto_Sign_Open(uint8 *const m, uint32 *const mlen, const uint8 *const sm, uint32 smlen,
                                        const uint8 *const pk);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
#endif /* FSMSW_DILITHIUM3_API_H */
