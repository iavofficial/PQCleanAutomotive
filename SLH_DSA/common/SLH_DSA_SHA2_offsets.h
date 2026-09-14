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
/** \addtogroup SLH_DSA_SHA2_offsets
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHA2_offsets.h
* \brief  description of SLH_DSA_SHA2_offsets.h file
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
#ifndef SLH_DSA_SHA2_OFFSETS_H
#define SLH_DSA_SHA2_OFFSETS_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define SLH_DSA_SHA256_BLOCK_BYTES  64u
#define SLH_DSA_SHA256_OUTPUT_BYTES 32u
#define SLH_DSA_SHA512_BLOCK_BYTES  128u
#define SLH_DSA_SHA512_OUTPUT_BYTES 64u
/* The byte used to specify the Merkle tree layer */
#define SLH_DSA_SHA2_OFFSET_LAYER 0
/* The start of the 8 byte field used to specify the tree */
#define SLH_DSA_SHA2_OFFSET_TREE 1
/* The byte used to specify the hash type (reason) */
#define SLH_DSA_SHA2_OFFSET_TYPE 9
/* The high byte used to specify the key pair (which one-time signature) */
#define SLH_DSA_SHA2_OFFSET_KP_ADDR2 12
/* The low byte used to specify the key pair */
#define SLH_DSA_SHA2_OFFSET_KP_ADDR1 13
/* The byte used to specify the chain address (which Winternitz chain) */
#define SLH_DSA_SHA2_OFFSET_CHAIN_ADDR 17
/* The byte used to specify the hash address (where in the Winternitz chain) */
#define SLH_DSA_SHA2_OFFSET_HASH_ADDR 21
/* The byte used to specify the height of this node in the FORS or Merkle tree */
#define SLH_DSA_SHA2_OFFSET_TREE_HGT 17
/* The start of the 4 byte field used to specify the node in the FORS or Merkle tree */
#define SLH_DSA_SHA2_OFFSET_TREE_INDEX 18
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

#endif /* SLH_DSA_SHA2_OFFSETS_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */