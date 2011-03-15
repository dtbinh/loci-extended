#ifndef LOCI_NUMERIC_TRANSFORM_BY_QUAT_H_
#define LOCI_NUMERIC_TRANSFORM_BY_QUAT_H_

/**
 * Transformation routines for moving vectors by quaternions.
 * Calculates the transformation of a basic_vector<> by a basic_quat<>.
 *
 * @file   by_quat.h
 * @author David Gill
 * @date   22/12/2009
 */

#include "loci/numeric/maths.h"
#include "loci/numeric/detail/quat/basic_quat.h"
#include "loci/numeric/detail/vector/basic_vector.h"

namespace loci {
namespace numeric {
namespace transform
{
    template <typename T>
    basic_vector<T, 3> rotated_vector(const basic_vector<T, 3> & v, const basic_quat<T> & rotation)
    {
        const T a =  rotation.w() * rotation.x();
        const T b =  rotation.w() * rotation.y();
        const T c =  rotation.w() * rotation.z();
        const T d = -rotation.x() * rotation.x();
        const T e =  rotation.x() * rotation.y();
        const T f =  rotation.x() * rotation.z();
        const T g = -rotation.y() * rotation.y();
        const T h =  rotation.y() * rotation.z();
        const T i = -rotation.z() * rotation.z();
        return basic_vector<T, 3>(v.x() + 2*(v.x()*(g + i) + v.y()*(e - c) + v.z()*(b + f)),
                                  v.y() + 2*(v.x()*(c + e) + v.y()*(d + i) + v.z()*(h - a)),
                                  v.z() + 2*(v.x()*(f - b) + v.y()*(a + h) + v.z()*(d + g)));
    }

    template <typename T>
    basic_vector<T, 2> rotated_xz_heading(const basic_vector<T, 2> & v, T y, const basic_quat<T> & rotation)
    {
        basic_vector<T, 3> rotated = rotated_vector(basic_vector<T, 3>(v.x(), y, v.y()), rotation);
        return basic_vector<T, 2>(rotated.x(), rotated.z());
    }

    template <typename T>
    basic_quat<T> quat_between(const basic_vector<T, 3> & a, const basic_vector<T, 3> & b)
    {
        T sqr_len_product = a.length_squared() * b.length_squared();
        if (!approx_equal<T>(sqr_len_product, 1)) { sqr_len_product = sqrt(sqr_len_product); }

        return basic_quat<T>(sqr_len_product + dot(a, b),
                             cross(a, b)).normalise();
    }

    template <typename T>
    basic_quat<T> yrot_quat_between(const basic_vector<T, 2> & a, const basic_vector<T, 2> & b)
    {
        return quat_between(basic_vector<T, 3>(a.x(), 0, a.y()), basic_vector<T, 3>(b.x(), 0, b.y()));
    }
} // namesapce transform
} // namespace numeric
} // namespace loci

#endif // LOCI_NUMERIC_TRANSFORM_BY_QUAT_H_
