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
/** \addtogroup FsmSw_Sphincs_sha2_address
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Sphincs_sha2_address.c
* \brief  description of FsmSw_Sphincs_sha2_address.c
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
#include "FsmSw_Sphincs_sha2_address.h"
#include "FsmSw_CommonLib.h"
#include "FsmSw_Sphincs_sha2_offsets.h"
#include "FsmSw_Sphincs_utils.h"

#include "FsmSw_Sphincs_sha2_address.h"
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
void FsmSw_SphincsSha2_SetLayerAddr(uint32 addr[8], uint32 layer)
{
  ((uint8 *)addr)[FSMSW_SPHINCSSHA2_OFFSET_LAYER] = (uint8)layer;
} // end: FsmSw_SphincsSha2_SetLayerAddr

/*====================================================================================================================*/
/**
* \brief Specify which Merkle tree within the level (the "tree address") we're working on
*
* \param[out] uint32 addr[8] : t.b.d.
* \param[in]  uint64    tree :    t.b.d.
*
*/
void FsmSw_SphincsSha2_SetTreeAddr(uint32 addr[8], uint64 tree)
{
  FsmSw_Sphincs_UllToBytes(&((uint8 *)addr)[FSMSW_SPHINCSSHA2_OFFSET_TREE], 8, tree);
} // end: FsmSw_SphincsSha2_SetTreeAddr

/*====================================================================================================================*/
/**
* \brief Specify the reason we'll use this address structure for, that is, what hash will we compute with it.
*        This is used so that unrelated types of hashes don't accidentally get the same address structure.
*        The type will be one of the SPX_ADDR_TYPE constants.
*
* \param[out] uint32 addr[8] : t.b.d.
* \param[in]  uint32    type : t.b.d.
*
*/
void FsmSw_SphincsSha2_SetType(uint32 addr[8], uint32 type)
{
  ((uint8 *)addr)[FSMSW_SPHINCSSHA2_OFFSET_TYPE] = (uint8)type;
} // end: FsmSw_SphincsSha2_SetType

/*====================================================================================================================*/
/**
* \brief Copy the layer and tree fields of the address structure. This is used when we're doing multiple types
*        of hashes within the same Merkle tree
*
* \param[out] uint32      out[8] : t.b.d.
* \param[in]  const uint32 in[8] : t.b.d.
*
*/
void FsmSw_SphincsSha2_CopySubTreeAddr(uint32 out[8], const uint32 in[8])
{
  FsmSw_CommonLib_MemCpy(out, in, FSMSW_SPHINCSSHA2_OFFSET_TREE + 8);
} // end: FsmSw_SphincsSha2_CopySubTreeAddr

/* These functions are used for OTS addresses. */

/*====================================================================================================================*/
/**
* \brief Specify which Merkle leaf we're working on; that is, which OTS keypair we're talking about.
*
* \param[out] uint32 addr[8] : t.b.d.
* \param[in]  uint32 keypair : t.b.d
*
*/
void FsmSw_SphincsSha2_SetKeyPairAddr1Byte(uint32 addr[8], uint32 keypair)
{
  ((uint8 *)addr)[FSMSW_SPHINCSSHA2_OFFSET_KP_ADDR1] = (uint8)keypair;
} // end: FsmSw_SphincsSha2_SetKeyPairAddr1Byte

/*====================================================================================================================*/
/**
* \brief Specify which Merkle leaf we're working on; that is, which OTS keypair we're talking about.
*
* \param[out] uint32 addr[8] : t.b.d.
* \param[in]  uint32 keypair : t.b.d.
*
*/
/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsSha2_SetKeyPairAddr2Byte(uint32 addr[8], uint32 keypair)
{
  /* We have > 256 OTS at the bottom of the Merkle tree; to specify which one, we'd need to express it in two bytes */
  ((uint8 *)addr)[FSMSW_SPHINCSSHA2_OFFSET_KP_ADDR2] = (uint8)(keypair >> 8);
  ((uint8 *)addr)[FSMSW_SPHINCSSHA2_OFFSET_KP_ADDR1] = (uint8)keypair;
} // end: FsmSw_SphincsSha2_SetKeyPairAddr2Byte

/*====================================================================================================================*/
/**
* \brief Copy the layer, tree and keypair fields of the address structure. This is used when we're doing
*        multiple things within the same OTS keypair
*
* \param[out] uint32      out[8] : t.b.d.
* \param[in]  const uint32 in[8] : t.b.d.
*
*/
void FsmSw_SphincsSha2_CopyKeyPairAddr1Byte(uint32 out[8], const uint32 in[8])
{
  FsmSw_CommonLib_MemCpy(out, in, FSMSW_SPHINCSSHA2_OFFSET_TREE + 8);
  ((uint8 *)out)[FSMSW_SPHINCSSHA2_OFFSET_KP_ADDR1] = ((const uint8 *)in)[FSMSW_SPHINCSSHA2_OFFSET_KP_ADDR1];
} // end: FsmSw_SphincsSha2_CopyKeyPairAddr1Byte

/*====================================================================================================================*/
/**
* \brief Copy the layer, tree and keypair fields of the address structure. This is used when we're doing
*        multiple things within the same OTS keypair
*
* \param[out] uint32      out[8] : t.b.d.
* \param[in]  const uint32 in[8] : t.b.d.
*
*/
/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsSha2_CopyKeyPairAddr2Byte(uint32 out[8], const uint32 in[8])
{
  FsmSw_CommonLib_MemCpy(out, in, FSMSW_SPHINCSSHA2_OFFSET_TREE + 8);
  ((uint8 *)out)[FSMSW_SPHINCSSHA2_OFFSET_KP_ADDR2] = ((const uint8 *)in)[FSMSW_SPHINCSSHA2_OFFSET_KP_ADDR2];
  ((uint8 *)out)[FSMSW_SPHINCSSHA2_OFFSET_KP_ADDR1] = ((const uint8 *)in)[FSMSW_SPHINCSSHA2_OFFSET_KP_ADDR1];
} // end: FsmSw_SphincsSha2_CopyKeyPairAddr2Byte

/*====================================================================================================================*/
/**
* \brief Specify which Merkle chain within the OTS we're working with (the chain address)
*
* \param[out] uint32 addr[8] : t.b.d.
* \param[in]  uint32   chain : t.b.d.
*
*/
void FsmSw_SphincsSha2_SetChainAddr(uint32 addr[8], uint32 chain)
{
  ((uint8 *)addr)[FSMSW_SPHINCSSHA2_OFFSET_CHAIN_ADDR] = (uint8)chain;
} // end: FsmSw_SphincsSha2_SetChainAddr

/*====================================================================================================================*/
/**
* \brief Specify where in the Merkle chain we are (the hash address)
*
* \param[out] uint32 addr[8] : t.b.d.
* \param[in]  uint32    hash : t.b.d.
*
*/
void FsmSw_SphincsSha2_SetHashAddr(uint32 addr[8], uint32 hash)
{
  ((uint8 *)addr)[FSMSW_SPHINCSSHA2_OFFSET_HASH_ADDR] = (uint8)hash;
} // end: FsmSw_SphincsSha2_SetHashAddr

/* These functions are used for all hash tree addresses (including FORS). */

/*====================================================================================================================*/
/**
* \brief Specify the height of the node in the Merkle/FORS tree we are in (the tree height)
*
* \param[out] uint32     addr[8] : t.b.d.
* \param[in]  uint32 tree_height : t.b.d.
*
*/
void FsmSw_SphincsSha2_SetTreeHeight(uint32 addr[8], uint32 tree_height)
{
  ((uint8 *)addr)[FSMSW_SPHINCSSHA2_OFFSET_TREE_HGT] = (uint8)tree_height;
} // end: FsmSw_SphincsSha2_SetTreeHeight

/*====================================================================================================================*/
/**
* \brief Specify the distance from the left edge of the node in the Merkle/FORS tree (the tree index)
*
* \param[out] uint32    addr[8] : t.b.d.
* \param[in]  uint32 tree_index : t.b.d.
*
*/
void FsmSw_SphincsSha2_SetTreeIndex(uint32 addr[8], uint32 tree_index)
{
  FsmSw_Sphincs_U32ToBytes(&((uint8 *)addr)[FSMSW_SPHINCSSHA2_OFFSET_TREE_INDEX], tree_index);
} // end: FsmSw_SphincsSha2_SetTreeIndex

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */