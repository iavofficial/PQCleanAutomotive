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
#ifndef FSMSW_SPHINCSSHAKE_192FSIMPLE_CONTEXT_H
#define FSMSW_SPHINCSSHAKE_192FSIMPLE_CONTEXT_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_SphincsShake_192fSimple_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct
{
    uint8 pub_seed[FSMSW_SPHINCSSHAKE_192FSIMPLE_N];
    uint8 sk_seed[FSMSW_SPHINCSSHAKE_192FSIMPLE_N];

} sphincs_shake_192f_ctx;

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_SphincsShake_192fSimple_initialize_hash_function(sphincs_shake_192f_ctx *ctx);

void FsmSw_SphincsShake_192fSimple_free_hash_function(sphincs_shake_192f_ctx *ctx);

#endif /* FSMSW_SPHINCSSHAKE_192FSIMPLE_CONTEXT_H */