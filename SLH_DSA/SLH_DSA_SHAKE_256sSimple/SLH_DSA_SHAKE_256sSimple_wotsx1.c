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
/** \addtogroup SLH_DSA_SHAKE_256sSimple
*    includes the modules for SLH_DSA_SHAKE_256sSimple
 ** @{ */
/** \addtogroup SLH_DSA_SHAKE_256sSimple_wotsx1
 ** @{ */

/*====================================================================================================================*/
/** \file SLH_DSA_SHAKE_256sSimple_wotsx1.c
* \brief  description of SLH_DSA_SHAKE_256sSimple_wotsx1.c
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
#include "FsmSw_CommonLib.h"
#include "SLH_DSA_SHAKE_256sSimple_FctWrapper.h"
#include "SLH_DSA_SHAKE_256sSimple_hash.h"
#include "SLH_DSA_SHAKE_256sSimple_params.h"
#include "SLH_DSA_SHAKE_256sSimple_thash.h"
#include "SLH_DSA_SHAKE_256sSimple_utils.h"
#include "SLH_DSA_SHAKE_256sSimple_wots.h"
#include "SLH_DSA_SHAKE_address.h"

#include "SLH_DSA_SHAKE_256sSimple_wotsx1.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define SLH_DSA_UINT32_MAX_VALUE 0xFFFFFFFFu
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
 * \brief This generates a WOTS public key. It also generates the WOTS signature if leaf_info indicates that we're
 *              signing with this WOTS key.
 *
 * \param[out] uint8                       *dest : t.b.d.
 * \param[in]  const slh_dsa_shake_256s_ctx *ctx : t.b.d.
 * \param[in]  uint32                   leaf_idx : t.b.d.
 * \param[in]  void                      *v_info : t.b.d.
 *
 */
void SLH_DSA_SHAKE_256sSimple_Wots_GenLeafX1(uint8 *const dest, const slh_dsa_shake_256s_ctx *const ctx,
                                                  uint32 leaf_idx, void *const v_info)
{
  /* polyspace +4 CERT-C:EXP36-C [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality.
    Ensured proper alignment and validity." */
  SLH_DSA_SHAKE_256sSimple_LeafInfoX1_T *info          = v_info;
  uint32 *const leaf_addr                                   = info->leaf_addr;
  uint32 *const pk_addr                                     = info->pk_addr;
  uint32 i                                                  = 0;
  uint32 k                                                  = 0;
  uint8 pk_buffer[SLH_DSA_SHAKE_256SSIMPLE_WOTS_BYTES] = {0};
  uint8 *buffer                                             = (uint8 *)NULL_PTR;
  uint32 wots_k_mask                                        = 0;

  if (leaf_idx == info->wots_sign_leaf)
  {
    /* We're traversing the leaf that's signing; generate the WOTS signature */
    wots_k_mask = 0;
  }
  else
  {
    /* Nope, we're just generating pk's; turn off the signature logic */
    wots_k_mask = (uint32)~0u;
  }

  SLH_DSA_SHAKE_256sSimple_set_keypair_addr(leaf_addr, leaf_idx);
  SLH_DSA_SHAKE_256sSimple_set_keypair_addr(pk_addr, leaf_idx);

  buffer = pk_buffer;
  for (i = 0; i < SLH_DSA_SHAKE_256SSIMPLE_WOTS_LEN; i++)
  {
    /* Set wots_k to the step if we're generating a signature, ~0 if we're not */
    uint32 const wots_k = info->wots_steps[i] | wots_k_mask;

    /* Start with the secret seed */
    SLH_DSA_SHAKE_SetChainAddr(leaf_addr, i);
    SLH_DSA_SHAKE_SetHashAddr(leaf_addr, 0);
    SLH_DSA_SHAKE_SetType(leaf_addr, SLH_DSA_ADDR_TYPE_WOTSPRF);

    SLH_DSA_SHAKE_256sSimple_PrfAddr(buffer, ctx, leaf_addr);

    SLH_DSA_SHAKE_SetType(leaf_addr, SLH_DSA_ADDR_TYPE_WOTS);

    /* Iterate down the WOTS chain */
    for (k = 0; k < SLH_DSA_UINT32_MAX_VALUE; k++)
    {
      /* Check if this is the value that needs to be saved as a part of the WOTS signature */
      if (k == wots_k)
      {
        FsmSw_CommonLib_MemCpy(&info->wots_sig[i * SLH_DSA_SHAKE_256SSIMPLE_N], buffer,
                               SLH_DSA_SHAKE_256SSIMPLE_N);
      }

      /* Check if we hit the top of the chain */
      if (k == SLH_DSA_SHAKE_256SSIMPLE_WOTS_W - 1u)
      {
        break;
      }

      /* Iterate one step on the chain */
      SLH_DSA_SHAKE_SetHashAddr(leaf_addr, k);

      SLH_DSA_SHAKE_256sSimple_Thash(buffer, buffer, 1, ctx, leaf_addr);
    }

    buffer = &buffer[SLH_DSA_SHAKE_256SSIMPLE_N];
  }

  /* Do the final SLH_DSA_SHAKE_256sSimple_Thash to generate the public keys */
  SLH_DSA_SHAKE_256sSimple_Thash(dest, pk_buffer, SLH_DSA_SHAKE_256SSIMPLE_WOTS_LEN, ctx, pk_addr);
} // end: SLH_DSA_SHAKE_256sSimple_Wots_GenLeafX1

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
