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
/** \addtogroup Fips202
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Fips202.h
* \brief  Declarations for the modul FsmSw_Fips202.c
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
#ifndef FSMSW_FIPS202_H
#define FSMSW_FIPS202_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Types.h"

/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define SHAKE128_RATE 168u
#define SHAKE256_RATE 136u
#define SHA3_256_RATE 136u
#define SHA3_384_RATE 104u
#define SHA3_512_RATE 72u

#define PQC_SHAKEINCCTX_LEN   26u
#define PQC_SHAKECTX_LEN      25u
#define PQC_SHAKEINCCTX_BYTES (sizeof(uint64) * 26u)
#define PQC_SHAKECTX_BYTES    (sizeof(uint64) * 25u)

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
/* polyspace +6 CERT-C:DCL12-C [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* polyspace +4 MISRA2012:D4.8 [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* Context for incremental API */
typedef struct
{
  uint64 ctx[PQC_SHAKEINCCTX_LEN];
} shake128incctx;

/* polyspace +6 CERT-C:DCL12-C [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* polyspace +4 MISRA2012:D4.8 [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* Context for non-incremental API */
typedef struct
{
  uint64 ctx[PQC_SHAKECTX_LEN];
} shake128ctx;

/* polyspace +6 CERT-C:DCL12-C [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* polyspace +4 MISRA2012:D4.8 [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* Context for incremental API */
typedef struct
{
  uint64 ctx[PQC_SHAKEINCCTX_LEN];
} shake256incctx;

/* polyspace +6 CERT-C:DCL12-C [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* polyspace +4 MISRA2012:D4.8 [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* Context for non-incremental API */
typedef struct
{
  uint64 ctx[PQC_SHAKECTX_LEN];
} shake256ctx;

/* polyspace +6 CERT-C:DCL12-C [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* polyspace +4 MISRA2012:D4.8 [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* Context for incremental API */
typedef struct
{
  uint64 ctx[PQC_SHAKEINCCTX_LEN];
} sha3_256incctx;

/* polyspace +5 CERT-C:DCL12-C [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* polyspace +3 MISRA2012:D4.8 [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
typedef struct
{
  uint64 ctx[PQC_SHAKEINCCTX_LEN];
} sha3_384incctx;

/* polyspace +5 CERT-C:DCL12-C [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* polyspace +3 MISRA2012:D4.8 [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
typedef struct
{
  uint64 ctx[PQC_SHAKEINCCTX_LEN];
} sha3_512incctx;

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
void FsmSw_Fips202_Shake128_Absorb(shake128ctx *const state, const uint8 *const input, uint32 inlen);
void FsmSw_Fips202_Shake128_SqueezeBlocks(uint8 *const output, uint32 nblocks, shake128ctx *state);
void FsmSw_Fips202_Shake128_CtxClone(shake128ctx *dest, const shake128ctx *const src);
void FsmSw_Fips202_Shake128_IncInit(shake128incctx *state);
void FsmSw_Fips202_Shake128_IncAbsorb(shake128incctx *state, const uint8 *const input, uint32 inlen);
void FsmSw_Fips202_Shake128_IncFinalize(shake128incctx *state);
void FsmSw_Fips202_Shake128_IncSqueeze(uint8 *const output, uint32 outlen, shake128incctx *state);
void FsmSw_Fips202_Shake128_IncCtxClone(shake128incctx *dest, const shake128incctx *const src);

void FsmSw_Fips202_Shake256_CtxClone(shake256ctx *dest, const shake256ctx *const src);
void FsmSw_Fips202_Shake256_IncInit(shake256incctx *state);
void FsmSw_Fips202_Shake256_IncAbsorb(shake256incctx *state, const uint8 *const input, uint32 inlen);
void FsmSw_Fips202_Shake256_IncFinalize(shake256incctx *state);
void FsmSw_Fips202_Shake256_IncSqueeze(uint8 *const output, uint32 outlen, shake256incctx *state);
void FsmSw_Fips202_Shake256_IncCtxClone(shake256incctx *dest, const shake256incctx *const src);
void FsmSw_Fips202_Shake128(uint8 *const output, uint32 outlen, const uint8 *const input, uint32 inlen);
void FsmSw_Fips202_Shake256(uint8 *const output, uint32 outlen, const uint8 *const input, uint32 inlen);
void FsmSw_Fips202_Sha3_256_IncInit(sha3_256incctx *state);
void FsmSw_Fips202_Sha3_256_IncAbsorb(sha3_256incctx *state, const uint8 *const input, uint32 inlen);
void FsmSw_Fips202_Sha3_256_IncFinalize(uint8 *const output, sha3_256incctx *state);
void FsmSw_Fips202_Sha3_256_IncCtxClone(sha3_256incctx *dest, const sha3_256incctx *const src);
void FsmSw_Fips202_Sha3_256(uint8 *const output, const uint8 *const input, uint32 inlen);

void FsmSw_Fips202_Sha3_384_IncInit(sha3_384incctx *state);
void FsmSw_Fips202_Sha3_384_IncAbsorb(sha3_384incctx *state, const uint8 *const input, uint32 inlen);
void FsmSw_Fips202_Sha3_384_IncFinalize(uint8 *const output, sha3_384incctx *state);
void FsmSw_Fips202_Sha3_384_IncCtxClone(sha3_384incctx *dest, const sha3_384incctx *const src);
void FsmSw_Fips202_Sha3_384(uint8 *const output, const uint8 *const input, uint32 inlen);

void FsmSw_Fips202_Sha3_512_IncInit(sha3_512incctx *state);
void FsmSw_Fips202_Sha3_512_IncAbsorb(sha3_512incctx *state, const uint8 *const input, uint32 inlen);
void FsmSw_Fips202_Sha3_512_IncFinalize(uint8 *const output, sha3_512incctx *state);
void FsmSw_Fips202_Sha3_512_IncCtxClone(sha3_512incctx *dest, const sha3_512incctx *const src);
void FsmSw_Fips202_Sha3_512(uint8 *const output, const uint8 *const input, uint32 inlen);

#endif

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */