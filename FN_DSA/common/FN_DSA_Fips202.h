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

/* FN-DSA names for the shared FIPS 202 interface. */
#ifndef FN_DSA_FIPS202_H
#define FN_DSA_FIPS202_H

#include "FsmSw_Fips202.h"

#define FN_DSA_Fips202_Shake256_IncInit          FsmSw_Fips202_Shake256_IncInit
#define FN_DSA_Fips202_Shake256_IncAbsorb        FsmSw_Fips202_Shake256_IncAbsorb
#define FN_DSA_Fips202_Shake256_IncFinalize      FsmSw_Fips202_Shake256_IncFinalize
#define FN_DSA_Fips202_Shake256_IncSqueeze       FsmSw_Fips202_Shake256_IncSqueeze

#endif /* FN_DSA_FIPS202_H */
