/***********************************************************************************************************************
 *
 * Original implementation: PQClean, SLH-DSA (standardized as SLH-DSA)
 *
 * Copyright 2026 IAV GmbH
 *
 * Original portions are dedicated to the public domain under CC0 1.0 Universal.
 * See the NOTICE file in the repository root for attribution information.
 * IAV modifications are licensed under the Apache License, Version 2.0.
 *
 * SPDX-License-Identifier: CC0-1.0 AND Apache-2.0
 *
 **********************************************************************************************************************/

/** \addtogroup SwC SLH-DSA
*    includes the modules for SwC SLH-DSA
 ** @{ */
/** \addtogroup common
*    includes the modules for common
 ** @{ */
/** \addtogroup SLH_DSA_SHA2_address
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHA2_address.h
* \brief  description of SLH_DSA_SHA2_address.h file
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
#ifndef SLH_DSA_SHA2_ADDRESS_H
#define SLH_DSA_SHA2_ADDRESS_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "SLH_DSA_address.h"
#include "Std_Types.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL CONSTANTS                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void SLH_DSA_SHA2_SetLayerAddr(uint32 addr[8], uint32 layer);

void SLH_DSA_SHA2_SetTreeAddr(uint32 addr[8], uint64 tree);

void SLH_DSA_SHA2_SetType(uint32 addr[8], uint32 type);

void SLH_DSA_SHA2_CopySubTreeAddr(uint32 out[8], const uint32 in[8]);

void SLH_DSA_SHA2_SetKeyPairAddr1Byte(uint32 addr[8], uint32 keypair);

void SLH_DSA_SHA2_SetKeyPairAddr2Byte(uint32 addr[8], uint32 keypair);

void SLH_DSA_SHA2_SetChainAddr(uint32 addr[8], uint32 chain);

void SLH_DSA_SHA2_SetHashAddr(uint32 addr[8], uint32 hash);

void SLH_DSA_SHA2_CopyKeyPairAddr1Byte(uint32 out[8], const uint32 in[8]);

void SLH_DSA_SHA2_CopyKeyPairAddr2Byte(uint32 out[8], const uint32 in[8]);

void SLH_DSA_SHA2_SetTreeHeight(uint32 addr[8], uint32 tree_height);

void SLH_DSA_SHA2_SetTreeIndex(uint32 addr[8], uint32 tree_index);

#endif /* SLH_DSA_SHA2_ADDRESS_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
