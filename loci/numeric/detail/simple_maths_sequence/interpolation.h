#ifndef LOCI_NUMERIC_DETAIL_SIMPLE_MATHS_SEQUENCE_INTERPOLATION_H_
#define LOCI_NUMERIC_DETAIL_SIMPLE_MATHS_SEQUENCE_INTERPOLATION_H_

/**
 * Component-wise interpolation routines
 * Defines interpolation functions that occurs per-component in the sequence
 *
 * @file   interpolation.h
 * @author David Gill
 * @date   07/06/2009
 */

#include "loci/numeric/detail/simple_maths_sequence/common_macros.h"
#include "loci/numeric/parametric.h"

namespace loci {
namespace numeric
{
    // the structure the lerp overloads
    #define LOCI_SIMPLE_MATHS_SEQUENCE_LERP_OVERLOADS(TYPE, SIZING) \
        LOCI_SIMPLE_MATHS_SEQUENCE_TEMPLATE_PART(SIZING) \
        TYPE lerp(const TYPE & a, const TYPE & b, T2 t) { \
            return linear(a, b, t); \
        } \
        \
        LOCI_SIMPLE_MATHS_SEQUENCE_TEMPLATE_PART(SIZING) \
        TYPE lerp(const TYPE & a, const TYPE & b, T2 t, T2 range) { \
            return lerp(a, b, t / range); \
        }
        
    // creates lerp overloads for anysize_simple_maths_sequences
    #define LOCI_ANYSIZE_SIMPLE_MATHS_SEQUENCE_LERP_OVERLOADS(TYPE) \
        LOCI_SIMPLE_MATHS_SEQUENCE_LERP_OVERLOADS( \
            LOCI_SIMPLE_MATHS_SEQUENCE_TEMPLATED_TYPE(TYPE, T1, ANYSIZE), ANYSIZE)

    // creates lerp overloads for onesize_simple_maths_sequences
    #define LOCI_ONESIZE_SIMPLE_MATHS_SEQUENCE_LERP_OVERLOADS(TYPE) \
        LOCI_SIMPLE_MATHS_SEQUENCE_LERP_OVERLOADS( \
            LOCI_SIMPLE_MATHS_SEQUENCE_TEMPLATED_TYPE(TYPE, T1, ONESIZE), ONESIZE)
} // namespace numeric
} // namespace loci

#endif // LOCI_NUMERIC_DETAIL_SIMPLE_MATHS_SEQUENCE_INTERPOLATION_H_
