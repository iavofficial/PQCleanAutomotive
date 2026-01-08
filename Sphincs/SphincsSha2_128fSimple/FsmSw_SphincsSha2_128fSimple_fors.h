/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsSha2_128fSimple
*    includes the modules for SphincsSha2_128fSimple
 ** @{ */
/** \addtogroup SphincsSha2_128fSimple_fors
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsSha2_128fSimple_fors.h
* \brief  Description of FsmSw_SphincsSha2_128fSimple_fors.h
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
#ifndef FSMSW_SPHINCSSHA2_128FSIMPLE_FORS_H
#define FSMSW_SPHINCSSHA2_128FSIMPLE_FORS_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_SphincsSha2_128fSimple_context.h"
#include "FsmSw_SphincsSha2_128fSimple_params.h"
#include "FsmSw_Types.h"
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
/**
 * Signs a message m, deriving the secret key from sk_seed and the FTS address.
 * Assumes m contains at least FSMSW_SPHINCSSHA2_128FSIMPLE_FORS_HEIGHT * FSMSW_SPHINCSSHA2_128FSIMPLE_FORS_TREES bits.
 */
void FsmSw_SphincsSha2_128fSimple_Fors_Sign(uint8 *const sig, uint8 *const pk, const uint8 *const m,
                                            const sphincs_sha2_128f_ctx *const ctx, const uint32 fors_addr[8]);

/**
 * Derives the FORS public key from a signature.
 * This can be used for verification by comparing to a known public key, or to
 * subsequently verify a signature on the derived public key. The latter is the
 * typical use-case when used as an FTS below an OTS in a hypertree.
 * Assumes m contains at least FSMSW_SPHINCSSHA2_128FSIMPLE_FORS_HEIGHT * FSMSW_SPHINCSSHA2_128FSIMPLE_FORS_TREES bits.
 */
/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsSha2_128fSimple_Fors_PkFromSig(uint8 *const pk, const uint8 *const sig, const uint8 *const m,
                                                 const sphincs_sha2_128f_ctx *const ctx, const uint32 fors_addr[8]);

#endif /* FSMSW_SPHINCSSHA2_128FSIMPLE_FORS_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */