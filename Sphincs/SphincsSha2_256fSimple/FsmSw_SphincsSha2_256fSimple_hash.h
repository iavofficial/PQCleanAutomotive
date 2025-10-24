/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsSha2_256fSimple
*    includes the modules for SphincsSha2_256fSimple
 ** @{ */
/** \addtogroup SphincsSha2_256fSimple_hash 
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsSha2_256fSimple_hash.h
* \brief  Description of FsmSw_SphincsSha2_256fSimple_hash.h
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
#ifndef FSMSW_SPHINCSSHA2_256FSIMPLE_HASH_H
#define FSMSW_SPHINCSSHA2_256FSIMPLE_HASH_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_SphincsSha2_256fSimple_context.h"
#include "FsmSw_SphincsSha2_256fSimple_params.h"
#include "FsmSw_Types.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define SPX_SHA256_ADDR_BYTES 22u
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
void FsmSw_SphincsSha2_256fSimple_PrfAddr(uint8 *out, const sphincs_sha2_256f_ctx *ctx, const uint32 addr[8]);

void FsmSw_SphincsSha2_256fSimple_GenMessageRandom(uint8 *R, const uint8 *sk_prf, const uint8 *optrand, const uint8 *m,
                                                   uint32 mlen, const sphincs_sha2_256f_ctx *ctx);

void FsmSw_SphincsSha2_256fSimple_HashMessage(uint8 *digest, uint64 *tree, uint32 *leaf_idx, const uint8 *R,
                                              const uint8 *pk, const uint8 *m, uint32 mlen,
                                              const sphincs_sha2_256f_ctx *ctx);

void FsmSw_SphincsSha2_256fSimple_MgF1256(uint8 *out, uint32 outlen, const uint8 *in, uint32 inlen);

/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsSha2_256fSimple_MgF1512(uint8 *out, uint32 outlen, const uint8 *in, uint32 inlen);

#endif /* FSMSW_SPHINCSSHA2_256FSIMPLE_HASH_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */