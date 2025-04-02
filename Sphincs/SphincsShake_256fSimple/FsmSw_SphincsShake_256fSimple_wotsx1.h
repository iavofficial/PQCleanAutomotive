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
#ifndef FSMSW_SPHINCSSHAKE_256FSIMPLE_WOTSX1_H
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_WOTSX1_H

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
typedef struct
{
    uint8 *wots_sig;
    uint32 wots_sign_leaf; /* The index of the WOTS we're using to sign */
    uint32 *wots_steps;
    uint32 leaf_addr[8];
    uint32 pk_addr[8];
} Fsmsw_Sphincsshake_256fSimple_LeafInfoX1_T;
/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsShake_256fSimple_wots_gen_leafx1(uint8 *dest, const sphincs_shake_256f_ctx *ctx, uint32 leaf_idx,
                                                   void *v_info);

#endif /* FSMSW_SPHINCSSHAKE_256FSIMPLE_WOTSX1_H */
