/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsSha2_192fSimple
*    includes the modules for SphincsSha2_192fSimple
 ** @{ */
/** \addtogroup SphincsSha2_192fSimple_thash
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsSha2_192fSimple_thash_sha2_simple.c
* \brief  description of FsmSw_SphincsSha2_192fSimple_thash_sha2_simple.c
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
#include "FsmSw_SphincsSha2_192fSimple_hash.h"
#include "FsmSw_SphincsSha2_192fSimple_params.h"
#include "FsmSw_SphincsSha2_192fSimple_utils.h"
#include "FsmSw_Sphincs_sha2_address.h"
#include "FsmSw_sha2.h"

#include "FsmSw_SphincsSha2_192fSimple_thash.h"
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
 * \brief Takes an array of inblocks concatenated arrays of FSMSW_SPHINCSSHA2_192FSIMPLE_N bytes.
 *
 * \param[out] uint8                       *out : t.b.d.
 * \param[in]  const uint8                  *in : t.b.d.
 * \param[in]  uint32                  inblocks : t.b.d.
 * \param[in]  const sphincs_sha2_192f_ctx *ctx : t.b.d.
 * \param[in]  const uint32             addr[8] : t.b.d.
 *
 */
void FsmSw_SphincsSha2_192fSimple_Thash(uint8 *out, const uint8 *in, uint32 inblocks, const sphincs_sha2_192f_ctx *ctx,
                                        const uint32 addr[8])
{

  uint8 outbuf[FSMSW_SPHINCS_SHA256_OUTPUT_BYTES] = {0};
  sha256ctx sha2_state                            = {{0}};
  uint8 buf[SPX_SHA256_ADDR_BYTES + (FSMSW_SPHINCSSHA2_192FSIMPLE_THASH_BUF_LEN * FSMSW_SPHINCSSHA2_192FSIMPLE_N)] = {
      0};

  /* Retrieve precomputed state containing pub_seed */
  FsmSw_Sha256_IncCtxClone(&sha2_state, &ctx->state_seeded);

  FsmSw_CommonLib_MemCpy(buf, addr, SPX_SHA256_ADDR_BYTES);
  FsmSw_CommonLib_MemCpy(&buf[SPX_SHA256_ADDR_BYTES], in, inblocks * FSMSW_SPHINCSSHA2_192FSIMPLE_N);

  FsmSw_Sha256_IncFinalize(outbuf, &sha2_state, buf,
                           SPX_SHA256_ADDR_BYTES + (inblocks * FSMSW_SPHINCSSHA2_192FSIMPLE_N));
  FsmSw_CommonLib_MemCpy(out, outbuf, FSMSW_SPHINCSSHA2_192FSIMPLE_N);
} // end: FsmSw_SphincsSha2_192fSimple_Thash

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */