#ifndef LOCI_NUMERIC_DETAIL_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_OPS_H_
#define LOCI_NUMERIC_DETAIL_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_OPS_H_

/**
 * Macros to make non-member operators convenient for clients to include.
 * These macros refactor away operator generation at every stage, hence there is
 * a lot of them. Only the bottom two are intended to be used by clients.
 *
 * @file   non_member_ops.h
 * @author David Gill
 * @date   20/12/2009
 */

#include "loci/numeric/detail/simple_maths_sequence/common_macros.h"

namespace loci {
namespace numeric {
namespace detail
{
    // the structure of an operator
    #define LOCI_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_OPERATOR(LHS, RHS, RET, RETL, RETR, OP, SIZING) \
        LOCI_SIMPLE_MATHS_SEQUENCE_TEMPLATE_PART(SIZING) \
        RET operator##OP(const LHS & lhs, const RHS & rhs) { return RET(RETL) OP= RETR; }

    // more specific operator with a return type the same as the lhs argument
    #define LOCI_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_LHS_OPERATOR(LHS, RHS, OP, SIZING) \
        LOCI_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_OPERATOR(LHS, RHS, LHS, lhs, rhs, OP, SIZING)

    // more specific operator with a return type the same as the rhs argument
    #define LOCI_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_RHS_OPERATOR(LHS, RHS, OP, SIZING) \
        LOCI_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_OPERATOR(LHS, RHS, RHS, rhs, lhs, OP, SIZING)

    // an operator that acts on two sequences
    #define LOCI_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_SEQ_SEQ_OPERATOR(TYPE, SIZING, OP) \
        LOCI_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_LHS_OPERATOR( \
            LOCI_SIMPLE_MATHS_SEQUENCE_TEMPLATED_TYPE(TYPE, T1, SIZING), \
            LOCI_SIMPLE_MATHS_SEQUENCE_TEMPLATED_TYPE(TYPE, T2, SIZING), \
            OP, SIZING)

    // an operator that acts on a lhs sequence and a rhs scalar
    #define LOCI_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_SEQ_SCALAR_OPERATOR(TYPE, SIZING, OP) \
        LOCI_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_LHS_OPERATOR( \
            LOCI_SIMPLE_MATHS_SEQUENCE_TEMPLATED_TYPE(TYPE, T1, SIZING), T2, OP, SIZING)

    // an operator that acts on a lhs scalar and a rhs sequence
    #define LOCI_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_SCALAR_SEQ_OPERATOR(TYPE, SIZING, OP) \
        LOCI_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_RHS_OPERATOR( \
            T1, LOCI_SIMPLE_MATHS_SEQUENCE_TEMPLATED_TYPE(TYPE, T2, SIZING), OP, SIZING)

    // conveniently creates all common operators (N.B. macros don't like comments past the backslashes)
    #define LOCI_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_OPERATORS(TYPE, SIZING) \
        LOCI_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_SEQ_SEQ_OPERATOR(TYPE, SIZING, +) \
        LOCI_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_SEQ_SEQ_OPERATOR(TYPE, SIZING, -) \
        LOCI_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_SEQ_SCALAR_OPERATOR(TYPE, SIZING, *) \
        LOCI_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_SCALAR_SEQ_OPERATOR(TYPE, SIZING, *) \
        LOCI_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_SEQ_SCALAR_OPERATOR(TYPE, SIZING, /)

    // conveniently creates all common operators for anysize_simple_maths_sequences
    #define LOCI_ANYSIZE_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_OPERATORS(TYPE) \
        LOCI_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_OPERATORS(TYPE, ANYSIZE)

    // conveniently creates all common operators for onesize_simple_maths_sequences
    #define LOCI_ONESIZE_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_OPERATORS(TYPE) \
        LOCI_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_OPERATORS(TYPE, ONESIZE)
} // namespace detail
} // namespace numeric
} // namespace loci

#endif // LOCI_NUMERIC_DETAIL_SIMPLE_MATHS_SEQUENCE_NON_MEMBER_OPS_H_
