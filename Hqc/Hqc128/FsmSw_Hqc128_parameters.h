/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup HQC128
*    includes the modules for HQC128
 ** @{ */
/** \addtogroup FsmSw_Hqc128_parameters
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Hqc128_parameters.h
* \brief  Parameters of the HQC_KEM IND-CCA2 schemec
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
#ifndef HQC_PARAMETERS_H
#define HQC_PARAMETERS_H
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Hqc128_kem.h"
/**********************************************************************************************************************/
/* GLOBAL DEFINES                                                                                                     */
/**********************************************************************************************************************/
/* polyspace +4 MISRA2012:D4.9 [Justified:]"No refactoring of macros, as converting to, for example, 
inline functions would not provide significant benefits." */
/* polyspace +2 CERT-C:PRE00-C [Justified:]"No refactoring of macros, as converting to, for example, 
inline functions would not provide significant benefits." */
#define CEIL_DIVIDE(a, b) (((a) + (b) - 1) / (b)) /*!< Divide a by b and ceil the result*/

/*
  #define PARAM_N                               Define the parameter n of the scheme
  #define PARAM_N1                              Define the parameter n1 of the scheme (length of Reed-Solomon code)
  #define PARAM_N2                              Define the parameter n2 of the scheme (length of Duplicated Reed-Muller code)
  #define PARAM_N1N2                            Define the length in bits of the Concatenated code
  #define PARAM_OMEGA                           Define the parameter omega of the scheme
  #define HQC128_PARAM_OMEGA_E                         Define the parameter omega_e of the scheme
  #define HQC128_PARAM_OMEGA_R                         Define the parameter omega_r of the scheme

  #define HQC128_SECRET_KEY_BYTES                      Define the size of the secret key in bytes
  #define HQC128_PUBLIC_KEY_BYTES                      Define the size of the public key in bytes
  #define HQC128_SHARED_SECRET_BYTES                   Define the size of the shared secret in bytes
  #define HQC128_CIPHERTEXT_BYTES                      Define the size of the ciphertext in bytes

  #define HQC128_VEC_N_SIZE_BYTES                      Define the size of the array used to store a PARAM_N sized vector in bytes
  #define HQC128_VEC_K_SIZE_BYTES                      Define the size of the array used to store a HQC128_PARAM_K sized vector in bytes
  #define HQC128_VEC_N1_SIZE_BYTES                     Define the size of the array used to store a PARAM_N1 sized vector in bytes
  #define HQC128_VEC_N1N2_SIZE_BYTES                   Define the size of the array used to store a PARAM_N1N2 sized vector in bytes

  #define HQC128_VEC_N_SIZE_64                         Define the size of the array used to store a PARAM_N sized vector in 64 bits
  #define HQC128_VEC_K_SIZE_64                         Define the size of the array used to store a HQC128_PARAM_K sized vector in 64 bits
  #define HQC128_VEC_N1_SIZE_64                        Define the size of the array used to store a PARAM_N1 sized vector in 64 bits
  #define HQC128_VEC_N1N2_SIZE_64                      Define the size of the array used to store a PARAM_N1N2 sized vector in 64 bits

  #define HQC128_PARAM_DELTA                           Define the parameter delta of the scheme (correcting capacity of the Reed-Solomon code)
  #define HQC128_PARAM_M                               Define a positive integer
  #define HQC128_PARAM_GF_POLY                         Generator polynomial of galois field GF(2^HQC128_PARAM_M), represented in hexadecimial form
  #define HQC128_PARAM_GF_POLY_WT                      Hamming weight of HQC128_PARAM_GF_POLY
  #define HQC128_PARAM_GF_POLY_M2                      Distance between the primitive polynomial first two set bits
  #define HQC128_PARAM_GF_MUL_ORDER                    Define the size of the multiplicative group of GF(2^HQC128_PARAM_M),  i.e 2^HQC128_PARAM_M -1
  #define HQC128_PARAM_K                               Define the size of the information bits of the Reed-Solomon code
  #define HQC128_PARAM_G                               Define the size of the generator polynomial of Reed-Solomon code
  #define HQC128_PARAM_FFT                             The additive FFT takes a 2^HQC128_PARAM_FFT polynomial as input
                                                We use the FFT to compute the roots of sigma, whose degree if HQC128_PARAM_DELTA=24
                                                The smallest power of 2 greater than 24+1 is 32=2^5
  #define HQC128_RS_POLY_COEFS                         Coefficients of the generator polynomial of the Reed-Solomon code

  #define HQC128_RED_MASK                              A mask for the higher bits of a vector
  #define HQC128_SHAKE256_512_BYTES                    Define the size of SHAKE-256 output in bytes
  #define HQC128_SEED_BYTES                            Define the size of the seed in bytes
  #define HQC128_SALT_SIZE_BYTES                       Define the size of a salt in bytes
*/

#define HQC128_PARAM_N       17669
#define HQC128_PARAM_N1      46
#define HQC128_PARAM_N2      384
#define HQC128_PARAM_N1N2    17664
#define HQC128_PARAM_OMEGA   66
#define HQC128_PARAM_OMEGA_E 75
#define HQC128_PARAM_OMEGA_R 75

#define HQC128_PUBLIC_KEY_BYTES PQCLEAN_HQC128_CLEAN_CRYPTO_PUBLICKEYBYTES

#define HQC128_VEC_N_SIZE_BYTES    CEIL_DIVIDE(HQC128_PARAM_N, 8)
#define HQC128_VEC_K_SIZE_BYTES    HQC128_PARAM_K
#define HQC128_VEC_N1_SIZE_BYTES   HQC128_PARAM_N1
#define HQC128_VEC_N1N2_SIZE_BYTES CEIL_DIVIDE(HQC128_PARAM_N1N2, 8)

#define HQC128_VEC_N_SIZE_64    CEIL_DIVIDE(HQC128_PARAM_N, 64)
#define HQC128_VEC_N1N2_SIZE_64 CEIL_DIVIDE(HQC128_PARAM_N1N2, 64)

#define HQC128_PARAM_DELTA        15
#define HQC128_PARAM_M            8
#define HQC128_PARAM_GF_POLY      0x11D
#define HQC128_PARAM_GF_POLY_WT   5
#define HQC128_PARAM_GF_POLY_M2   4
#define HQC128_PARAM_GF_MUL_ORDER 255
#define HQC128_PARAM_K            16
#define HQC128_PARAM_G            31
#define HQC128_PARAM_FFT          4
#define HQC128_RS_POLY_COEFS                                                                                           \
  89, 69, 153, 116, 176, 117, 111, 75, 73, 233, 242, 233, 65, 210, 21, 139, 103, 173, 67, 118, 105, 210, 174, 110, 74, \
      69, 228, 82, 255, 181, 1

#define HQC128_RED_MASK           0x1f
#define HQC128_SHAKE256_512_BYTES 64
#define HQC128_SEED_BYTES         40
#define HQC128_SALT_SIZE_BYTES    16
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
#endif /* HQC_PARAMETERS_H */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
