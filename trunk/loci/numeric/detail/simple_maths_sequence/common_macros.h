#ifndef LOCI_NUMERIC_DETAIL_SIMPLE_MATHS_SEQUENCE_COMMON_MACROS_H_
#define LOCI_NUMERIC_DETAIL_SIMPLE_MATHS_SEQUENCE_COMMON_MACROS_H_

/**
 * Macros that are common to non_member_ops and interpolation.
 * These macros refactor away generation of the template heading for functions
 * and the generation of templated type names, along with the switch between
 * anysize and onesize versions.
 *
 * @file   common_macros.h
 * @author David Gill
 * @date   29/12/2009
 */

#include <cstddef>

namespace loci {
namespace numeric {
namespace detail
{
    // for selecting the correct template heading depending on whether we want anysize or onesize
    #define LOCI_SIMPLE_MATHS_SEQUENCE_ANYSIZE_TEMPLATE_PART template <typename T1, typename T2, std::size_t N>
    #define LOCI_SIMPLE_MATHS_SEQUENCE_ONESIZE_TEMPLATE_PART template <typename T1, typename T2>
    #define LOCI_SIMPLE_MATHS_SEQUENCE_TEMPLATE_PART(SIZING) LOCI_SIMPLE_MATHS_SEQUENCE_##SIZING##_TEMPLATE_PART

    // for concatenating types with template parameters, depending on whether we want anysize or onesize
    #define LOCI_SIMPLE_MATHS_SEQUENCE_ANYSIZE_TEMPLATED_TYPE(TYPE, T) TYPE<T, N>
    #define LOCI_SIMPLE_MATHS_SEQUENCE_ONESIZE_TEMPLATED_TYPE(TYPE, T) TYPE<T>
    #define LOCI_SIMPLE_MATHS_SEQUENCE_TEMPLATED_TYPE(TYPE, T, SIZING) LOCI_SIMPLE_MATHS_SEQUENCE_##SIZING##_TEMPLATED_TYPE(TYPE, T)
} // namespace detail
} // namespace numeric
} // namespace loci

#endif // LOCI_NUMERIC_DETAIL_SIMPLE_MATHS_SEQUENCE_COMMON_MACROS_H_