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
#ifndef FSMSW_SPHINCSSHA2_256SSIMPLE_WOTS_H
#define FSMSW_SPHINCSSHA2_256SSIMPLE_WOTS_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_SphincsSha2_256sSimple_context.h"
#include "FsmSw_SphincsSha2_256sSimple_params.h"

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

/**
 * Takes a WOTS signature and an n-byte message, computes a WOTS public key.
 *
 * Writes the computed public key to 'pk'.
 */
void FsmSw_SphincsSha2_256sSimple_wots_pk_from_sig(uint8 *pk, const uint8 *sig, const uint8 *msg,
                                                   const sphincs_sha2_256s_ctx *ctx, uint32 addr[8]);

/*
 * Compute the chain lengths needed for a given message hash
 */
void FsmSw_SphincsSha2_256sSimple_chain_lengths(uint32 *lengths, const uint8 *msg);

#endif /* FSMSW_SPHINCSSHA2_256SSIMPLE_WOTS_H */
