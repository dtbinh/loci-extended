#ifndef LOCI_NUMERIC_PARAMETRIC_H_
#define LOCI_NUMERIC_PARAMETRIC_H_

/**
 * Parametric curve functions
 * Provides functions that evaluate various parametric polynomial curves.
 * Useful for interpolation.
 *
 * @file   parametric.h
 * @author David Gill
 * @date   06/02/2009
 */

namespace loci {
namespace numeric
{
    template <class T>
    double linear_weight(const T & first, const T & last, const T & middle)
    {
        return (static_cast<double>(middle) - first) / (static_cast<double>(last) - first);
    }

    template <class T>
    T linear(const T & a, const T & b, double t)
    {
        return static_cast<T>(a*(1.0 - t) + b*t);
    }

    template <class T, class U>
    U linear_scale(const T & first, const T & last, const T & middle, const U & low, const U & high)
    {
        return linear(low, high, linear_weight(first, last, middle));
    }
} // namespace numeric
} // namespace loci

#endif // LOCI_NUMERIC_PARAMETRIC_H_
