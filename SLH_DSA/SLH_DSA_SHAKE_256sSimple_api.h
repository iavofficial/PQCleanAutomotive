#ifndef FSMSW_SPHINCSSHAKE_256SSIMPLE_API_H
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_API_H
/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *  \file
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsShake_256sSimple
*    includes the modules for SphincsShake_256sSimple
 ** @{ */
/** \addtogroup FsmSw_SphincsShake_256sSimple_api
 ** @{ */

/***********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"

/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
/* Resulting SPX sizes. */
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_BYTES    29792u
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_PK_BYTES 64u
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_SK_BYTES 128u

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
void FsmSw_SphincsShake_256sSimple_Crypto_Sign_KeyPair(uint8 *const pk, uint8 *const sk);
void FsmSw_SphincsShake_256sSimple_Crypto_Sign_Signature(uint8 *const sig, uint32 *const siglen, const uint8 *const m,
                                                         uint32 mlen, const uint8 *const sk);
uint8 FsmSw_SphincsShake_256sSimple_Crypto_Sign_Verify(const uint8 *const sig, uint32 siglen, const uint8 *const m,
                                                       uint32 mlen, const uint8 *const pk);
void FsmSw_SphincsShake_256sSimple_Crypto_Sign(uint8 *const sm, uint32 *const smlen, const uint8 *const m, uint32 mlen,
                                               const uint8 *const sk);
uint8 FsmSw_SphincsShake_256sSimple_Crypto_Sign_Open(uint8 *const m, uint32 *const mlen, const uint8 *const sm,
                                                     uint32 smlen, const uint8 *const pk);
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
#endif /* FSMSW_SPHINCSSHAKE_256SSIMPLE_API_H */
