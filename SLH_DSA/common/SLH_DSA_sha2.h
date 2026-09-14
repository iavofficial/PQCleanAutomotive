/***********************************************************************************************************************
 * SLH-DSA names for the shared SHA-2 interface.
 **********************************************************************************************************************/
#ifndef SLH_DSA_SHA2_H
#define SLH_DSA_SHA2_H

#include "FsmSw_sha2.h"

#define SLH_DSA_Sha256                 FsmSw_Sha256
#define SLH_DSA_Sha256_IncBlocks       FsmSw_Sha256_IncBlocks
#define SLH_DSA_Sha256_IncCtxClone     FsmSw_Sha256_IncCtxClone
#define SLH_DSA_Sha256_IncFinalize     FsmSw_Sha256_IncFinalize
#define SLH_DSA_Sha256_IncInit         FsmSw_Sha256_IncInit
#define SLH_DSA_Sha512                 FsmSw_Sha512
#define SLH_DSA_Sha512_IncBlocks       FsmSw_Sha512_IncBlocks
#define SLH_DSA_Sha512_IncFinalize     FsmSw_Sha512_IncFinalize
#define SLH_DSA_Sha512_IncInit         FsmSw_Sha512_IncInit

#endif /* SLH_DSA_SHA2_H */
