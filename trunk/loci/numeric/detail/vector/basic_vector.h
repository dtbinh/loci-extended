#ifndef LOCI_NUMERIC_DETAIL_VECTOR_BASIC_VECTOR_H_
#define LOCI_NUMERIC_DETAIL_VECTOR_BASIC_VECTOR_H_

/**
 * Spatial vector classes.
 * Defines a templates base class for 2/3/4 vectors to specialise from.
 *
 * @file   basic_vector.h
 * @author David Gill
 * @date   07/06/2009
 */

#include <numeric>

#include "loci/numeric/maths.h"
#include "loci/numeric/detail/simple_maths_sequence.h"
#include "loci/numeric/detail/xyzw_manipulators.h"

namespace loci {
namespace numeric
{
    // forward declaration
    template <typename T, std::size_t N> class basic_vector;

    namespace detail
    {
        template <typename T, std::size_t N>
        class base_vector : public anysize_simple_maths_sequence<basic_vector, T, N>
        {
        public:
            T length_squared() const
            {
                return std::inner_product(begin(), end(), begin(), T());
            }

            T length() const
            {
                return sqrt(length_squared());
            }

            derived_type & normalise()
            {
                return derived_this_ref() /= length();
            }
        };
    } // namespace detail


    // unspecialised for N <= 2 and N >= 4
    template <typename T, std::size_t N>
    class basic_vector : public detail::base_vector<T, N>
    {
    public:
        basic_vector() { }
        explicit basic_vector(const T & value) { assign(value); }
    };

    // specialised for N = 2
    template <typename T>
    class basic_vector<T, 2> : public detail::with_xy<detail::base_vector<T, 2> >
    {
    public:
        basic_vector() { }
        explicit basic_vector(const T & value) { assign(value); }
        basic_vector(const T & x, const T & y)
        {
            elems[0] = x;
            elems[1] = y;
        }
    };

    // specialised for N = 3
    template <typename T>
    class basic_vector<T, 3> : public detail::with_xyz<detail::base_vector<T, 3> >
    {
    public:
        basic_vector() { }
        explicit basic_vector(const T & value) { assign(value); }
        basic_vector(const T & x, const T & y, const T & z)
        {
            elems[0] = x;
            elems[1] = y;
            elems[2] = z;
        }
    };

    // specialised for N = 4
    template <typename T>
    class basic_vector<T, 4> : public detail::with_xyzw<detail::base_vector<T, 4> >
    {
    public:
        basic_vector() { }
        explicit basic_vector(const T & value) { assign(value); }
        basic_vector(const T & x, const T & y, const T & z, const T & w)
        {
            elems[0] = x;
            elems[1] = y;
            elems[2] = z;
            elems[3] = w;
        }
    };
} // namespace numeric
} // namespace loci

#endif // LOCI_NUMERIC_DETAIL_VECTOR_BASIC_VECTOR_H_
