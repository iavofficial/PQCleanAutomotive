/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsSha2_128sSimple
*    includes the modules for SphincsSha2_128sSimple
 ** @{ */
/** \addtogroup SphincsSha2_128sSimple_wotsx1
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsSha2_128sSimple_wotsx1.h
* \brief  Description of FsmSw_SphincsSha2_128sSimple_wotsx1.h
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
#ifndef FSMSW_SPHINCSSHA2_128SSIMPLE_WOTSX1_H
#define FSMSW_SPHINCSSHA2_128SSIMPLE_WOTSX1_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_SphincsSha2_128sSimple_context.h"
#include "FsmSw_SphincsSha2_128sSimple_params.h"
#include "FsmSw_Types.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define FSMSW_SPHINCS_WOTSX1_ADDR_SIZE 8
/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct
{
  uint8 *wots_sig;
  uint32 wots_sign_leaf; /* The index of the WOTS we're using to sign */
  uint32 *wots_steps;
  uint32 leaf_addr[FSMSW_SPHINCS_WOTSX1_ADDR_SIZE];
  uint32 pk_addr[FSMSW_SPHINCS_WOTSX1_ADDR_SIZE];
} Fsmsw_Sphincssha2_128sSimple_LeafInfoX1_T;
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
/* polyspace +6 CERT-C:DCL23-C [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsSha2_128sSimple_Wots_GenLeafX1(uint8 *const dest, const sphincs_sha2_128s_ctx *const ctx,
                                                 uint32 leaf_idx, void *const v_info);

#endif /* FSMSW_SPHINCSSHA2_128SSIMPLE_WOTSX1_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */