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

/* SLH-DSA names for the shared FIPS 202 interface. */
#ifndef SLH_DSA_FIPS202_H
#define SLH_DSA_FIPS202_H

#include "FsmSw_Fips202.h"

#define SLH_DSA_Fips202_Shake256                  FsmSw_Fips202_Shake256
#define SLH_DSA_Fips202_Shake256_IncInit          FsmSw_Fips202_Shake256_IncInit
#define SLH_DSA_Fips202_Shake256_IncAbsorb        FsmSw_Fips202_Shake256_IncAbsorb
#define SLH_DSA_Fips202_Shake256_IncFinalize      FsmSw_Fips202_Shake256_IncFinalize
#define SLH_DSA_Fips202_Shake256_IncSqueeze       FsmSw_Fips202_Shake256_IncSqueeze

#endif /* SLH_DSA_FIPS202_H */
