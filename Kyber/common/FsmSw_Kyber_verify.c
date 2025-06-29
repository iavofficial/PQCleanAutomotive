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
#include "FsmSw_Types.h"

#include "FsmSw_Kyber_verify.h"
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
* Name:        FsmSw_Kyber_Verify
*
* Description: Compare two arrays for equality in constant time.
*
* Arguments:   const uint8 *a:   pointer to first byte array
*              const uint8 *b:   pointer to second byte array
*                    uint32 len: length of the byte arrays
*
* Returns 0 if the byte arrays are equal, 1 otherwise
***********************************************************************************************************************/
uint8 FsmSw_Kyber_Verify(const uint8 *a, const uint8 *b, uint32 len)
{
  uint32 i = 0;
  uint8 r  = 0;

  sint64 temp1 = 0;
  uint64 temp2 = 0;

  for (i = 0; i < len; i++)
  {
    r |= a[i] ^ b[i];
  }

  temp1 = (-1) * (sint64)r;

  if (((uint64)temp1 & 0x8000000000000000u) != 0u)
  {
    temp2 = ((uint64)temp1 >> 63u) | 0xFFFFFFFFFFFFFFFEu;
  }
  else
  {
    temp2 = ((uint64)temp1 >> 63u);
  }

  return (uint8)temp2;
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber_Cmov
*
* Description: Copy len bytes from x to r if b is 1;
*              don't modify x if b is 0. Requires b to be in {0,1};
*              assumes two's complement representation of negative integers.
*              Runs in constant time.
*
* Arguments:   uint8 *r:       pointer to output byte array
*              const uint8 *x: pointer to input byte array
*              uint32 len:     Amount of bytes to be copied
*              uint8 b:        Condition bit; has to be in {0,1}
***********************************************************************************************************************/
void FsmSw_Kyber_Cmov(uint8 *r, const uint8 *x, uint32 len, uint8 b)
{
  uint32 i     = 0;
  sint8 b_sint = 0;

  b_sint = (-1) * (sint8)b;

  for (i = 0; i < len; i++)
  {
    r[i] = r[i] ^ ((uint8)b_sint & (r[i] ^ x[i]));
  }
}
