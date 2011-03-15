#ifndef LOCI_NUMERIC_DETAIL_SIMPLE_MATHS_SEQUENCE_H_
#define LOCI_NUMERIC_DETAIL_SIMPLE_MATHS_SEQUENCE_H_

/**
 * Common refactoring of vector-style maths sequences
 * collects the implementation files together and provides macros for creating
 * all non-member functions associated with simple maths sequences.
 *
 * @file   simple_maths_sequence.h
 * @author David Gill
 * @date   29/12/2009
 */

#include "loci/numeric/detail/simple_maths_sequence/sequences.h"
#include "loci/numeric/detail/simple_maths_sequence/non_member_ops.h"
#include "loci/numeric/detail/simple_maths_sequence/interpolation.h"

// conveniently provide a macros to include all non-member functions.
// lerp overloads use non-member operators and so must appear afterwards,
// this is the primary motivation for provoding the below macros.

// for anysize
#define LOCI_ANYSIZE_SIMPLE_MATHS_SEQUENCE_ALL_NON_MEMBERS(TYPE) \
    LOCI_ANYSIZE_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_OPERATORS(TYPE) \
    LOCI_ANYSIZE_SIMPLE_MATHS_SEQUENCE_LERP_OVERLOADS(TYPE)

// for onesize
#define LOCI_ONESIZE_SIMPLE_MATHS_SEQUENCE_ALL_NON_MEMBERS(TYPE) \
    LOCI_ONESIZE_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_OPERATORS(TYPE) \
    LOCI_ONESIZE_SIMPLE_MATHS_SEQUENCE_LERP_OVERLOADS(TYPE)

#endif // LOCI_NUMERIC_DETAIL_SIMPLE_MATHS_SEQUENCE_H_
