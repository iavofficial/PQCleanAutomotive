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
 **********************************************************************************************************************/
#ifndef FSMSW_SPHINCSSHA2_128FSIMPLE_WOTSX1_H
#define FSMSW_SPHINCSSHA2_128FSIMPLE_WOTSX1_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Types.h"
#include "FsmSw_SphincsSha2_128fSimple_context.h"
#include "FsmSw_SphincsSha2_128fSimple_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
struct leaf_info_x1
{
    uint8 *wots_sig;
    uint32 wots_sign_leaf; /* The index of the WOTS we're using to sign */
    uint32 *wots_steps;
    uint32 leaf_addr[8];
    uint32 pk_addr[8];
};
/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/


void FsmSw_SphincsSha2_128fSimple_wots_gen_leafx1(uint8 *dest, const sphincs_sha2_128f_ctx *ctx, uint32 leaf_idx,
                                                  void *v_info);

#endif /* FSMSW_SPHINCSSHA2_128FSIMPLE_WOTSX1_H */
