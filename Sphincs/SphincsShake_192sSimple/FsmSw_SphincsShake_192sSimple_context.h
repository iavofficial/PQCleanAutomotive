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
#ifndef FSMSW_SPHINCSSHAKE_192SSIMPLE_CONTEXT_H
#define FSMSW_SPHINCSSHAKE_192SSIMPLE_CONTEXT_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_SphincsShake_192sSimple_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct
{
    uint8 pub_seed[FSMSW_SPHINCSSHAKE_192SSIMPLE_N];
    uint8 sk_seed[FSMSW_SPHINCSSHAKE_192SSIMPLE_N];

} sphincs_shake_192s_ctx;

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_SphincsShake_192sSimple_initialize_hash_function(sphincs_shake_192s_ctx *ctx);

void FsmSw_SphincsShake_192sSimple_free_hash_function(sphincs_shake_192s_ctx *ctx);

#endif /* FSMSW_SPHINCSSHAKE_192SSIMPLE_CONTEXT_H */
