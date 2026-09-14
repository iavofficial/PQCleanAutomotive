/***********************************************************************************************************************
 *
 * Original implementation: PQClean, HQC
 *
 * Copyright 2026 IAV GmbH
 *
 * The upstream PQClean repository identifies the original HQC
 * implementation as "Public Domain". No complete upstream license text
 * or explicit CC0 reference is provided.
 * IAV modifications are licensed under the Apache License, Version 2.0.
 *
 * SPDX-License-Identifier: LicenseRef-PQClean-HQC-Public-Domain AND Apache-2.0
 *
 **********************************************************************************************************************/

/** \addtogroup SwC Hqc
*    includes the modules for SwC Hqc
 ** @{ */
/** \addtogroup Hqc128
*    includes the modules for Hqc128
 ** @{ */
/** \addtogroup Hqc128_code
 ** @{ */

/*====================================================================================================================*/
/** \file Hqc128_code.c
* \brief Implementation of concatenated code
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
#include "Hqc128_parameters.h"
#include "Hqc128_reed_muller.h"
#include "Hqc128_reed_solomon.h"
#include "Platform_Types.h"

#include "Hqc128_code.h"

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
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
* \brief Encoding the message m to a code word em using the concatenated code
*
* First we encode the message using the Reed-Solomon code, then with the duplicated Reed-Muller code we obtain
* a concatenated code word.
*
* \param[out]   em :  Pointer to an array that is the tensor code word
* \param[in]    m  :  Pointer to an array that is the message
*
*
*/
void Hqc128_Code_Encode(uint64 *const em, const uint8 *const message)
{
  uint8 tmp[HQC128_VEC_N1_SIZE_BYTES] = {0};

  Hqc128_Reed_Solomon_Encode(tmp, message);
  Hqc128_Reed_Muller_Encode(em, tmp);
} // end: Hqc128_Code_Encode

/*====================================================================================================================*/
/**
* \brief Decoding the code word em to a message m using the concatenated code
*
*
* \param[out] m Pointer to an array that is the message
* \param[in] em Pointer to an array that is the code word
*
*
*/
void Hqc128_Code_Decode(uint8 *const m, const uint64 *const em)
{
  uint8 tmp[HQC128_VEC_N1_SIZE_BYTES] = {0};

  Hqc128_Reed_Muller_Decode(tmp, em);
  Hqc128_Reed_Solomon_Decode(m, tmp);
} // end: Hqc128_Code_Decode

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */