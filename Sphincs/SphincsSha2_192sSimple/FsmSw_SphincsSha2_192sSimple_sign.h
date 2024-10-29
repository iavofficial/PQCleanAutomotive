/***********************************************************************************************************************
*
*                                          IAV GmbH
*
***********************************************************************************************************************/
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
***********************************************************************************************************************/
#ifndef FSMSW_SPHINCSSHA2_192SSIMPLE_SIGN_H
#define FSMSW_SPHINCSSHA2_192SSIMPLE_SIGN_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Types.h"

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
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
sint8 FsmSw_SphincsSha2_192sSimple_crypto_sign_keypair(uint8 *pk, uint8 *sk);
sint8 FsmSw_SphincsSha2_192sSimple_crypto_sign_signature(uint8 *sig, uint32 *siglen, const uint8 *m, uint32 mlen,
                                                       const uint8 *sk);
sint8 FsmSw_SphincsSha2_192sSimple_crypto_sign_verify(const uint8 *sig, uint32 siglen, const uint8 *m, uint32 mlen,
                                                    const uint8 *pk);
sint8 FsmSw_SphincsSha2_192sSimple_crypto_sign(uint8 *sm, uint32 *smlen, const uint8 *m, uint32 mlen, const uint8 *sk);
sint8 FsmSw_SphincsSha2_192sSimple_crypto_sign_open(uint8 *m, uint32 *mlen, const uint8 *sm, uint32 smlen,
                                                  const uint8 *pk);

#endif /* FSMSW_SPHINCSSHA2_192SSIMPLE_SIGN_H */
