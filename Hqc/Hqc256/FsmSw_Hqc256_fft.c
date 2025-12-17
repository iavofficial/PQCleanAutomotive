/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Hqc256
*    includes the modules for Hqc256
 ** @{ */
/** \addtogroup FsmSw_Hqc256_fft
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Hqc256_fft.c
* \brief Implementation of the additive FFT and its transpose.
 * This implementation is based on the paper from Gao and Mateer: <br>
 * Shuhong Gao and Todd Mateer, Additive Fast Fourier Transforms over Finite Fields,
 * IEEE Transactions on Information Theory 56 (2010), 6265--6272.
 * polyspace +1 MISRA2012:3.1 [Justified:]"The comment is a link and therefore contains a slash" 
 * http://www.math.clemson.edu/~sgao/papers/GM10.pdf <br>
 * and includes improvements proposed by Bernstein, Chou and Schwabe here:
 * polyspace +1 MISRA2012:3.1 [Justified:]"The comment is a link and therefore contains a slash" 
 * https://binary.cr.yp.to/mcbits-20130616.pdf
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
#include "FsmSw_CommonLib.h"
#include "FsmSw_Hqc256_gf.h"
#include "FsmSw_Hqc256_parameters.h"
#include "Platform_Types.h"

#include "FsmSw_Hqc256_fft.h"
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
/**
 * Logarithm of elements of GF(2^8) to the base alpha (root of 1 + x^2 + x^3 + x^4 + x^8).
 * The logarithm of 0 is set to 0 by convention.
 */
static const uint16 gf_log_256[256] = {
    0,   0,   1,   25,  2,   50,  26,  198, 3,   223, 51,  238, 27,  104, 199, 75,  4,   100, 224, 14,  52,  141,
    239, 129, 28,  193, 105, 248, 200, 8,   76,  113, 5,   138, 101, 47,  225, 36,  15,  33,  53,  147, 142, 218,
    240, 18,  130, 69,  29,  181, 194, 125, 106, 39,  249, 185, 201, 154, 9,   120, 77,  228, 114, 166, 6,   191,
    139, 98,  102, 221, 48,  253, 226, 152, 37,  179, 16,  145, 34,  136, 54,  208, 148, 206, 143, 150, 219, 189,
    241, 210, 19,  92,  131, 56,  70,  64,  30,  66,  182, 163, 195, 72,  126, 110, 107, 58,  40,  84,  250, 133,
    186, 61,  202, 94,  155, 159, 10,  21,  121, 43,  78,  212, 229, 172, 115, 243, 167, 87,  7,   112, 192, 247,
    140, 128, 99,  13,  103, 74,  222, 237, 49,  197, 254, 24,  227, 165, 153, 119, 38,  184, 180, 124, 17,  68,
    146, 217, 35,  32,  137, 46,  55,  63,  209, 91,  149, 188, 207, 205, 144, 135, 151, 178, 220, 252, 190, 97,
    242, 86,  211, 171, 20,  42,  93,  158, 132, 60,  57,  83,  71,  109, 65,  162, 31,  45,  67,  216, 183, 123,
    164, 118, 196, 23,  73,  236, 127, 12,  111, 246, 108, 161, 59,  82,  41,  157, 85,  170, 251, 96,  134, 177,
    187, 204, 62,  90,  203, 89,  95,  176, 156, 169, 160, 81,  11,  245, 22,  235, 122, 117, 44,  215, 79,  174,
    213, 233, 230, 231, 173, 232, 116, 214, 244, 234, 168, 80,  88,  175};

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/

static void radix_big_256(uint16 *const f0, uint16 *const f1, const uint16 *const f, uint32 m_f);

/**********************************************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
 * \brief Computes the basis of betas (omitting 1) used in the additive FFT and its transpose
 *
 * \param[out] betas Array of size HQC256_PARAM_M-1
 */
static void hqc256_compute_fft_betas(uint16 *const betas)
{
  uint8 i;
  for (i = 0; i < (HQC256_PARAM_M - 1); ++i)
  {
    betas[i] = (uint16)1 << (HQC256_PARAM_M - 1 - i);
  }
} // end: compute_fft_betas

/*====================================================================================================================*/
/**
 * \brief Computes the subset sums of the given set
 *
 * The array subset_sums is such that its ith element is
 * the subset sum of the set elements given by the binary form of i.
 *
 * \param[out] subset_sums Array of size 2^set_size receiving the subset sums
 * \param[in] set Array of set_size elements
 * \param[in] set_size Size of the array set
 */
static void compute_subset_sums_256(uint16 *const subset_sums, const uint16 *const set, uint16 set_size)
{
  uint16 i, j;
  subset_sums[0] = 0;

  for (i = 0; i < set_size; ++i)
  {
    for (j = 0; j < ((uint16)1 << i); ++j)
    {
      subset_sums[((uint16)1 << i) + j] = set[i] ^ subset_sums[j];
    }
  }
} // end: compute_subset_sums

/*====================================================================================================================*/
/**
 * \brief Computes the hqc256_radix conversion of a polynomial f in GF(2^m)[x]
 *
 * Computes f0 and f1 such that f(x) = f0(x^2-x) + x.f1(x^2-x)
 * as proposed by Bernstein, Chou and Schwabe:
 * polyspace +1 MISRA2012:3.1 [Justified:]"The comment is a link and therefore contains a slash" 
 * https://binary.cr.yp.to/mcbits-20130616.pdf
 *
 * \param[out] f0 Array half the size of f
 * \param[out] f1 Array half the size of f
 * \param[in] f Array of size a power of 2
 * \param[in] m_f 2^{m_f} is the smallest power of 2 greater or equal to the number of coefficients of f
 */
/* polyspace +2 MISRA2012:17.2 [Justified:]"Without in-depth knowledge, this violation cannot be resolved." */
/* polyspace +1 CERT-C:MEM05-C [Justified:]"Without in-depth knowledge, this violation cannot be resolved." */
static void hqc256_radix(uint16 *const f0, uint16 *const f1, const uint16 *const f, uint32 m_f)
{
  if (m_f == 4U)
  {
    f0[4] = f[8] ^ f[12];
    f0[6] = f[12] ^ f[14];
    f0[7] = f[14] ^ f[15];
    f1[5] = f[11] ^ f[13];
    f1[6] = f[13] ^ f[14];
    f1[7] = f[15];
    f0[5] = f[10] ^ f[12] ^ f1[5];
    f1[4] = f[9] ^ f[13] ^ f0[5];

    f0[0] = f[0];
    f1[3] = f[7] ^ f[11] ^ f[15];
    f0[3] = f[6] ^ f[10] ^ f[14] ^ f1[3];
    f0[2] = f[4] ^ f0[4] ^ f0[3] ^ f1[3];
    f1[1] = f[3] ^ f[5] ^ f[9] ^ f[13] ^ f1[3];
    f1[2] = f[3] ^ f1[1] ^ f0[3];
    f0[1] = f[2] ^ f0[2] ^ f1[1];
    f1[0] = f[1] ^ f0[1];
  }
  else if (m_f == 3U)
  {
    f0[0] = f[0];
    f0[2] = f[4] ^ f[6];
    f0[3] = f[6] ^ f[7];
    f1[1] = f[3] ^ f[5] ^ f[7];
    f1[2] = f[5] ^ f[6];
    f1[3] = f[7];
    f0[1] = f[2] ^ f0[2] ^ f1[1];
    f1[0] = f[1] ^ f0[1];
  }
  else if (m_f == 2U)
  {
    f0[0] = f[0];
    f0[1] = f[2] ^ f[3];
    f1[0] = f[1] ^ f0[1];
    f1[1] = f[3];
  }
  else if (m_f == 1U)
  {
    f0[0] = f[0];
    f1[0] = f[1];
  }
  else
  {
    radix_big_256(f0, f1, f, m_f);
  }
} // end: radix

/*====================================================================================================================*/
/* polyspace +2 MISRA2012:17.2 [Justified:]"Without in-depth knowledge, this violation cannot be resolved." */
/* polyspace +1 CERT-C:MEM05-C [Justified:]"Without in-depth knowledge, this violation cannot be resolved." */
static void radix_big_256(uint16 *const f0, uint16 *const f1, const uint16 *const f, uint32 m_f)
{
  uint16 Q[(2 * (1U << (HQC256_PARAM_FFT - 2))) + 1] = {0};
  uint16 R[(2 * (1U << (HQC256_PARAM_FFT - 2))) + 1] = {0};

  uint16 Q0[1U << (HQC256_PARAM_FFT - 2)] = {0};
  uint16 Q1[1U << (HQC256_PARAM_FFT - 2)] = {0};
  uint16 R0[1U << (HQC256_PARAM_FFT - 2)] = {0};
  uint16 R1[1U << (HQC256_PARAM_FFT - 2)] = {0};

  uint64 i, n;

  n = 1;
  n <<= (m_f - 2);
  FsmSw_CommonLib_MemCpy(Q, &f[3 * n], FsmSw_Convert_u64_to_u32(2 * n));
  FsmSw_CommonLib_MemCpy(&Q[n], &f[3 * n], FsmSw_Convert_u64_to_u32(2 * n));
  FsmSw_CommonLib_MemCpy(R, f, FsmSw_Convert_u64_to_u32(4 * n));

  for (i = 0; i < n; ++i)
  {
    Q[i] ^= f[(2 * n) + i];
    R[n + i] ^= Q[i];
  }

  hqc256_radix(Q0, Q1, Q, m_f - 1);
  hqc256_radix(R0, R1, R, m_f - 1);

  FsmSw_CommonLib_MemCpy(f0, R0, FsmSw_Convert_u64_to_u32(2 * n));
  FsmSw_CommonLib_MemCpy(&f0[n], Q0, FsmSw_Convert_u64_to_u32(2 * n));
  FsmSw_CommonLib_MemCpy(f1, R1, FsmSw_Convert_u64_to_u32(2 * n));
  FsmSw_CommonLib_MemCpy(&f1[n], Q1, FsmSw_Convert_u64_to_u32(2 * n));
} // end: radix_big

/*====================================================================================================================*/
/**
 * \brief Evaluates f at all subset sums of a given set
 *
 * This function is a subroutine of the function FsmSw_Hqc256_fft.
 *
 * \param[out] w Array
 * \param[in] f Array
 * \param[in] f_coeffs Number of coefficients of f
 * \param[in] m Number of betas
 * \param[in] m_f Number of coefficients of f (one more than its degree)
 * \param[in] betas FFT constants
 */
static void fft_rec_256(uint16 *const w, uint16 *const f, uint32 f_coeffs, uint8 m, uint32 m_f,
                        const uint16 *const betas)
{
  uint16 f0[1U << (HQC256_PARAM_FFT - 2)]             = {0};
  uint16 f1[1U << (HQC256_PARAM_FFT - 2)]             = {0};
  uint16 gammas[HQC256_PARAM_M - 2]                   = {0};
  uint16 deltas[HQC256_PARAM_M - 2]                   = {0};
  uint16 gammas_sums[1U << (HQC256_PARAM_M - 2)]      = {0};
  uint16 u[1U << (HQC256_PARAM_M - 2)]                = {0};
  uint16 v[1U << (HQC256_PARAM_M - 2)]                = {0};
  uint16 tmp[HQC256_PARAM_M - (HQC256_PARAM_FFT - 1)] = {0};

  uint16 beta_m_pow;
  uint8 i;
  uint64 k, x;

  // Step 1
  if (m_f == 1)
  {
    for (uint8 y = 0; y < m; ++y)
    {
      tmp[y] = FsmSw_Hqc256_Gf_Mul(betas[y], f[1]);
    }

    w[0] = f[0];
    x    = 1;
    for (uint8 z = 0; z < m; ++z)
    {
      for (k = 0; k < x; ++k)
      {
        w[x + k] = w[k] ^ tmp[z];
      }
      x <<= 1;
    }
  }
  else
  {
    // Step 2: compute g
    if (betas[m - 1] != 1)
    {
      beta_m_pow = 1;
      x          = 1;
      x <<= m_f;
      for (i = 1; i < x; ++i)
      {
        beta_m_pow = FsmSw_Hqc256_Gf_Mul(beta_m_pow, betas[m - 1]);
        f[i]       = FsmSw_Hqc256_Gf_Mul(beta_m_pow, f[i]);
      }
    }

    // Step 3
    hqc256_radix(f0, f1, f, m_f);

    // Step 4: compute gammas and deltas
    for (uint8 y = 0; y < (m - 1); ++y)
    {
      gammas[y] = FsmSw_Hqc256_Gf_Mul(betas[y], FsmSw_Hqc256_Gf_Inverse(betas[m - 1]));
      deltas[y] = FsmSw_Hqc256_Gf_Square(gammas[y]) ^ gammas[y];
    }

    // Compute gammas sums
    compute_subset_sums_256(gammas_sums, gammas, FsmSw_Convert_u8_to_u16(m - 1));

    // Step 5
    /* polyspace +2 MISRA2012:17.2 [Justified:]"Without in-depth knowledge, this violation cannot be resolved." */
    /* polyspace +1 CERT-C:MEM05-C [Justified:]"Without in-depth knowledge, this violation cannot be resolved." */
    fft_rec_256(u, f0, (f_coeffs + 1) / 2, m - 1, m_f - 1, deltas);

    k = 1;
    k <<= ((m - 1) & (uint8)0xf); // &0xf is to let the compiler know that m-1 is small.
    if (f_coeffs <= 3)
    { // 3-coefficient polynomial f case: f1 is constant
      w[0] = u[0];
      w[k] = u[0] ^ f1[0];
      for (i = 1; i < k; ++i)
      {
        w[i]     = u[i] ^ FsmSw_Hqc256_Gf_Mul(gammas_sums[i], f1[0]);
        w[k + i] = w[i] ^ f1[0];
      }
    }
    else
    {
      /* polyspace +2 MISRA2012:17.2 [Justified:]"Without in-depth knowledge, this violation cannot be resolved." */
      /* polyspace +1 CERT-C:MEM05-C [Justified:]"Without in-depth knowledge, this violation cannot be resolved." */
      fft_rec_256(v, f1, f_coeffs / 2, m - 1, m_f - 1, deltas);

      // Step 6
      FsmSw_CommonLib_MemCpy(&w[k], v, FsmSw_Convert_u64_to_u32(2 * k));
      w[0] = u[0];
      w[k] ^= u[0];
      for (i = 1; i < k; ++i)
      {
        w[i] = u[i] ^ FsmSw_Hqc256_Gf_Mul(gammas_sums[i], v[i]);
        w[k + i] ^= w[i];
      }
    }
  }
} // end: fft_rec

/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
 * \brief Evaluates f on all fields elements using an additive FFT algorithm
 *
 * f_coeffs is the number of coefficients of f (one less than its degree). <br>
 * The FFT proceeds recursively to evaluate f at all subset sums of a basis B. <br>
 * This implementation is based on the paper from Gao and Mateer: <br>
 * Shuhong Gao and Todd Mateer, Additive Fast Fourier Transforms over Finite Fields,
 * IEEE Transactions on Information Theory 56 (2010), 6265--6272.
 * polyspace +1 MISRA2012:3.1 [Justified:]"The comment is a link and therefore contains a slash" 
 * http://www.math.clemson.edu/~sgao/papers/GM10.pdf <br>
 * and includes improvements proposed by Bernstein, Chou and Schwabe here:
 * polyspace +1 MISRA2012:3.1 [Justified:]"The comment is a link and therefore contains a slash" 
 * https://binary.cr.yp.to/mcbits-20130616.pdf <br>
 * Note that on this first call (as opposed to the recursive calls to fft_rec_256), gammas are equal to betas,
 * meaning the first gammas subset sums are actually the subset sums of betas (except 1). <br>
 * Also note that f is altered during computation (twisted at each level).
 *
 * \param[out] w Array
 * \param[in] f Array of 2^HQC256_PARAM_FFT elements
 * \param[in] f_coeffs Number coefficients of f (i.e. deg(f)+1)
 */
void FsmSw_Hqc256_Fft(uint16 *const w, const uint16 *const f, uint32 f_coeffs)
{
  uint16 betas[HQC256_PARAM_M - 1]              = {0};
  uint16 betas_sums[1U << (HQC256_PARAM_M - 1)] = {0};
  uint16 f0[1U << (HQC256_PARAM_FFT - 1)]       = {0};
  uint16 f1[1U << (HQC256_PARAM_FFT - 1)]       = {0};
  uint16 deltas[HQC256_PARAM_M - 1]             = {0};
  uint16 u[1U << (HQC256_PARAM_M - 1)]          = {0};
  uint16 v[1U << (HQC256_PARAM_M - 1)]          = {0};

  uint8 i, k;

  // Follows Gao and Mateer algorithm
  hqc256_compute_fft_betas(betas);

  // Step 1: HQC256_PARAM_FFT > 1, nothing to do

  // Compute gammas sums
  compute_subset_sums_256(betas_sums, betas, HQC256_PARAM_M - 1);

  // Step 2: beta_m = 1, nothing to do

  // Step 3
  hqc256_radix(f0, f1, f, HQC256_PARAM_FFT);

  // Step 4: Compute deltas
  for (i = 0; i < (HQC256_PARAM_M - 1); ++i)
  {
    deltas[i] = FsmSw_Hqc256_Gf_Square(betas[i]) ^ betas[i];
  }

  // Step 5
  fft_rec_256(u, f0, (f_coeffs + 1) / 2, HQC256_PARAM_M - 1, HQC256_PARAM_FFT - 1, deltas);
  fft_rec_256(v, f1, f_coeffs / 2, HQC256_PARAM_M - 1, HQC256_PARAM_FFT - 1, deltas);

  k = (uint8)1 << (HQC256_PARAM_M - 1);
  // Step 6, 7 and error polynomial computation
  FsmSw_CommonLib_MemCpy(&w[k], v, (2 * FsmSw_Convert_u8_to_u32(k)));

  // Check if 0 is root
  w[0] = u[0];

  // Check if 1 is root
  w[k] ^= u[0];

  // Find other roots
  for (i = 1; i < k; ++i)
  {
    w[i] = u[i] ^ FsmSw_Hqc256_Gf_Mul(betas_sums[i], v[i]);
    w[k + i] ^= w[i];
  }
} // end: FsmSw_Hqc256_Fft

/*====================================================================================================================*/
/**
 * \brief Retrieves the error polynomial error from the evaluations w of the ELP (Error Locator Polynomial) on all field elements.
 *
 * \param[out] err Array with the error
 * \param[out] error_compact Array with the error in a compact form
 * \param[in] w Array of size 2^HQC256_PARAM_M
 */
void FsmSw_Hqc256_Fft_Retrieve_Error_Poly(uint8 *const err, const uint16 *const w)
{
  uint16 gammas[HQC256_PARAM_M - 1]              = {0};
  uint16 gammas_sums[1U << (HQC256_PARAM_M - 1)] = {0};
  uint16 k;
  uint16 i, index;

  hqc256_compute_fft_betas(gammas);
  compute_subset_sums_256(gammas_sums, gammas, HQC256_PARAM_M - 1);

  k = 1U << (HQC256_PARAM_M - 1);
  err[0] ^= (uint8)((1U ^ (((uint16)(~w[0]) + 1U) >> 15)) & 0xFFU);
  err[0] ^= (uint8)((1U ^ (((uint16)(~w[k]) + 1U) >> 15)) & 0xFFU);

  for (i = 1; i < k; ++i)
  {
    index = HQC256_PARAM_GF_MUL_ORDER - gf_log_256[gammas_sums[i]];
    err[index] ^= (uint8)((1U ^ (((uint16)(~w[i]) + 1U) >> 15)) & 0xFFU);

    index = HQC256_PARAM_GF_MUL_ORDER - gf_log_256[gammas_sums[i] ^ 1U];
    err[index] ^= (uint8)((1U ^ (((uint16)(~w[k + i]) + 1U) >> 15)) & 0xFFU);
  }
} // end: FsmSw_Hqc256_Fft_Retrieve_Error_Poly

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */