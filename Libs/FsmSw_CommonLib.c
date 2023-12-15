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
#include "FsmSw_CommonLib.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/
static uint32 next = 1;

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/
static uint8  iav_commonlib_rand_byte(void);
static uint32 iav_commonlib_rand(void);

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        iav_commonlib_rand_byte
*
* Description: This function generates a 1 byte random value
*
* Arguments:   -
* Returns a 1 byte random value
***********************************************************************************************************************/
static uint8 iav_commonlib_rand_byte(void)
{
    return (uint8)((float32)(256.0f * ((float32)((float32)iav_commonlib_rand()/32768.0f))));
}

/***********************************************************************************************************************
* Name:        iav_commonlib_rand
*
* Description: This function generates a 4 byte random value

* Arguments:   -
* Returns a 4 byte random value
***********************************************************************************************************************/
static uint32 iav_commonlib_rand(void)
{
    next = next * 1103515245u + 12345u;
    return( (uint32)(next/65536u) & (32768u - 1u));

}

/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        FsmSw_CommonLib_memcpy
*
* Description: This function copies n values from dest to src
*
* Arguments:   -       void   *dest: pointer to output array destination
*              - const void   *src:  pointer to input array source
*              - const uint32  n:    number of  values
*
***********************************************************************************************************************/
void FsmSw_CommonLib_memcpy(void *dest, const void * src,  const uint32 n)
{
    uint32 i;
    uint8 *destPtr = (uint8*)dest;
    const uint8* srcPtr  = (const uint8*)src;

    for (i=0; i<n; i++)
    {
        destPtr[i] = srcPtr[i];
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_CommonLib_memset
*
* Description: This function sets n values from dest to value
*
* Arguments:   -       void   *dest:  pointer to output array destination
*              - const uint8   value: set dest array to this value
*              - const uint32  n:     number of values
*
***********************************************************************************************************************/
void FsmSw_CommonLib_memset(void *dest, const uint8 value, const uint32 n)
{
    uint32 i;
    uint8 *destPtr = (uint8*)dest;

    for (i=0; i<n; i++)
    {
        destPtr[i] = value;
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_CommonLib_memcmp
*
* Description: This function compares dest to src
*
* Arguments:   - const void   *dest: pointer to input array destination
*              - const void   *src:  pointer to input array source
*              - const uint32  n:    number of  values
*
* Returns 0 success, otherwise 1
***********************************************************************************************************************/
uint8 FsmSw_CommonLib_memcmp(void *dest, const void * src,  const uint32 n)
{
    uint32 i;
    uint8 *destPtr = (uint8*)dest;
    const uint8* srcPtr  = (const uint8*)src;

    for (i=0; i<n; i++)
    {
        if (destPtr[i] != srcPtr[i])
        {
            return 1;
        }
    }

    return 0;
}

/***********************************************************************************************************************
* Name:        FsmSw_CommonLib_memmove
*
* Description: This function moves n values from dest to src
*
* Arguments:   -       void   *dest: pointer to output array destination
*              - const void   *src:  pointer to input array source
*              - const uint32  n:    number of  values
*
***********************************************************************************************************************/
void FsmSw_CommonLib_memmove(void *dest, const void * src,  const uint32 n)
{
    sint32 i;
    uint8 *destPtr = (uint8*)dest;
    const uint8* srcPtr  = (const uint8*)src;

    if (destPtr > srcPtr)
    {    /* copy from right to left */
        for (i = (sint32)((sint32)n - (sint32)1u); i>=0; i--)
        {
            destPtr[i] = srcPtr[i];
        }
    }
    else
    {   /* copy from left to right */
        for (i=0; i<(sint32)n; i++)
        {
            destPtr[i] = srcPtr[i];
        }

    }
}

/***********************************************************************************************************************
* Name:        FsmSw_CommonLib_randombytes
*
* Description: This function generates n random values
*
* Arguments:   -       uint8  *output: pointer to output array with random values
*              - const uint32  n:      number of random values
*
* Returns 0 (success)
***********************************************************************************************************************/
uint8 FsmSw_CommonLib_randombytes(uint8 *output, const uint32 n)
{
    for (uint32 i = 0; i < n; i++)
    {
        output[i] = iav_commonlib_rand_byte();
    }

    return 0;
}

/***********************************************************************************************************************
* Name:        FsmSw_CommonLib_srand
*
* Description: Initialize the random function with a seed
*
* Arguments:   - const uint32 seed: start seed for generating random values
***********************************************************************************************************************/
void FsmSw_CommonLib_srand(const uint32 seed)
{
    next = seed;
}
