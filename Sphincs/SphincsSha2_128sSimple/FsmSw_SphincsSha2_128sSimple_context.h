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
#ifndef FSMSW_SPHINCSSHA2_128SSIMPLE_CONTEXT_H
#define FSMSW_SPHINCSSHA2_128SSIMPLE_CONTEXT_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_sha2.h"
#include "FsmSw_SphincsSha2_128sSimple_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct
{
                 uint8 pub_seed[FSMSW_SPHINCSSHA2_128SSIMPLE_N];
                 uint8 sk_seed[FSMSW_SPHINCSSHA2_128SSIMPLE_N];
                 // FsmSw_sha256 state that absorbed pub_seed
                 sha256ctx state_seeded;
} sphincs_sha2_128s_ctx;
/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_SphincsSha2_128sSimple_initialize_hash_function(sphincs_sha2_128s_ctx *ctx);

#endif /* FSMSW_SPHINCSSHA2_128SSIMPLE_CONTEXT_H */
