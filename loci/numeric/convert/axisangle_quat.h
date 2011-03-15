#ifndef LOCI_NUMERIC_CONVERT_AXISANGLE_QUAT_H_
#define LOCI_NUMERIC_CONVERT_AXISANGLE_QUAT_H_

/**
 * Conversion routines between axis angles and quats
 * Provides conversion functionality for converting between equivalent
 * representations of an axis (basic_vector<>) with an angle and a basic_quat<>.
 *
 * @file   axisangle_quat.h
 * @author David Gill
 * @date   22/12/2009
 */

#include "loci/numeric/maths.h"
#include "loci/numeric/convert/angles.h"
#include "loci/numeric/detail/quat/basic_quat.h"
#include "loci/numeric/detail/vector/basic_vector.h"

namespace loci {
namespace numeric {
namespace convert
{
    template <typename T>
    basic_quat<T> quat_from_axisangle(T x, T y, T z, T angle)
    {
        angle = to_rads(angle) / T(2.0); // reusing angle variable
        T s = sin(angle);
        return basic_quat<T>(cos(angle), x*s, y*s, z*s);
    }

    template <typename T>
    basic_quat<T> quat_from_axisangle(const basic_vector<T, 3> & axis_normal, T angle)
    {
        return quat_from_axisangle(axis_normal.x(), axis_normal.y(), axis_normal.z(), angle);
    }


} // namesapce convert
} // namespace numeric
} // namespace loci

#endif // LOCI_NUMERIC_CONVERT_AXISANGLE_QUAT_H_
