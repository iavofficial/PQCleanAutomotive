#ifndef FSMSW_SPHINCSSHAKE_128FSIMPLE_API_H
#define FSMSW_SPHINCSSHAKE_128FSIMPLE_API_H
/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *  \file
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsShake_128fSimple
*    includes the modules for SphincsShake_128fSimple
 ** @{ */
/** \addtogroup FsmSw_SphincsShake_128fSimple_api
 ** @{ */

/***********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"

/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
/* Resulting SPX sizes. */
#define FSMSW_SPHINCSSHAKE_128FSIMPLE_BYTES    17088u
#define FSMSW_SPHINCSSHAKE_128FSIMPLE_PK_BYTES 32u
#define FSMSW_SPHINCSSHAKE_128FSIMPLE_SK_BYTES 64u

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
void FsmSw_SphincsShake_128fSimple_Crypto_Sign_KeyPair(uint8 *const pk, uint8 *const sk);
void FsmSw_SphincsShake_128fSimple_Crypto_Sign_Signature(uint8 *const sig, uint32 *const siglen, const uint8 *const m,
                                                         uint32 mlen, const uint8 *const sk);
uint8 FsmSw_SphincsShake_128fSimple_Crypto_Sign_Verify(const uint8 *const sig, uint32 siglen, const uint8 *const m,
                                                       uint32 mlen, const uint8 *const pk);
void FsmSw_SphincsShake_128fSimple_Crypto_Sign(uint8 *const sm, uint32 *const smlen, const uint8 *const m, uint32 mlen,
                                               const uint8 *const sk);
uint8 FsmSw_SphincsShake_128fSimple_Crypto_Sign_Open(uint8 *const m, uint32 *const mlen, const uint8 *const sm,
                                                     uint32 smlen, const uint8 *const pk);
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
#endif /* FSMSW_SPHINCSSHAKE_128FSIMPLE_API_H */
