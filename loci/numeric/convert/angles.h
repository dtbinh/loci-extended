#ifndef LOCI_NUMERIC_CONVERT_ANGLES_H_
#define LOCI_NUMERIC_CONVERT_ANGLES_H_

/**
 * Conversion routines between radians and degrees
 * Provides bi-directional conversion between angles in radians and degrees
 *
 * @file   angles.h
 * @author David Gill
 * @date   22/12/2009
 */

#include "loci/numeric/maths.h"

namespace loci {
namespace numeric {
namespace convert
{
    template <typename T>
    inline T to_degs(const T & radians)
    {
        return static_cast<T>(radians * degs_per_rad);
    }

    template <typename T>
    inline T to_rads(const T & degrees)
    {
        return static_cast<T>(degrees * rads_per_deg);
    }
} // namesapce convert
} // namespace numeric
} // namespace loci

#endif // LOCI_NUMERIC_CONVERT_ANGLES_H_
