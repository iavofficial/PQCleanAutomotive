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
/** \addtogroup HQC256
*    includes the modules for HQC256
 ** @{ */
/** \addtogroup Hqc256_parsing
 ** @{ */

/*====================================================================================================================*/
/** \file Hqc256_parsing.h
* \brief  Declarations for the modul Hqc256_parsing.c
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
#ifndef PARSING_H
#define PARSING_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
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

void Hqc256_Load8_Arr(uint64 *const out64, uint16 outlen, const uint8 *const in8, uint16 inlen);

void Hqc256_Store8_Arr(uint8 *const out8, uint16 outlen, const uint64 *const in64, uint16 inlen);

void Hqc256_Secret_Key_To_String(uint8 *const sk, const uint8 *const sk_seed, const uint8 *const sigma,
                                       const uint8 *const pk);

void Hqc256_Secret_Key_From_String(uint64 *const x, uint64 *const y, uint8 *const sigma, uint8 *const pk,
                                         const uint8 *const sk);

void Hqc256_Public_Key_To_String(uint8 *const pk, const uint8 *const pk_seed, const uint64 *const s);

void Hqc256_Public_Key_From_String(uint64 *const h, uint64 *const s, const uint8 *const pk);

void Hqc256_Ciphertext_To_String(uint8 *const ct, const uint64 *const u, const uint64 *const v,
                                       const uint8 *const salt);

void Hqc256_Ciphertext_From_String(uint64 *const u, uint64 *const v, uint8 *const salt, const uint8 *const ct);

#endif /* PARSING_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */