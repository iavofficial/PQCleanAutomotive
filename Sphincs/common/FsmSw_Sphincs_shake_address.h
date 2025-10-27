/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup common
*    includes the modules for common
 ** @{ */
/** \addtogroup FsmSw_Sphincs_shake_address
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Sphincs_shake_address.h
* \brief  description of FsmSw_Sphincs_shake_address.h file
*
* \details
*
*
*/
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
#ifndef FSMSW_SPHINCSSHAKE_ADDRESS_H
#define FSMSW_SPHINCSSHAKE_ADDRESS_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Sphincs_address.h"
#include "FsmSw_Types.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL CONSTANTS                                                                                                   */
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
void FsmSw_SphincsShake_SetLayerAddr(uint32 addr[8], uint32 layer);

void FsmSw_SphincsShake_SetTreeAddr(uint32 addr[8], uint64 tree);

void FsmSw_SphincsShake_SetType(uint32 addr[8], uint32 type);

void FsmSw_SphincsShake_CopySubTreeAddr(uint32 out[8], const uint32 in[8]);

void FsmSw_SphincsShake_SetKeyPairAddr1Byte(uint32 addr[8], uint32 keypair);

/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsShake_SetKeyPairAddr2Byte(uint32 addr[8], uint32 keypair);

void FsmSw_SphincsShake_SetChainAddr(uint32 addr[8], uint32 chain);

void FsmSw_SphincsShake_SetHashAddr(uint32 addr[8], uint32 hash);

void FsmSw_SphincsShake_CopyKeyPairAddr1Byte(uint32 out[8], const uint32 in[8]);

/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsShake_CopyKeyPairAddr2Byte(uint32 out[8], const uint32 in[8]);

void FsmSw_SphincsShake_SetTreeHeight(uint32 addr[8], uint32 tree_height);

void FsmSw_SphincsShake_SetTreeIndex(uint32 addr[8], uint32 tree_index);

#endif /* FSMSW_SPHINCSSHAKE_ADDRESS_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */