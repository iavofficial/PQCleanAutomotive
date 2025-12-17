/***********************************************************************************************************************
 *
 *                                                    IAV GmbH
 *
 *
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Hqc128
*    includes the modules for Hqc128
 ** @{ */
/** \addtogroup FsmSw_Hqc128_reed_solomon
 ** @{ */

/*====================================================================================================================*/
/** \file FsmSw_Hqc128_reed_solomon.c
* \brief Constant time implementation of Reed-Solomon codes
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
#include "FsmSw_Hqc128_fft.h"
#include "FsmSw_Hqc128_gf.h"
#include "FsmSw_Hqc128_parameters.h"
#include "Platform_Types.h"

#include "FsmSw_Hqc128_reed_solomon.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define HQC128_X_SIGMA_P_2ND_ELEMENT_INDEX 1
/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL CONSTANTS                                                                                                   */
/**********************************************************************************************************************/
static const uint16 alpha_ij_pow_128[30][45] = {
    {  2,  4,    8,   16,   32,   64,  128,   29,  58, 116,   232,  205,  135,   19,    38,  76,  152,   45,   90, 180,   117,  234, 201,
     143,    3,    6,   12,   24,   48,   96,  192,  157,   39,   78, 156,   37,  74,  148,   53,  106,   212,  181,   119,  238,  193  },
    {  4,  16,   64,   29,  116,  205,   19,  76,  45,  180,   234,  143,    6, 24,   96,  157,   78,  37,  148,  106,   181,  238, 159,
     70,    5,  20,   80,   93, 105,  185,   222,   95,   97,  153,   94, 101,  137,   30,  120,  253,   211,  107,   177,  254,  223   },
    {  8, 64,   58, 205,   38,  45, 117,  143,   12,  96,   39,   37,   53, 181,   193,   70,  10,   80,  186,  185,   161,   97,  47,
     101,   15,  120,  231,  107,  127,  223,   182,  217,  134,    68,   26,  208,  206,   62, 237,   59,   197,  102,    23, 184,  169},
    { 16,  29,  205,   76, 180,  143,   24,  157,   37, 106,   238,   70,  20,  93,   185,   95, 153,  101,    30, 253,   107,  254,  91,
     217,   17,   13, 208,  129,  248,   59,   151,  133,  184,    79, 132,  168,   82,   73, 228,  230,   198,  252,   123,  227,  150 },
    { 32,  116,   38,  180,    3, 96,  156,  106,  193,    5, 160,  185,  190,   94,    15, 253,  214,  223,   226,   17,   26, 103, 124,
     59,   51,  46,  169,  132,   77,   85,  114,  230,  145,   215,  255,  150,   55,  174,  100,   28,  167,   89,  239,  172,   36   },
    { 64, 205,   45, 143,   96,  37, 181,   70,  80, 185,    97, 101,  120,  107,   223,  217,   68,  208,    62,  59,   102,  184,  33,
     168,   85, 228,  191,  252,  241,  150,   110,  130,    7, 221,   89, 195,  138,   61, 251,   44,   207,  173,     8,  58,  38     },
    {128,   19, 117,   24,  156,  181,  140,   93, 161,   94,   60,  107,  163,   67,    26, 129,  147,  102,   109,  132,    41,   57, 209,
     252,  255,   98,  87,  200,  224,   89,  155,   18, 245,    11,  233,  173,   16,  232,   45,   3, 157,   53,  159,   40,  185     },
    { 29,  76,  143,  157,  106,   70,  93,   95,  101,  253,   254,  217,   13,  129,    59, 133,   79, 168,    73, 230,   252,  227, 149,
     130,   28,   81, 195,   18, 247,   44,    27,    2, 58,  152,    3, 39,  212,  140,  186,  190,   202,  231,   225,  175,   26     },
    { 58,  45,   12,  37, 193,   80,  161,  101,  231,  223,   134,  208,  237,  102,   169,  168,  146,  191,   179,  150,    87,    7, 166,
     195,   36,  251,  125,  173,   64,   38,  143,   39, 181,    10, 185,   47, 120,  127,  217,   26,   62, 197,   184,   21,   85    },
    {116,  180,   96,  106,    5,  185,   94, 253,  223,   17,   103,   59,  46,  132,    85,  230,  215,  150,   174,   28,   89,  172, 244,
     44, 108,   32,   38,   3,  156,  193,   160,  190,   15,   214,  226,   26,  124,   51,  169,   77,  114,  145,   255,   55,  100  },
    {232,  234,   39, 238,  160,   97,   60,  254,  134,  103,   118,  184,   84,   57,   145,  227,  220,    7, 162,  172,   245,  176,  71,
     58, 180,  192,  181,   40,  95,   15,   177,  175,  208,   147,   46,  21,   73,   99, 241,   55,  200,  166,    43, 122,   44     },
    {205,  143,   37,  70,  185,  101,  107,  217,  208,   59,   184,  168,  228,  252,   150,  130,  221,  195,    61,   44,   173,   58, 117,
     39, 193,  186,   47,  231,  182,   26,  237,   23,  21,   146,  145,  219,   87,  56, 242,   36,  139,   54,    64,   45,  96      },
    {135,    6,  53,  20, 190,  120,  163,   13,  237,   46,   84, 228,  229,   98,   100,   81,  69, 251,   131,   32,   45, 192, 238,
     186,   94,  187,  217,  189,  236,  169,    82,  209,  241,   220,   28, 242,   72,   22, 173,  116,   201,   37,  140,  222,   15 },
    { 19,  24, 181,   93,  94, 107,   67, 129,  102,  132,    57,  252,   98,  200,    89,  18,   11,  173,   232,    3,   53,   40, 194,
     231,  226,  189,  197,  158,  170,  145,    75,  25, 166,    69,  235,   54,   29, 234,   37,    5,   95, 120,    91,   52,   59   },
    { 38,   96,  193,  185,   15, 223,   26,  59,  169,   85,   145,  150,  100,   89,   36,  44,    1, 38,    96,  193,   185,   15, 223,
     26,   59,  169,   85, 145,  150,  100,    89,   36,  44,     1, 38,  96, 193,  185,   15,  223,    26,   59,   169,   85, 145      },
    { 76,  157,   70,  95, 253,  217,  129,  133,  168,  230,   227,  130,   81,   18,   44,    2, 152,   39,  140,  190,   231,  175,  31,
     23,   77, 209,  219,   25, 162,   36,   88,   4, 45,   78,   5, 97,  211,   67,   62,   46,  154,  191,   171,   50,  89           },
    {152,   78,  10, 153,  214,   68,  147,   79, 146,  215,   220,  221,   69,   11,    1, 152,   78,  10,  153,  214,    68,  147,  79,
     146,  215,  220,  221,   69,  11,    1, 152,   78,  10,  153,  214,   68,  147,   79,  146,  215,   220,  221,    69,  11,    1    },
    { 45,  37,  80, 101,  223,  208,  102,  168,  191,  150,     7, 195,  251,  173,    38,  39,  10,   47,   127,   26,   197,   21, 115,
     219,  100,  242,  245,   54, 205,   96,   70,  97, 107,    68,   59,  33, 228,  241,  130,   89,    61,  207,    58,   12, 193     },
    { 90,  148,  186,   30,  226,   62, 109,   73, 179,  174,   162,   61, 131,  232,    96, 140,  153,  127,    52,   51,   168,   99,  98,
     56,  172,   22,   8, 234,  212,  185,   240,   67, 237,    79, 114,  241,   25, 121,  245,  108,    19,  39,    20,  188,  223     },
    {180,  106,  185,  253,   17,   59,  132,  230,  150,   28,   172,   44,  32,    3, 193,  190,  214,   26,   51,   77,  145,   55, 167,
     36, 233,  116,   96,   5,  94,  223,   103,   46,  85,   215,  174,   89,  244,  108,   38, 156,   160,   15,   226,  124,  169    },
    {117,  181,  161,  107,   26, 102,   41,  252,   87,  89,  245,  173,   45,  53,  185,  231,   68,  197,   168,  145,   110,  166,  61,
     54,  38,  37, 186,  120,  134,   59,    21, 191,  196,   221,   36, 207,  205,   39,  80,  15,   217,  237,    33, 115,  150       },
    {234,  238,   97, 254,  103,  184,   57,  227,    7, 172,   176,   58,  192,   40,   15, 175,  147,   21,   99,  55,   166,  122, 216,
     45, 106,  222,  107,   52, 133,   85,   123,   50,  195,    11,  32,   12, 140,  188,  182,  124,   158,  115,    49,  224,   36   },
    {201,  159,   47,  91, 124,   33, 209,  149,  166,  244,    71, 117,  238,  194,   223,   31,  79, 115,    98,  167,    61, 216,  90,
     181,  190,  254,  206,  218,  213,  150,   224,   72,  54,  152,  106,  161,  177,  189,  184,  114,   171,   56,    18, 131,   38 },
    {143,   70, 101,  217,   59, 168,  252,  130,  195,   44,    58,   39,  186,  231,    26,   23, 146,  219,    56,  36,   54,   45, 181,
     97, 223,   62,  33, 191,  110,   89,  251,    8, 12,    10,   15,  134,  197,   41,  179,  100,    86, 125,   205,   37,  185      },
    {  3,  5, 15,  17,  51,  85, 255,   28,  36, 108,   180,  193,   94, 226,    59,   77,  215,  100,   172,  233,    38, 106, 190,
     223,  124,  132,  145,  174,  239,   44,  116,  156,  185,   214,  103,  169,  230,   55,   89,  235,    32,  96,  160,  253,   26 },
    {  6,  20, 120,   13,  46,  228,   98,   81, 251,   32,  192,  186,  187,  189,   169,  209,  220,  242,    22, 116,    37, 222, 254,
     62,  132,   63, 130,   43,  250,   38,   212,  194,  182,   147,   77, 179,  141,    9, 54, 180,   159,  101,    67, 151,   85     },
    { 12,   80, 231,  208,  169,  191,   87, 195,  125,   38,  181,   47, 217,  197,    85,  219,  221,  245,     8,  96,   186,  107, 206,
     33,  145,  130,   86, 207,   45, 193,   101,  134,  102,   146,  150,  166,  251,   64,   39, 185,   127,   62,    21,  252,  100  },
    { 24,  93, 107,  129,  132,  252,  200,   18, 173,    3,  40, 231,  189,  158,   145,   25,  69,   54,  234,    5, 120,   52, 218,
     191,  174,   43, 207,   90,  35,  15,  136,   92, 115,   220,  239,  125,   76, 238,  101,   17,   133,  228,   149,  121,   44    },
    { 48, 105,  127,  248,   77,  241,  224,  247,   64,  156,    95,  182,  236,  170,   150,  162,   11, 205,   212,   94,  134,  133, 213,
     110,  239,  250,   45,  35,   30,  26,  218,   99,  130,    69,  108,  143,   40, 211,  206,  132,   229,    7, 144,    2, 96      },
    { 96,  185,  223,   59,  85,  150,   89,  44,  38, 193,    15,   26,  169,  145,   100,   36,    1, 96,   185,  223,    59,  85, 150,
     89,   44,  38, 193,   15,   26, 169,   145,  100,   36,    1,  96,  185,  223,   59,  85,  150,    89,   44,   38, 193,   15       }
};

/**
 * Powers of the root alpha of 1 + x^2 + x^3 + x^4 + x^8.
 * The last two elements are needed by the FsmSw_Hqc128_gf_mul function
 * (for example if both elements to multiply are zero).
 */
static const uint16 hqc128_gf_exp[258] = {
    1,   2,   4,   8,   16,  32,  64,  128, 29,  58,  116, 232, 205, 135, 19,  38,  76,  152, 45,  90,  180, 117,
    234, 201, 143, 3,   6,   12,  24,  48,  96,  192, 157, 39,  78,  156, 37,  74,  148, 53,  106, 212, 181, 119,
    238, 193, 159, 35,  70,  140, 5,   10,  20,  40,  80,  160, 93,  186, 105, 210, 185, 111, 222, 161, 95,  190,
    97,  194, 153, 47,  94,  188, 101, 202, 137, 15,  30,  60,  120, 240, 253, 231, 211, 187, 107, 214, 177, 127,
    254, 225, 223, 163, 91,  182, 113, 226, 217, 175, 67,  134, 17,  34,  68,  136, 13,  26,  52,  104, 208, 189,
    103, 206, 129, 31,  62,  124, 248, 237, 199, 147, 59,  118, 236, 197, 151, 51,  102, 204, 133, 23,  46,  92,
    184, 109, 218, 169, 79,  158, 33,  66,  132, 21,  42,  84,  168, 77,  154, 41,  82,  164, 85,  170, 73,  146,
    57,  114, 228, 213, 183, 115, 230, 209, 191, 99,  198, 145, 63,  126, 252, 229, 215, 179, 123, 246, 241, 255,
    227, 219, 171, 75,  150, 49,  98,  196, 149, 55,  110, 220, 165, 87,  174, 65,  130, 25,  50,  100, 200, 141,
    7,   14,  28,  56,  112, 224, 221, 167, 83,  166, 81,  162, 89,  178, 121, 242, 249, 239, 195, 155, 43,  86,
    172, 69,  138, 9,   18,  36,  72,  144, 61,  122, 244, 245, 247, 243, 251, 235, 203, 139, 11,  22,  44,  88,
    176, 125, 250, 233, 207, 131, 27,  54,  108, 216, 173, 71,  142, 1,   2,   4};
/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
 * \brief Computes 2 * HQC128_PARAM_DELTA syndromes
 *
 * \param[out] syndromes Array of size 2 * HQC128_PARAM_DELTA receiving the computed syndromes
 * \param[in] cdw Array of size PARAM_N1 storing the received vector
 */
static void compute_syndromes_128(uint16 *const syndromes, const uint8 *const cdw)
{
  for (uint8 i = 0; i < (2 * HQC128_PARAM_DELTA); ++i)
  {
    for (uint8 j = 1; j < HQC128_PARAM_N1; ++j)
    {
      syndromes[i] ^= FsmSw_Hqc128_Gf_Mul(cdw[j], alpha_ij_pow_128[i][j - 1]);
    }
    syndromes[i] ^= cdw[0];
  }
} // end: compute_syndromes

/*====================================================================================================================*/
/**
 * \brief Computes the error locator polynomial (ELP) sigma
 *
 * This is a constant time implementation of Berlekamp's simplified algorithm (see \cite lin1983error (Chapter 6 - BCH Codes). <br>
 * We use the letter p for rho which is initialized at -1. <br>
 * The array X_sigma_p represents the polynomial X^(mu-rho)*sigma_p(X). <br>
 * Instead of maintaining a list of sigmas, we update in place both sigma and X_sigma_p. <br>
 * sigma_copy serves as a temporary save of sigma in case X_sigma_p needs to be updated. <br>
 * We can properly correct only if the degree of sigma does not exceed HQC128_PARAM_DELTA.
 * This means only the first HQC128_PARAM_DELTA + 1 coefficients of sigma are of value
 * and we only need to save its first HQC128_PARAM_DELTA - 1 coefficients.
 *
 * \returns the degree of the ELP sigma
 * \param[out] sigma Array of size (at least) HQC128_PARAM_DELTA receiving the ELP
 * \param[in] syndromes Array of size (at least) 2*HQC128_PARAM_DELTA storing the syndromes
 */
static uint16 hqc128_compute_elp(uint16 *const sigma, const uint16 *const syndromes)
{
  uint16 deg_sigma                              = 0;
  uint16 deg_sigma_p                            = 0;
  uint16 deg_sigma_copy                         = 0;
  uint16 sigma_copy[HQC128_PARAM_DELTA + 1]     = {0};
  uint16 X_sigma_p[HQC128_PARAM_DELTA + 1]      = {0};
  X_sigma_p[HQC128_X_SIGMA_P_2ND_ELEMENT_INDEX] = 1;
  uint16 pp                                     = 0xFFFFU; // 2*rho
  uint16 d_p                                    = 1;
  uint16 d                                      = syndromes[0];

  uint16 mask1, mask2, mask12;
  uint16 deg_X, deg_X_sigma_p;
  uint16 dd;
  uint16 mu;

  uint16 i;

  sigma[0] = 1;
  for (mu = 0; (mu < (2 * HQC128_PARAM_DELTA)); ++mu)
  {
    // Save sigma in case we need it to update X_sigma_p
    FsmSw_CommonLib_MemCpy(sigma_copy, sigma, 2 * (HQC128_PARAM_DELTA));
    deg_sigma_copy = deg_sigma;

    dd = FsmSw_Hqc128_Gf_Mul(d, FsmSw_Hqc128_Gf_Inverse(d_p));

    for (i = 1; (i <= (mu + 1)) && (i <= HQC128_PARAM_DELTA); ++i)
    {
      sigma[i] ^= FsmSw_Hqc128_Gf_Mul(dd, X_sigma_p[i]);
    }

    // "deg_X" equals "mu" minus "pp"
    deg_X         = mu + 1U;
    deg_X_sigma_p = deg_X + deg_sigma_p;

    // mask1 = 0xffff if(d != 0) and 0 otherwise
    mask1 = (d != 0) ? 0xFFFFU : 0x0000U;

    // mask2 = 0xffff if(deg_X_sigma_p > deg_sigma) and 0 otherwise
    mask2 = (deg_X_sigma_p > deg_sigma) ? 0xFFFFU : 0x0000U;

    // mask12 = 0xffff if the deg_sigma increased and 0 otherwise
    mask12 = mask1 & mask2;
    deg_sigma ^= mask12 & (deg_X_sigma_p ^ deg_sigma);

    if (mu == ((2 * HQC128_PARAM_DELTA) - 1))
    {
      break;
    }

    pp ^= mask12 & (mu ^ pp);
    d_p ^= mask12 & (d ^ d_p);
    for (i = HQC128_PARAM_DELTA; i > 0; --i)
    {
      X_sigma_p[i] = (mask12 & sigma_copy[i - 1]) ^ (~mask12 & X_sigma_p[i - 1]);
    }

    deg_sigma_p ^= mask12 & (deg_sigma_copy ^ deg_sigma_p);
    d = syndromes[mu + 1];

    for (i = 1; (i <= (mu + 1)) && (i <= HQC128_PARAM_DELTA); ++i)
    {
      d ^= FsmSw_Hqc128_Gf_Mul(sigma[i], syndromes[mu + 1 - i]);
    }
  }

  return deg_sigma;
} // end: compute_elp

/*====================================================================================================================*/
/**
 * \brief Computes the error polynomial error from the error locator polynomial sigma
 *
 * See function FsmSw_Hqc128_fft for more details.
 *
 * \param[out] err Array of 2^HQC128_PARAM_M elements receiving the error polynomial
 * \param[out] error_compact Array of HQC128_PARAM_DELTA + PARAM_N1 elements receiving a compact representation of the vector error
 * \param[in] sigma Array of 2^HQC128_PARAM_FFT elements storing the error locator polynomial
 */
static void compute_roots_128(uint8 *const err, const uint16 *const sigma)
{
  uint16 w[(uint32)1U << HQC128_PARAM_M] = {0};

  FsmSw_Hqc128_Fft(w, sigma, HQC128_PARAM_DELTA + 1);
  FsmSw_Hqc128_Fft_Retrieve_Error_Poly(err, w);
} // end: compute_roots

/*====================================================================================================================*/
/**
 * \brief Computes the polynomial z(x)
 *
 * See \cite lin1983error (Chapter 6 - BCH Codes) for more details.
 *
 * \param[out] z Array of HQC128_PARAM_DELTA + 1 elements receiving the polynomial z(x)
 * \param[in] sigma Array of 2^HQC128_PARAM_FFT elements storing the error locator polynomial
 * \param[in] degree Integer that is the degree of polynomial sigma
 * \param[in] syndromes Array of 2 * HQC128_PARAM_DELTA storing the syndromes
 */
static void hqc128_compute_z_poly(uint16 *const z, const uint16 *const sigma, uint16 degree,
                                  const uint16 *const syndromes)
{
  uint8 i, j;
  uint16 mask;

  z[0] = 1;

  for (i = 1; i < (HQC128_PARAM_DELTA + 1); ++i)
  {
    mask = (((i - degree - 1) >> 15) == 1) ? 0xFFFFU : 0x0000U;
    z[i] = mask & sigma[i];
  }

  z[1] ^= syndromes[0];

  for (i = 2; i <= HQC128_PARAM_DELTA; ++i)
  {
    mask = (((i - degree - 1) >> 15) == 1) ? 0xFFFFU : 0x0000U;
    z[i] ^= mask & syndromes[i - 1];

    for (j = 1; j < i; ++j)
    {
      z[i] ^= mask & FsmSw_Hqc128_Gf_Mul(sigma[j], syndromes[i - j - 1]);
    }
  }
} // end: compute_z_poly

/*====================================================================================================================*/
/**
 * \brief Computes the error values
 *
 * See \cite lin1983error (Chapter 6 - BCH Codes) for more details.
 *
 * \param[out] error_values Array of HQC128_PARAM_DELTA elements receiving the error values
 * \param[in] z Array of HQC128_PARAM_DELTA + 1 elements storing the polynomial z(x)
 * \param[in] z_degree Integer that is the degree of polynomial z(x)
 * \param[in] error_compact Array of HQC128_PARAM_DELTA + PARAM_N1 storing compact representation of the error
 */
