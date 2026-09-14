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
/** \addtogroup SLH_DSA_SHAKE_address
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHAKE_address.c
* \brief  description of SLH_DSA_SHAKE_address.c
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

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "SLH_DSA_CommonLib.h"
#include "SLH_DSA_SHAKE_offsets.h"
#include "SLH_DSA_utils.h"

#include "SLH_DSA_SHAKE_address.h"
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
/* GLOBAL CONSTANTS                                                                                                   */
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

/*====================================================================================================================*/
/**
* \brief Specify which level of Merkle tree (the "layer") we're working on
*
* \param[out] uint32 addr[8] : t.b.d.
* \param[in]  uint32   layer : t.b.d.
*
*/
void SLH_DSA_SHAKE_SetLayerAddr(uint32 addr[8], uint32 layer)
{
  ((uint8 *)addr)[SLH_DSA_SHAKE_OFFSET_LAYER] = (uint8)layer;
} // end: SLH_DSA_SHAKE_SetLayerAddr

/*====================================================================================================================*/
/**
* \brief Specify which Merkle tree within the level (the "tree address") we're working on
*
* \param[out] uint32  addr[8] : t.b.d.
* \param[in]  uint64     tree : t.b.d.
*
*/
void SLH_DSA_SHAKE_SetTreeAddr(uint32 addr[8], uint64 tree)
{
  SLH_DSA_UllToBytes(&((uint8 *)addr)[SLH_DSA_SHAKE_OFFSET_TREE], 8, tree);
} // end: SLH_DSA_SHAKE_SetTreeAddr

/*====================================================================================================================*/
/**
* \brief Specify the reason we'll use this address structure for, that is, what hash will we compute with it.
*        This is used so that unrelated types of hashes don't accidentally get the same address structure.
*        The type will be one of the SPX_ADDR_TYPE constants
*
* \param[out] uint32 addr[8] : t.b.d.
* \param[in]  uint32    type : t.b.d.
*
*/
void SLH_DSA_SHAKE_SetType(uint32 addr[8], uint32 type)
{
  ((uint8 *)addr)[SLH_DSA_SHAKE_OFFSET_TYPE] = (uint8)type;
} // end: SLH_DSA_SHAKE_SetType

/*====================================================================================================================*/
/**
* \brief Copy the layer and tree fields of the address structure.  This is used when we're doing multiple types
*        of hashes within the same Merkle tree.
*
* \param[out] uint32      out[8] : t.b.d.
* \param[in]  const uint32 in[8] : t.b.d.
*
*/
void SLH_DSA_SHAKE_CopySubTreeAddr(uint32 out[8], const uint32 in[8])
{
  SLH_DSA_CommonLib_MemCpy(out, in, SLH_DSA_SHAKE_OFFSET_TREE + 8);
} // end: SLH_DSA_SHAKE_CopySubTreeAddr

/* These functions are used for OTS addresses. */

/*====================================================================================================================*/
/**
* \brief Specify which Merkle leaf we're working on; that is, which OTS keypair we're talking about.
*
* \param[out] uint32 addr[8] : t.b.d.
* \param[in]  uint32 keypair : t.b.d.
*
*/
void SLH_DSA_SHAKE_SetKeyPairAddr1Byte(uint32 addr[8], uint32 keypair)
{
  ((uint8 *)addr)[SLH_DSA_SHAKE_OFFSET_KP_ADDR1] = (uint8)keypair;
} // end: SLH_DSA_SHAKE_SetKeyPairAddr1Byte

/*====================================================================================================================*/
/**
* \brief Specify which Merkle leaf we're working on; that is, which OTS keypair we're talking about.
*
* \param[out] uint32 addr[8] : t.b.d.
* \param[in]  uint32 keypair : t.b.d.
*
*/
void SLH_DSA_SHAKE_SetKeyPairAddr2Byte(uint32 addr[8], uint32 keypair)
{
  /* We have > 256 OTS at the bottom of the Merkle tree; to specify */
  /* which one, we'd need to express it in two bytes */
  ((uint8 *)addr)[SLH_DSA_SHAKE_OFFSET_KP_ADDR2] = (uint8)(keypair >> 8);
  ((uint8 *)addr)[SLH_DSA_SHAKE_OFFSET_KP_ADDR1] = (uint8)keypair;
} // end: SLH_DSA_SHAKE_SetKeyPairAddr2Byte

/*====================================================================================================================*/
/**
* \brief Copy the layer, tree and keypair fields of the address structure. This is used when we're doing multiple
*        things within the same OTS keypair.
*
* \param[out] uint32      out[8] : t.b.d.
* \param[in]  const uint32 in[8] : t.b.d.
*
*/
void SLH_DSA_SHAKE_CopyKeyPairAddr1Byte(uint32 out[8], const uint32 in[8])
{
  SLH_DSA_CommonLib_MemCpy(out, in, SLH_DSA_SHAKE_OFFSET_TREE + 8);
  ((uint8 *)out)[SLH_DSA_SHAKE_OFFSET_KP_ADDR1] = ((const uint8 *)in)[SLH_DSA_SHAKE_OFFSET_KP_ADDR1];
} // end: SLH_DSA_SHAKE_CopyKeyPairAddr1Byte

/*====================================================================================================================*/
/**
* \brief Copy the layer, tree and keypair fields of the address structure. This is used when we're doing multiple
*        things within the same OTS keypair.
*
* \param[out] uint32      out[8] : t.b.d.
* \param[in]  const uint32 in[8] : t.b.d.
*
*/
void SLH_DSA_SHAKE_CopyKeyPairAddr2Byte(uint32 out[8], const uint32 in[8])
{
  SLH_DSA_CommonLib_MemCpy(out, in, SLH_DSA_SHAKE_OFFSET_TREE + 8);
  ((uint8 *)out)[SLH_DSA_SHAKE_OFFSET_KP_ADDR2] = ((const uint8 *)in)[SLH_DSA_SHAKE_OFFSET_KP_ADDR2];
  ((uint8 *)out)[SLH_DSA_SHAKE_OFFSET_KP_ADDR1] = ((const uint8 *)in)[SLH_DSA_SHAKE_OFFSET_KP_ADDR1];
} // end: SLH_DSA_SHAKE_CopyKeyPairAddr2Byte

/*====================================================================================================================*/
/**
* \brief Specify which Merkle chain within the OTS we're working with (the chain address)
*
* \param[out] uint32 addr[8] : t.b.d.
* \param[in]  uint32   chain : t.b.d.
*
*/
void SLH_DSA_SHAKE_SetChainAddr(uint32 addr[8], uint32 chain)
{
  ((uint8 *)addr)[SLH_DSA_SHAKE_OFFSET_CHAIN_ADDR] = (uint8)chain;
} // end: SLH_DSA_SHAKE_SetChainAddr

/*====================================================================================================================*/
/**
* \brief Specify where in the Merkle chain we are (the hash address)
*
* \param[out] uint32 addr[8] : t.b.d.
* \param[in]  uint32    hash : t.b.d.
*
*/
void SLH_DSA_SHAKE_SetHashAddr(uint32 addr[8], uint32 hash)
{
  ((uint8 *)addr)[SLH_DSA_SHAKE_OFFSET_HASH_ADDR] = (uint8)hash;
} // end: SLH_DSA_SHAKE_SetHashAddr

/* These functions are used for all hash tree addresses (including FORS). */

/*====================================================================================================================*/
/**
* \brief Specify the height of the node in the Merkle/FORS tree we are in (the tree height)
*
* \param[out] uint32     addr[8] : t.b.d.
* \param[in]  uint32 tree_height : t.b.d.
*
*/
void SLH_DSA_SHAKE_SetTreeHeight(uint32 addr[8], uint32 tree_height)
{
  ((uint8 *)addr)[SLH_DSA_SHAKE_OFFSET_TREE_HGT] = (uint8)tree_height;
} // end: SLH_DSA_SHAKE_SetTreeHeight

/*====================================================================================================================*/
/**
* \brief Specify the distance from the left edge of the node in the Merkle/FORS tree (the tree index)
*
* \param[out] uint32    addr[8] : t.b.d.
* \param[in]  uint32 tree_index : t.b.d.
*
*/
void SLH_DSA_SHAKE_SetTreeIndex(uint32 addr[8], uint32 tree_index)
{
  SLH_DSA_U32ToBytes(&((uint8 *)addr)[SLH_DSA_SHAKE_OFFSET_TREE_INDEX], tree_index);
} // end: SLH_DSA_SHAKE_SetTreeIndex

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
