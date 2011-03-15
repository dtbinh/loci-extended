#ifndef LOCI_NUMERIC_DETAIL_SIMPLE_MATHS_SEQUENCE_SEQUENCES_H_
#define LOCI_NUMERIC_DETAIL_SIMPLE_MATHS_SEQUENCE_SEQUENCES_H_

/**
 * Simple vector-style mathematical sequences
 * Extends the base_sequence functionality, adding operators and functions to be
 * used mathematical sequences that have vector-style mathematical operations,
 * this include vectors and eulers but excludes quaternions.
 *
 * @file   sequences.h
 * @author David Gill
 * @date   29/12/2009
 */

#include <algorithm>
#include <functional>
#include <cstddef>

#include "loci/numeric/detail/base_sequence.h"

namespace loci {
namespace numeric {
namespace detail
{
    // extends base_sequence, refactoring away common operators and functions 
    template <class dT, typename T, std::size_t N>
    class base_simple_maths_sequence : public base_sequence<dT, T, N>
    {
    public:
        derived_type & operator*=(const T & rhs) { return apply_scalar(rhs, std::multiplies<T>()); }
        derived_type & operator/=(const T & rhs) { return apply_scalar(rhs, std::divides<T>());    }

        //derived_type & operator-() { return apply(*this, std::negate<T>()); }

    protected:
        template <class U, typename UnaryFunction>
        derived_type & apply(const U & rhs, UnaryFunction op)
        {
            std::transform(begin(), end(), rhs.begin(), begin(), op);
            return derived_this_ref();
        }

    private:
        template <typename UnaryFunction>
        derived_type & apply_scalar(const T & scalar, UnaryFunction op)
        {
            std::transform(begin(), end(), begin(), std::bind2nd(op, scalar));
            return derived_this_ref();
        }
    };

    // to be used by clients that support multiple sizes, specified as the
    // second template parameter.
    template <template <typename, std::size_t> class dT, typename T, std::size_t N>
    class anysize_simple_maths_sequence : public base_simple_maths_sequence<dT<T, N>, T, N>
    {
    public:
        typedef anysize_simple_maths_sequence maths_sequence_type;

        template <typename U>
        derived_type & operator+=(const dT<U, N> & rhs) { return apply(rhs, std::plus<T>()); }

        template <typename U>
        derived_type & operator-=(const dT<U, N> & rhs) { return apply(rhs, std::minus<T>()); }
    };

    // to be used by clients that support a fixed size container.
    template <template <typename> class dT, typename T, std::size_t N>
    class onesize_simple_maths_sequence : public base_simple_maths_sequence<dT<T>, T, N>
    {
    public:
        typedef onesize_simple_maths_sequence maths_sequence_type;

        template <typename U>
        derived_type & operator+=(const dT<U> & rhs) { return apply(rhs, std::plus<T>()); }

        template <typename U>
        derived_type & operator-=(const dT<U> & rhs) { return apply(rhs, std::minus<T>()); }
    };
} // namespace detail
} // namespace numeric
} // namespace loci

#endif // LOCI_NUMERIC_DETAIL_SIMPLE_MATHS_SEQUENCE_SEQUENCES_H_
