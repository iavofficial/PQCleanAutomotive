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
#ifndef FSMSW_SPHINCSSHA2_256SSIMPLE_CONTEXT_H
#define FSMSW_SPHINCSSHA2_256SSIMPLE_CONTEXT_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_sha2.h"
#include "FsmSw_SphincsSha2_256sSimple_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct
{
                uint8 pub_seed[FSMSW_SPHINCSSHA2_256SSIMPLE_N];
                uint8 sk_seed[FSMSW_SPHINCSSHA2_256SSIMPLE_N];
                /* sha256 state that absorbed pub_seed */
                sha256ctx state_seeded;
                /* sha512 state that absorbed pub_seed */
                sha512ctx state_seeded_512;
} sphincs_sha2_256s_ctx;

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_SphincsSha2_256sSimple_initialize_hash_function(sphincs_sha2_256s_ctx *ctx);

#endif /* FSMSW_SPHINCSSHA2_256SSIMPLE_CONTEXT_H */
