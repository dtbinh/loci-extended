#ifndef LOCI_NUMERIC_DETAIL_VECTOR_NON_MEMBERS_H_
#define LOCI_NUMERIC_DETAIL_VECTOR_NON_MEMBERS_H_

/**
 * Spatial vector functionality.
 * Defines non member functions for basic_vectors.
 *
 * @file   non_members.h
 * @author David Gill
 * @date   07/06/2009
 */

#include <cstddef>
#include <numeric>

#include "loci/platform/tstream.h"

#include "loci/numeric/maths.h"
#include "loci/numeric/convert/angles.h"
#include "loci/numeric/detail/vector/basic_vector.h"
#include "loci/numeric/detail/simple_maths_sequence.h"

namespace loci {
namespace numeric
{
    LOCI_ANYSIZE_SIMPLE_MATHS_SEQUENCE_ALL_NON_MEMBERS(basic_vector)

    template <typename T, std::size_t N>
    inline T length_squared(const basic_vector<T, N> & v)
    {
        return v.length_squared();
    }

    template <typename T, std::size_t N>
    inline T length(const basic_vector<T, N> & v)
    {
        return v.length();
    }

    template <typename T, std::size_t N>
    inline bool approx_zero(const basic_vector<T, N> & v)
    {
        return approx_equal(v.length_squared(), T(0));
    }

    template <typename T, std::size_t N>
    inline bool approx_normal(const basic_vector<T, N> & v)
    {
        return approx_equal(v.length_squared(), T(1));
    }

    template <typename T, std::size_t N>
    inline basic_vector<T, N> normalise(const basic_vector<T, N> & v)
    {
        return basic_vector<T, N>(v).normalise();
    }

    template <typename T, std::size_t N>
    inline basic_vector<T, N> & try_normalise_inplace(basic_vector<T, N> & v,  const basic_vector<T, N> & on_zero)
    {
        return approx_zero(v) ? (v = on_zero) : v.normalise();
    }

    template <typename T, std::size_t N>
    inline basic_vector<T, N> & try_normalise_inplace(basic_vector<T, N> & v)
    {
        return approx_zero(v) ? v : v.normalise();
    }

    template <typename T, std::size_t N>
    inline basic_vector<T, N> try_normalise(const basic_vector<T, N> & v, const basic_vector<T, N> & on_zero)
    {
        return approx_zero(v) ? on_zero : basic_vector<T, N>(v).normalise();
    }

    template <typename T, std::size_t N>
    inline basic_vector<T, N> try_normalise(const basic_vector<T, N> & v)
    {
        return try_normalise(v, v);
    }

    template <typename T, std::size_t N>
    inline T dot(const basic_vector<T, N> & lhs, const basic_vector<T, N> & rhs)
    {
        return std::inner_product(lhs.begin(), lhs.end(), rhs.begin(), T());
    }

    template <typename T>
    inline basic_vector<T, 3> cross(const basic_vector<T, 3> & lhs, const basic_vector<T, 3> & rhs)
    {
        return basic_vector<T, 3>(lhs.y() * rhs.z() - lhs.z() * rhs.y(),
                                  lhs.z() * rhs.x() - lhs.x() * rhs.z(),
                                  lhs.x() * rhs.y() - lhs.y() * rhs.x());
    }

    template <typename T, std::size_t N>
    T rads_between_normals(const basic_vector<T, N> & lhs, const basic_vector<T, N> & rhs)
    {
        return acos(dot(lhs, rhs));
    }

    template <typename T, std::size_t N>
    T degs_between_normals(const basic_vector<T, N> & lhs, const basic_vector<T, N> & rhs)
    {
        return convert::to_degs(rads_between_normals(lhs, rhs));
    }

    template <typename T, std::size_t N>
    T rads_between(const basic_vector<T, N> & lhs, const basic_vector<T, N> & rhs)
    {
        return acos( dot(lhs, rhs) / (lhs.length() * rhs.length()) );
    }

    template <typename T, std::size_t N>
    T degs_between(const basic_vector<T, N> & lhs, const basic_vector<T, N> & rhs)
    {
        return convert::to_degs(rads_between(lhs, rhs));
    }

    template <typename T>
    basic_vector<T, 2> xz_from_xyz(const basic_vector<T, 3> & v)
    {
        return basic_vector<T, 2>(v.x(), v.z());
    }

    template <typename T>
    basic_vector<T, 3> xyz_from_xz(const basic_vector<T, 2> & v, T y = T(0))
    {
        return basic_vector<T, 3>(v.x(), y, v.y());
    }
} // namespace numeric
} // namespace loci

#endif // LOCI_NUMERIC_DETAIL_VECTOR_NON_MEMBERS_H_
