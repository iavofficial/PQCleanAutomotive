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

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Dilithium_params.h"
#include "FsmSw_Dilithium_reduce.h"

#include "FsmSw_Types.h"

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
* Name:        FsmSw_Dilithium_montgomery_reduce
*
* Description: For finite field element a with -2^{31}Q <= a <= Q*2^31,
*              compute t \equiv a*2^{-32} (mod Q) such that -Q < r < Q.
*
* Arguments:   - sint64: finite field element a
*
* Returns t .
***********************************************************************************************************************/
sint32 FsmSw_Dilithium_montgomery_reduce(sint64 a)
{
    sint32 t;
    sint64 temp;

    t = (sint32)((uint32)((uint64)a * (uint64)QINV));

    temp = a - ((sint64)t * (sint64)Q_DILITHIUM);
    t    = (sint32)((uint32)((uint64)temp >> 32));


    return t;
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium_reduce32
*
* Description: For finite field element a with a <= 2^{31} - 2^{22} - 1,
*              compute t \equiv a (mod Q) such that -6283009 <= r <= 6283007.
*
* Arguments:   - sint32: finite field element a
*
* Returns t.
***********************************************************************************************************************/
sint32 FsmSw_Dilithium_reduce32(sint32 a)
{
    sint32 t;
    sint32 temp;

    temp = a + (sint32)((uint32)((uint32)1 << 22u));
    t = (sint32)((uint32)((uint64)temp >> 23));

    t = a - (t * Q_DILITHIUM);

    return t;
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium_caddq
*
* Description: Add Q if input coefficient is negative.
*
* Arguments:   - sint32: finite field element a
*
* Returns result of addition.
***********************************************************************************************************************/
sint32 FsmSw_Dilithium_caddq(sint32 a)
{
    uint32 temp;

    /* a_temp is used to avoid modifying the input. */
    sint32 a_temp = a;

    temp = (uint32)((uint64)a_temp >> 31);
    a_temp = a_temp + (sint32)((uint32)(temp & (uint32)Q_DILITHIUM));

    return a_temp;
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium_freeze
*
* Description: For finite field element a, compute standard
*              representative r = a mod^+ Q.
*
* Arguments:   - sint32: finite field element a
*
* Returns r.
***********************************************************************************************************************/
sint32 FsmSw_Dilithium_freeze(sint32 a)
{
    /* a_temp is used to avoid modifying the input. */
    sint32 a_temp = a;

    a_temp = FsmSw_Dilithium_reduce32(a_temp);
    a_temp = FsmSw_Dilithium_caddq(a_temp);

    return a_temp;
}
