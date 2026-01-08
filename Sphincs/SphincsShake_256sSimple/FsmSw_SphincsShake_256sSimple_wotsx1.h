/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup SphincsShake_256sSimple
*    includes the modules for SphincsShake_256ssSimple
 ** @{ */
/** \addtogroup SphincsShake_256sSimple_wotsx1
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_SphincsShake_256sSimple_wotsx1.h
* \brief  Description of FsmSw_SphincsShake_256sSimple_wotsx1.h
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
#ifndef FSMSW_SPHINCSSHAKE_256SSIMPLE_WOTSX1_H
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_WOTSX1_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_SphincsShake_256sSimple_context.h"
#include "FsmSw_SphincsShake_256sSimple_params.h"
#include "FsmSw_Types.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
#define FSMSW_SPHINCS_ADDR_SIZE 8
/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct
{
  uint8 *wots_sig;
  uint32 wots_sign_leaf; /* The index of the WOTS we're using to sign */
  uint32 *wots_steps;
  uint32 leaf_addr[FSMSW_SPHINCS_ADDR_SIZE];
  uint32 pk_addr[FSMSW_SPHINCS_ADDR_SIZE];
} Fsmsw_Sphincsshake_256sSimple_LeafInfoX1_T;
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
void FsmSw_SphincsShake_256sSimple_Wots_GenLeafX1(uint8 *const dest, const sphincs_shake_256s_ctx *const ctx,
                                                  uint32 leaf_idx, void *const v_info);

#endif /* FSMSW_SPHINCSSHAKE_256SSIMPLE_WOTSX1_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */