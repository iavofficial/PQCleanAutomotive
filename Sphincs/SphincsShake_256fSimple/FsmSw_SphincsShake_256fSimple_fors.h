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
#ifndef FSMSW_SPHINCSSHAKE_256FSIMPLE_FORS_H
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_FORS_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Types.h"
#include "FsmSw_SphincsShake_256fSimple_context.h"
#include "FsmSw_SphincsShake_256fSimple_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_SphincsShake_256fSimple_fors_sign(uint8 *sig, uint8 *pk, const uint8 *m, const sphincs_shake_256f_ctx *ctx,
                                             const uint32 fors_addr[8]);

void FsmSw_SphincsShake_256fSimple_fors_pk_from_sig(uint8 *pk, const uint8 *sig, const uint8 *m,
                                                    const sphincs_shake_256f_ctx *ctx, const uint32 fors_addr[8]);

#endif /* FSMSW_SPHINCSSHAKE_256FSIMPLE_FORS_H */
