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
#ifndef FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_H
#define FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_SphincsSha2_256fSimple_context.h"
#include "FsmSw_SphincsSha2_256fSimple_params.h"

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
void FsmSw_SphincsSha2_256fSimple_fors_sign(uint8 *sig, uint8 *pk, const uint8 *m, const sphincs_sha2_256f_ctx *ctx,
                                            const uint32 fors_addr[8]);
void FsmSw_SphincsSha2_256fSimple_fors_pk_from_sig(uint8 *pk, const uint8 *sig, const uint8 *m,
                                                   const sphincs_sha2_256f_ctx *ctx, const uint32 fors_addr[8]);

#endif /* FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_H */
