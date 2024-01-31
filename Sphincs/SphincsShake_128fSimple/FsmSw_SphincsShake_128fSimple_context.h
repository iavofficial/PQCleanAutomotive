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
#ifndef FSMSW_SPHINCSSHAKE_128FSIMPLE_CONTEXT_H
#define FSMSW_SPHINCSSHAKE_128FSIMPLE_CONTEXT_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_SphincsShake_128fSimple_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct {
    uint8 pub_seed[FSMSW_SPHINCSSHAKE_128FSIMPLE_N];
    uint8 sk_seed[FSMSW_SPHINCSSHAKE_128FSIMPLE_N];

} sphincs_shake_128f_ctx;

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_SphincsShake_128fSimple_initialize_hash_function(sphincs_shake_128f_ctx *ctx);

void FsmSw_SphincsShake_128fSimple_free_hash_function(sphincs_shake_128f_ctx *ctx);

#endif /* FSMSW_SPHINCSSHAKE_128FSIMPLE_CONTEXT_H */
