#ifndef FSMSW_STATIC_ASSERT_H
#define FSMSW_STATIC_ASSERT_H
/**********************************************************************************************************************
 *
 *                                                    IAV GmbH
 * \file
 * \brief Compile time assertions
 * \details Provides the macro FSMSW_STATIC_ASSERT which can be used to perform compile time assertions.
 * If the assertion fails, a compilation error will occur.
 * Inspired by www.pixelbeat.org/programming/gcc/static_assert.html
 * 
 **********************************************************************************************************************/

/** \addtogroup SwC FsmSw
*    includes the modules for SwC FsmSw
 ** @{ */
/** \addtogroup Libs
*    includes the modules for Libs
 ** @{ */
/** \addtogroup StaticAssert
 ** @{ */

/**********************************************************************************************************************
 * MACROS                                                                                                             
 **********************************************************************************************************************/
/**
 * \brief Compile time assertion macro
 * \param[in] e The expression to assert. If this expression evaluates to false, a compilation error
 * ("error: array size is negative") will occur.
 */
/* polyspace +3 MISRA2012:D4.9,2.2 [Justified:]
   polyspace +2 CERT-C:PRE00-C [Justified:]
   "Function not feasible to trigger compile time error, macro must be used" */
#define FSMSW_STATIC_ASSERT(e) extern char (*fsmsw_assert(void))[sizeof(char[1 - (2 * !(e))])]

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
#endif /* FSMSW_STATIC_ASSERT_H */
