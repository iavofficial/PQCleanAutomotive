/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsShake_192sSimple
*    includes the modules for SphincsShake_192sSimple
 ** @{ */
/** \addtogroup SphincsShake_192sSimple_thash
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsShake_192sSimple_thash_shake_simple.c
* \brief  description of FsmSw_SphincsShake_192sSimple_thash_shake_simple.c
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
#include "FsmSw_Fips202.h"
#include "FsmSw_SphincsShake_192sSimple_params.h"
#include "FsmSw_SphincsShake_192sSimple_utils.h"
#include "FsmSw_Sphincs_shake_address.h"

#include "FsmSw_SphincsShake_192sSimple_thash.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                     */
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
 * \brief Takes an array of inblocks concatenated arrays of FSMSW_SPHINCSSHAKE_192SSIMPLE_N bytes.
 *
 * \param[out] uint8                        *out : t.b.d.
 * \param[in]  const uint8                   *in : t.b.d.
 * \param[in]  uint32                   inblocks : t.b.d.
 * \param[in]  const sphincs_shake_192s_ctx *ctx : t.b.d.
 * \param[in]  const uint32              addr[8] : t.b.d.
 *
 */
void FsmSw_SphincsShake_192sSimple_Thash(uint8 *out, const uint8 *in, uint32 inblocks,
                                         const sphincs_shake_192s_ctx *ctx, const uint32 addr[8])
{
  uint8 buf[FSMSW_SPHINCSSHAKE_192SSIMPLE_N + FSMSW_SPHINCSSHAKE_192SSIMPLE_ADDR_BYTES +
            (FSMSW_SPHINCSSHAKE_192SSIMPLE_THASH_BUF_LEN * FSMSW_SPHINCSSHAKE_192SSIMPLE_N)] = {0};

  FsmSw_CommonLib_MemCpy(buf, ctx->pub_seed, FSMSW_SPHINCSSHAKE_192SSIMPLE_N);
  FsmSw_CommonLib_MemCpy(&buf[FSMSW_SPHINCSSHAKE_192SSIMPLE_N], addr, FSMSW_SPHINCSSHAKE_192SSIMPLE_ADDR_BYTES);
  FsmSw_CommonLib_MemCpy(&buf[FSMSW_SPHINCSSHAKE_192SSIMPLE_N + FSMSW_SPHINCSSHAKE_192SSIMPLE_ADDR_BYTES], in,
                         inblocks * FSMSW_SPHINCSSHAKE_192SSIMPLE_N);

  FsmSw_Fips202_Shake256(out, FSMSW_SPHINCSSHAKE_192SSIMPLE_N, buf,
                         FSMSW_SPHINCSSHAKE_192SSIMPLE_N + FSMSW_SPHINCSSHAKE_192SSIMPLE_ADDR_BYTES +
                             (inblocks * FSMSW_SPHINCSSHAKE_192SSIMPLE_N));
} // end: FsmSw_SphincsShake_192sSimple_Thash

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */