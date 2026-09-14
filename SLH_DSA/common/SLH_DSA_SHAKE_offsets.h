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
/** \addtogroup SLH_DSA_SHAKE_offsets
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHAKE_offsets.h
* \brief  description of SLH_DSA_SHAKE_offsets.h file
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
#ifndef SHAKE_OFFSETS_H_
#define SHAKE_OFFSETS_H_
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
/* The byte used to specify the Merkle tree layer */
#define SLH_DSA_SHAKE_OFFSET_LAYER 3
/* The start of the 8 byte field used to specify the tree */
#define SLH_DSA_SHAKE_OFFSET_TREE 8
/* The byte used to specify the hash type (reason) */
#define SLH_DSA_SHAKE_OFFSET_TYPE 19
/* The high byte used to specify the key pair (which one-time signature) */
#define SLH_DSA_SHAKE_OFFSET_KP_ADDR2 22
/* The low byte used to specify the key pair */
#define SLH_DSA_SHAKE_OFFSET_KP_ADDR1 23

/* The byte used to specify the chain address (which Winternitz chain) */
#define SLH_DSA_SHAKE_OFFSET_CHAIN_ADDR 27
/* The byte used to specify the hash address (where in the Winternitz chain) */
#define SLH_DSA_SHAKE_OFFSET_HASH_ADDR 31
/* The byte used to specify the height of this node in the FORS or Merkle tree */
#define SLH_DSA_SHAKE_OFFSET_TREE_HGT 27
/* The start of the 4 byte field used to specify the node in the FORS or Merkle tree */
#define SLH_DSA_SHAKE_OFFSET_TREE_INDEX 28
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

#endif /* SHAKE_OFFSETS_H_ */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */