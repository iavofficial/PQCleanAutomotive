#ifndef FSMSW_SPHINCSSHA2_128SSIMPLE_API_H
#define FSMSW_SPHINCSSHA2_128SSIMPLE_API_H
/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *  \file
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsSha2_128sSimple
*    includes the modules for SphincsSha2_128sSimple
 ** @{ */
/** \addtogroup FsmSw_SphincsSha2_128fSimple_api
 ** @{ */

/***********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"

/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
/* Resulting SPX sizes. */
#define FSMSW_SPHINCSSHA2_128SSIMPLE_BYTES    7856u
#define FSMSW_SPHINCSSHA2_128SSIMPLE_PK_BYTES 32u
#define FSMSW_SPHINCSSHA2_128SSIMPLE_SK_BYTES 64u

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
void FsmSw_SphincsSha2_128sSimple_Crypto_Sign_KeyPair(uint8 *const pk, uint8 *const sk);
void FsmSw_SphincsSha2_128sSimple_Crypto_Sign_Signature(uint8 *const sig, uint32 *const siglen, const uint8 *const m,
                                                        uint32 mlen, const uint8 *const sk);
uint8 FsmSw_SphincsSha2_128sSimple_Crypto_Sign_Verify(const uint8 *const sig, uint32 siglen, const uint8 *const m,
                                                      uint32 mlen, const uint8 *const pk);
void FsmSw_SphincsSha2_128sSimple_Crypto_Sign(uint8 *const sm, uint32 *const smlen, const uint8 *const m, uint32 mlen,
                                              const uint8 *const sk);
uint8 FsmSw_SphincsSha2_128sSimple_Crypto_Sign_Open(uint8 *const m, uint32 *const mlen, const uint8 *const sm,
                                                    uint32 smlen, const uint8 *const pk);
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
#endif /* FSMSW_SPHINCSSHA2_128SSIMPLE_API_H */
