/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Libs
*    includes the modules for Libs
 ** @{ */
/** \addtogroup sha2
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_sha2.h
* \brief  Declarations for the modul FsmSw_sha2.c
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
#ifndef FSMSW_SHA2_H
#define FSMSW_SHA2_H

/* The incremental API allows hashing of individual input blocks; these blocks must be exactly 64 bytes each.
 * Use the 'finalize' functions for any remaining bytes (possibly over 64). */

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Types.h"

/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define PQC_SHA256CTX_BYTES 40
#define PQC_SHA512CTX_BYTES 72

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
/* polyspace +6 CERT-C:DCL12-C [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* polyspace +4 MISRA2012:D4.8 [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* Structure for the incremental API */
typedef struct
{
  uint8 ctx[PQC_SHA256CTX_BYTES];
} sha224ctx;

/* polyspace +6 CERT-C:DCL12-C [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* polyspace +4 MISRA2012:D4.8 [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* Structure for the incremental API */
typedef struct
{
  uint8 ctx[PQC_SHA256CTX_BYTES];
} sha256ctx;

/* polyspace +6 CERT-C:DCL12-C [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* polyspace +4 MISRA2012:D4.8 [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* Structure for the incremental API */
typedef struct
{
  uint8 ctx[PQC_SHA512CTX_BYTES];
} sha384ctx;

/* polyspace +6 CERT-C:DCL12-C [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* polyspace +4 MISRA2012:D4.8 [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* Structure for the incremental API */
typedef struct
{
  uint8 ctx[PQC_SHA512CTX_BYTES];
} sha512ctx;

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
/* SHA224 API */
void FsmSw_Sha224_IncCtxClone(sha224ctx *stateout, const sha224ctx *const statein);
void FsmSw_Sha224_IncBlocks(sha224ctx *const state, const uint8 *const in, uint32 inblocks);
void FsmSw_Sha224(uint8 *const out, const uint8 *const in, uint32 inlen);

/* SHA256 API */
void FsmSw_Sha256_IncInit(sha256ctx *const state);
void FsmSw_Sha256_IncCtxClone(sha256ctx *stateout, const sha256ctx *const statein);
void FsmSw_Sha256_IncBlocks(sha256ctx *state, const uint8 *const in, uint32 inblocks);
void FsmSw_Sha256_IncFinalize(uint8 *const out, sha256ctx *state, const uint8 *const in, uint32 inlen);
void FsmSw_Sha256(uint8 *const out, const uint8 *const in, uint32 inlen);

/* SHA384 API */
void FsmSw_Sha384_IncCtxClone(sha384ctx *stateout, const sha384ctx *const statein);
void FsmSw_Sha384_IncBlocks(sha384ctx *const state, const uint8 *const in, uint32 inblocks);
void FsmSw_Sha384(uint8 *const out, const uint8 *const in, uint32 inlen);

/* SHA512 API */
void FsmSw_Sha512_IncInit(sha512ctx *const state);
void FsmSw_Sha512_IncCtxClone(sha512ctx *stateout, const sha512ctx *const statein);
void FsmSw_Sha512_IncBlocks(sha512ctx *state, const uint8 *const in, uint32 inblocks);
void FsmSw_Sha512_IncFinalize(uint8 *const out, sha512ctx *state, const uint8 *const in, uint32 inlen);
void FsmSw_Sha512(uint8 *const out, const uint8 *const in, uint32 inlen);

#endif

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */