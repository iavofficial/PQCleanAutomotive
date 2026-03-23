#ifndef FSMSW_DILITHIUM5_API_H
#define FSMSW_DILITHIUM5_API_H
/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *  \file
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Dilithium5
*    includes the modules for Dilithium5
 ** @{ */
/** \addtogroup FsmSw_Dilithium5_api
 ** @{ */

/***********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"

/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define FSMSW_DILITHIUM5_CRYPTO_PUBLICKEYBYTES 2592U
#define FSMSW_DILITHIUM5_CRYPTO_SECRETKEYBYTES 4896U
#define FSMSW_DILITHIUM5_CRYPTO_BYTES          4627U

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
void FsmSw_Dilithium5_Crypto_Sign_KeyPair(uint8 *const pk, uint8 *const sk);
uint8 FsmSw_Dilithium5_Crypto_Sign_Signature(uint8 *const sig, uint32 *const siglen, const uint8 *const m, uint32 mlen,
                                             const uint8 *const sk);
uint8 FsmSw_Dilithium5_Crypto_Sign(uint8 *const sm, uint32 *const smlen, const uint8 *const m, uint32 mlen,
                                   const uint8 *const sk);
uint8 FsmSw_Dilithium5_Crypto_Sign_Verify(const uint8 *const sig, uint32 siglen, const uint8 *const m, uint32 mlen,
                                          const uint8 *const pk);
uint8 FsmSw_Dilithium5_Crypto_Sign_Open(uint8 *const m, uint32 *const mlen, const uint8 *const sm, uint32 smlen,
                                        const uint8 *const pk);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
#endif /* FSMSW_DILITHIUM5_API_H */
