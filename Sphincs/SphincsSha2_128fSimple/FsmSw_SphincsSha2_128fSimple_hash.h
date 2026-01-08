/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsSha2_128fSimple
*    includes the modules for SphincsSha2_128fSimple
 ** @{ */
/** \addtogroup SphincsSha2_128fSimple_hash 
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsSha2_128fSimple_hash.h
* \brief  Description of FsmSw_SphincsSha2_128fSimple_hash.h
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
#ifndef FSMSW_SPHINCSSHA2_128FSIMPLE_HASH_H
#define FSMSW_SPHINCSSHA2_128FSIMPLE_HASH_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_SphincsSha2_128fSimple_context.h"
#include "FsmSw_SphincsSha2_128fSimple_params.h"
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
void FsmSw_SphincsSha2_128fSimple_PrfAddr(uint8 *const out, const sphincs_sha2_128f_ctx *const ctx,
                                          const uint32 addr[8]);

void FsmSw_SphincsSha2_128fSimple_GenMessageRandom(uint8 *const R, const uint8 *const sk_prf,
                                                   const uint8 *const optrand, const uint8 *const m, uint32 mlen,
                                                   const sphincs_sha2_128f_ctx *const ctx);

void FsmSw_SphincsSha2_128fSimple_HashMessage(uint8 *const digest, uint64 *const tree, uint32 *const leaf_idx,
                                              const uint8 *const R, const uint8 *const pk, const uint8 *const m,
                                              uint32 mlen, const sphincs_sha2_128f_ctx *const ctx);

void FsmSw_SphincsSha2_128fSimple_Mgf1512(uint8 *const out, uint32 outlen, const uint8 *const in, uint32 inlen);

#endif /* FSMSW_SPHINCSSHA2_128FSIMPLE_HASH_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */