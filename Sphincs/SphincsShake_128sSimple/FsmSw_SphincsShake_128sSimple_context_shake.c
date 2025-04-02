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

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_SphincsShake_128sSimple_context.h"

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
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        FsmSw_SphincsShake_128sSimple_initialize_hash_function
*
* Description: For SHAKE256, there is no immediate reason to initialize at the start, so this function is an empty
*              operation.
*
* Arguments:   - sphincs_shake_128s_ctx *ctx: t.b.d.
*
***********************************************************************************************************************/
/* polyspace +2 MISRA2012:8.13 [Justified:]"For SHAKE256, there is no immediate reason to initialize at the start, 
so this function is an empty operation." */
void FsmSw_SphincsShake_128sSimple_initialize_hash_function(sphincs_shake_128s_ctx *ctx)
{
    (void)ctx; /* Suppress an 'unused parameter' warning. */
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsShake_128sSimple_1_free_hash_function
*
* Description: in case the hash function api is heap-based.
*
* Arguments:   - sphincs_shake_128s_ctx *ctx: t.b.d.
*
***********************************************************************************************************************/
/* polyspace +1 MISRA2012:8.13 [Justified:]"in case the hash function api is heap-based." */
void FsmSw_SphincsShake_128sSimple_1_free_hash_function(sphincs_shake_128s_ctx *ctx)
{
    (void)ctx;
}
