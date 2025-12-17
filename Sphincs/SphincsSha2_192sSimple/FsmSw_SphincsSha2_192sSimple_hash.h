/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsSha2_192sSimple
*    includes the modules for SphincsSha2_192sSimple
 ** @{ */
/** \addtogroup SphincsSha2_192sSimple_hash 
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsSha2_192sSimple_hash.h
* \brief  Description of FsmSw_SphincsSha2_192sSimple_hash.h
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
#ifndef FSMSW_SPHINCSSHA2_192SSIMPLE_HASH_H
#define FSMSW_SPHINCSSHA2_192SSIMPLE_HASH_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_SphincsSha2_192sSimple_context.h"
#include "FsmSw_SphincsSha2_192sSimple_params.h"
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
void FsmSw_SphincsSha2_192sSimple_PrfAddr(uint8 *const out, const sphincs_sha2_192s_ctx *const ctx,
                                          const uint32 addr[8]);

void FsmSw_SphincsSha2_192sSimple_GenMessageRandom(uint8 *const R, const uint8 *const sk_prf,
                                                   const uint8 *const optrand, const uint8 *const m, uint32 mlen,
                                                   const sphincs_sha2_192s_ctx *const ctx);

void FsmSw_SphincsSha2_192sSimple_HashMessage(uint8 *const digest, uint64 *const tree, uint32 *const leaf_idx,
                                              const uint8 *const R, const uint8 *const pk, const uint8 *const m,
                                              uint32 mlen, const sphincs_sha2_192s_ctx *const ctx);

void FsmSw_SphincsSha2_192sSimple_MgF1256(uint8 *const out, uint32 outlen, const uint8 *const in, uint32 inlen);

/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsSha2_192sSimple_MgF1512(uint8 *const out, uint32 outlen, const uint8 *const in, uint32 inlen);

#endif /* FSMSW_SPHINCSSHA2_192SSIMPLE_HASH_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */