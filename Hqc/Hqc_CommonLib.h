/***********************************************************************************************************************
 *
 * Copyright 2026 IAV GmbH
 *
 * This file is licensed under the Apache License, Version 2.0.
 * See the LICENSE file in the repository root for the complete license text.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 **********************************************************************************************************************/

/* HQC names for the shared common-library interface. */
#ifndef HQC_COMMONLIB_H
#define HQC_COMMONLIB_H

#include "FsmSw_CommonLib.h"

#define Hqc_CommonLib_MemCmp      FsmSw_CommonLib_MemCmp
#define Hqc_CommonLib_MemCpy      FsmSw_CommonLib_MemCpy
#define Hqc_CommonLib_MemMove     FsmSw_CommonLib_MemMove
#define Hqc_CommonLib_MemSet      FsmSw_CommonLib_MemSet
#define Hqc_CommonLib_RandomBytes FsmSw_CommonLib_RandomBytes
#define Hqc_Convert_u32_to_u64    FsmSw_Convert_u32_to_u64
#define Hqc_Convert_u16_to_u64    FsmSw_Convert_u16_to_u64
#define Hqc_Convert_u8_to_u64     FsmSw_Convert_u8_to_u64
#define Hqc_Convert_u8_to_u32     FsmSw_Convert_u8_to_u32
#define Hqc_Convert_u64_to_u32    FsmSw_Convert_u64_to_u32
#define Hqc_Convert_u16_to_u32    FsmSw_Convert_u16_to_u32
#define Hqc_Convert_u8_to_u16     FsmSw_Convert_u8_to_u16
#define Hqc_Convert_u16_to_u8     FsmSw_Convert_u16_to_u8
#define Hqc_Convert_u32_to_u16    FsmSw_Convert_u32_to_u16
#define Hqc_GetSignBit_i16        FsmSw_GetSignBit_i16

#endif /* HQC_COMMONLIB_H */