static void compute_error_values_128(uint16 *const error_values, const uint16 *const z, const uint8 *const err)
{
  uint16 beta_j[HQC128_PARAM_DELTA] = {0};
  uint16 e_j[HQC128_PARAM_DELTA]    = {0};

  uint16 delta_counter;
  uint16 delta_real_value;
  uint16 found;
  uint16 mask1;
  uint16 mask2;
  uint16 tmp1;
  uint16 tmp2;
  uint16 inverse;
  uint16 inverse_power_j;

  // Compute the "beta_{j_i}" page 31 of the documentation
  delta_counter = 0;
  for (uint8 i = 0; i < HQC128_PARAM_N1; i++)
  {
    found = 0;
    mask1 = ((err[i] >> 7) == 1) ? 0xFFFFU : 0x0000U; // err[i] != 0
    for (uint8 j = 0; j < HQC128_PARAM_DELTA; j++)
    {
      mask2 = (((j ^ FsmSw_Convert_u16_to_u32(delta_counter)) >> 31) == 1) ? 0x0000U : 0xFFFFU;
      beta_j[j] += mask1 & mask2 & hqc128_gf_exp[i];
      found += mask1 & mask2 & 1U;
    }
    delta_counter += found;
  }
  delta_real_value = delta_counter;

  // Compute the "e_{j_i}" page 31 of the documentation
  for (uint8 i = 0; i < HQC128_PARAM_DELTA; ++i)
  {
    tmp1            = 1;
    tmp2            = 1;
    inverse         = FsmSw_Hqc128_Gf_Inverse(beta_j[i]);
    inverse_power_j = 1;

    for (uint8 j = 1; j <= HQC128_PARAM_DELTA; ++j)
    {
      inverse_power_j = FsmSw_Hqc128_Gf_Mul(inverse_power_j, inverse);
      tmp1 ^= FsmSw_Hqc128_Gf_Mul(inverse_power_j, z[j]);
    }
    for (uint8 k = 1; k < HQC128_PARAM_DELTA; ++k)
    {
      tmp2 = FsmSw_Hqc128_Gf_Mul(tmp2, (1U ^ FsmSw_Hqc128_Gf_Mul(inverse, beta_j[(i + k) % HQC128_PARAM_DELTA])));
    }
    mask1  = (i < delta_real_value) ? 0xFFFFU : 0x0000U;
    e_j[i] = mask1 & FsmSw_Hqc128_Gf_Mul(tmp1, FsmSw_Hqc128_Gf_Inverse(tmp2));
  }

  // Place the delta e_{j_i} values at the right coordinates of the output vector
  delta_counter = 0;
  for (uint8 i = 0; i < HQC128_PARAM_N1; ++i)
  {
    found = 0;
    mask1 = ((err[i] >> 7) == 1) ? 0xFFFFU : 0x0000U; // err[i] != 0
    for (uint8 j = 0; j < HQC128_PARAM_DELTA; j++)
    {
      mask2 = (((j ^ FsmSw_Convert_u16_to_u32(delta_counter)) >> 31) == 1) ? 0x0000U : 0xFFFFU;
      error_values[i] += mask1 & mask2 & e_j[j];
      found += mask1 & mask2 & 1U;
    }
    delta_counter += found;
  }
} // end: compute_error_values

/*====================================================================================================================*/
/**
 * \brief Correct the errors
 *
 * \param[out] cdw Array of PARAM_N1 elements receiving the corrected vector
 * \param[in] err Array of the error vector
 * \param[in] error_values Array of HQC128_PARAM_DELTA elements storing the error values
 */
static void hqc128_correct_errors(uint8 *const cdw, const uint16 *const error_values)
{
  for (uint8 i = 0; i < HQC128_PARAM_N1; ++i)
  {
    cdw[i] ^= (uint8)(error_values[i] & 0xFFU);
  }
} // end: correct_errors

/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/

/*====================================================================================================================*/
/**
 * \brief Encodes a message message of HQC128_PARAM_K bits to a Reed-Solomon codeword codeword of PARAM_N1 bytes
 *
 * Following \cite lin1983error (Chapter 4 - Cyclic Codes),
 * We perform a systematic encoding using a linear (PARAM_N1 - HQC128_PARAM_K)-stage shift register
 * with feedback connections based on the generator polynomial PARAM_RS_POLY of the Reed-Solomon code.
 *
 * \param[out] cdw Array of size HQC128_VEC_N1_SIZE_64 receiving the encoded message
 * \param[in] msg Array of size HQC128_VEC_K_SIZE_64 storing the message
 */
void FsmSw_Hqc128_Reed_Solomon_Encode(uint8 *const cdw, const uint8 *const msg)
{
  uint8 gate_value = 0;

  uint16 tmp[HQC128_PARAM_G] = {0};
  uint16 PARAM_RS_POLY[]     = {HQC128_RS_POLY_COEFS};

  FsmSw_CommonLib_MemSet(cdw, 0, HQC128_PARAM_N1);

  for (uint8 i = 0; i < HQC128_PARAM_K; ++i)
  {
    gate_value = msg[HQC128_PARAM_K - 1 - i] ^ cdw[HQC128_PARAM_N1 - HQC128_PARAM_K - 1];

    for (uint8 j = 0; j < HQC128_PARAM_G; ++j)
    {
      tmp[j] = FsmSw_Hqc128_Gf_Mul(gate_value, PARAM_RS_POLY[j]);
    }

    for (sint8 k = HQC128_PARAM_N1 - HQC128_PARAM_K - 1; k > 0; --k)
    {
      cdw[k] = (uint8)(cdw[k - 1] ^ tmp[k]);
    }

    cdw[0] = (uint8)tmp[0];
  }

  FsmSw_CommonLib_MemCpy(&cdw[HQC128_PARAM_N1 - HQC128_PARAM_K], msg, HQC128_PARAM_K);
} //end: FsmSw_Hqc128_Reed_Solomon_Encode

/*====================================================================================================================*/
/**
 * \brief Decodes the received word
 *
 * This function relies on six steps:
 *    <ol>
 *    <li> The first step, is the computation of the 2*HQC128_PARAM_DELTA syndromes.
 *    <li> The second step is the computation of the error-locator polynomial sigma.
 *    <li> The third step, done by additive FFT, is finding the error-locator numbers by calculating the roots of the polynomial sigma and takings their inverses.
 *    <li> The fourth step, is the polynomial z(x).
 *    <li> The fifth step, is the computation of the error values.
 *    <li> The sixth step is the correction of the errors in the received polynomial.
 *    </ol>
 * For a more complete picture on Reed-Solomon decoding, see Shu. Lin and Daniel J. Costello in Error Control Coding: Fundamentals and Applications @cite lin1983error
 *
 * \param[out] msg Array of size HQC128_VEC_K_SIZE_64 receiving the decoded message
 * \param[in] cdw Array of size HQC128_VEC_N1_SIZE_64 storing the received word
 */
void FsmSw_Hqc128_Reed_Solomon_Decode(uint8 *const msg, uint8 *const cdw)
{
  uint16 syndromes[2 * HQC128_PARAM_DELTA] = {0};
  uint16 sigma[1U << HQC128_PARAM_FFT]     = {0};
  uint8 err[(uint32)1U << HQC128_PARAM_M]  = {0};
  uint16 z[HQC128_PARAM_N1]                = {0};
  uint16 error_values[HQC128_PARAM_N1]     = {0};
  uint16 deg;

  // Calculate the 2*HQC128_PARAM_DELTA syndromes
  compute_syndromes_128(syndromes, cdw);

  // Compute the error locator polynomial sigma
  // Sigma's degree is at most HQC128_PARAM_DELTA but the FFT requires the extra room
  deg = hqc128_compute_elp(sigma, syndromes);

  // Compute the error polynomial error
  compute_roots_128(err, sigma);

  // Compute the polynomial z(x)
  hqc128_compute_z_poly(z, sigma, deg, syndromes);

  // Compute the error values
  compute_error_values_128(error_values, z, err);

  // Correct the errors
  hqc128_correct_errors(cdw, error_values);

  // Retrieve the message from the decoded codeword
  FsmSw_CommonLib_MemCpy(msg, &cdw[HQC128_PARAM_G - 1], HQC128_PARAM_K);
} // end: FsmSw_Hqc128_Reed_Solomon_Decode

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
