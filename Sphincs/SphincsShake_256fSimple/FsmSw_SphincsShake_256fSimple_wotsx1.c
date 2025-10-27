/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsShake_256fSimple
*    includes the modules for SphincsShake_256fSimple
 ** @{ */
/** \addtogroup SphincsShake_256fSimple_wotsx1
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsShake_256fSimple_wotsx1.c
* \brief  description of FsmSw_SphincsShake_256fSimple_wotsx1.c
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
#include "FsmSw_SphincsShake_256fSimple_FctWrapper.h"
#include "FsmSw_SphincsShake_256fSimple_hash.h"
#include "FsmSw_SphincsShake_256fSimple_params.h"
#include "FsmSw_SphincsShake_256fSimple_thash.h"
#include "FsmSw_SphincsShake_256fSimple_utils.h"
#include "FsmSw_SphincsShake_256fSimple_wots.h"
#include "FsmSw_Sphincs_shake_address.h"

#include "FsmSw_SphincsShake_256fSimple_wotsx1.h"
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
/*
 * This generates a WOTS public key
 * It also generates the WOTS signature if leaf_info indicates
 * that we're signing with this WOTS key
 */

/*====================================================================================================================*/
/**
 * \brief This generates a WOTS public key. It also generates the WOTS signature if leaf_info indicates that we're
 *        signing with this WOTS key.
 *
 * \param[out] uint8                       *dest : t.b.d.
 * \param[in]  const sphincs_shake_256f_ctx *ctx : t.b.d.
 * \param[in]  uint32                   leaf_idx : t.b.d.
 * \param[in]  void                      *v_info : t.b.d.
 *
 */
/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsShake_256fSimple_Wots_GenLeafX1(uint8 *dest, const sphincs_shake_256f_ctx *ctx, uint32 leaf_idx,
                                                  void *v_info)
{
  /* polyspace +4 CERT-C:EXP36-C [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality.
    Ensured proper alignment and validity." */
  Fsmsw_Sphincsshake_256fSimple_LeafInfoX1_T *info          = v_info;
  uint32 *leaf_addr                                         = info->leaf_addr;
  uint32 *pk_addr                                           = info->pk_addr;
  uint32 i                                                  = 0;
  uint32 k                                                  = 0;
  uint8 pk_buffer[FSMSW_SPHINCSSHAKE_256FSIMPLE_WOTS_BYTES] = {0};
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

  FsmSw_SphincsShake_256fSimple_set_keypair_addr(leaf_addr, leaf_idx);
  FsmSw_SphincsShake_256fSimple_set_keypair_addr(pk_addr, leaf_idx);

  buffer = pk_buffer;

  for (i = 0; i < FSMSW_SPHINCSSHAKE_256FSIMPLE_WOTS_LEN; i++)
  {
    /* Set wots_k to the step if we're generating a signature, ~0 if we're not */
    uint32 wots_k = info->wots_steps[i] | wots_k_mask;

    /* Start with the secret seed */
    FsmSw_SphincsShake_SetChainAddr(leaf_addr, i);
    FsmSw_SphincsShake_SetHashAddr(leaf_addr, 0);
    FsmSw_SphincsShake_SetType(leaf_addr, FSMSW_SPHINCS_ADDR_TYPE_WOTSPRF);

    FsmSw_SphincsShake_256fSimple_PrfAddr(buffer, ctx, leaf_addr);

    FsmSw_SphincsShake_SetType(leaf_addr, FSMSW_SPHINCS_ADDR_TYPE_WOTS);

    /* Iterate down the WOTS chain */
    for (k = 0; k < 0xFFFFFFFFu; k++)
    {
      /* Check if this is the value that needs to be saved as a part of the WOTS signature */
      if (k == wots_k)
      {
        FsmSw_CommonLib_MemCpy(&info->wots_sig[i * FSMSW_SPHINCSSHAKE_256FSIMPLE_N], buffer,
                               FSMSW_SPHINCSSHAKE_256FSIMPLE_N);
      }

      /* Check if we hit the top of the chain */
      if (k == FSMSW_SPHINCSSHAKE_256FSIMPLE_WOTS_W - 1u)
      {
        break;
      }

      /* Iterate one step on the chain */
      FsmSw_SphincsShake_SetHashAddr(leaf_addr, k);

      FsmSw_SphincsShake_256fSimple_Thash(buffer, buffer, 1, ctx, leaf_addr);
    }

    buffer = &buffer[FSMSW_SPHINCSSHAKE_256FSIMPLE_N];
  }

  /* Do the final FsmSw_SphincsShake_256fSimple_Thash to generate the public keys */
  FsmSw_SphincsShake_256fSimple_Thash(dest, pk_buffer, FSMSW_SPHINCSSHAKE_256FSIMPLE_WOTS_LEN, ctx, pk_addr);
} // end: FsmSw_SphincsShake_256fSimple_Wots_GenLeafX1

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */