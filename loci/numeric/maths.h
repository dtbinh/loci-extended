#ifndef LOCI_NUMERIC_MATHS_H_
#define LOCI_NUMERIC_MATHS_H_

/**
 * Provides templated versions of some of <cmath> and mathematical constants
 * In the numerics framework it is useful to be able to apply mathematical
 * operations to templated, and therefore unknown, types. For correctness these
 * functions handle all the ugly casting (to double).
 *
 * @file   maths.h
 * @author David Gill
 * @date   21/02/2010
 */

#include <cmath>

namespace loci {
namespace numeric
{
    const float similarity_epsilon = 0.00000001f;

    const long double pi = 3.1415926535897932384626433832795;
    const long double inv_pi = 1.0 / pi;
    
    const long double rads_per_deg = pi / 180.0;
    const long double degs_per_rad = 180.0 * inv_pi;

    template <typename T>
    T abs(T x)
    {
        return static_cast<T>(std::abs(static_cast<double>(x)));
    }

    template <typename T>
    bool approx_equal(T x, T y)
    {
        return abs(x - y) <= static_cast<T>(similarity_epsilon);
    }

    template <typename T>
    T cos(T x)
    {
        return static_cast<T>(std::cos(static_cast<double>(x)));
    }

    template <typename T>
    T acos(T x)
    {
        return static_cast<T>(std::acos(static_cast<double>(x)));
    }
   
    template <typename T>
    T sin(T x)
    {
        return static_cast<T>(std::sin(static_cast<double>(x)));
    }

    template <typename T>
    T asin(T x)
    {
        return static_cast<T>(std::asin(static_cast<double>(x)));
    }

    template <typename T>
    T atan2(T y, T x)
    {
        return static_cast<T>(std::atan2(static_cast<double>(y), static_cast<double>(x)));
    }

    template <typename T>
    T sqrt(T x)
    {
        return static_cast<T>(std::sqrt(static_cast<double>(x)));
    }
} // namespace numeric
} // namespace loci

#endif // LOCI_NUMERIC_MATHS_H_
