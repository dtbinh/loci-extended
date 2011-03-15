#ifndef LOCI_NUMERIC_CONVERT_EULER_QUAT_H_
#define LOCI_NUMERIC_CONVERT_EULER_QUAT_H_

/**
 * Conversion routines between eulers and quats
 * Provides conversion functionality for converting between equivalent
 * representations of basic_euler<> and basic_quat<>.
 *
 * @file   euler_quat.h
 * @author David Gill
 * @date   22/12/2009
 */

#include "loci/numeric/maths.h"
#include "loci/numeric/convert/angles.h"
#include "loci/numeric/detail/quat/basic_quat.h"
#include "loci/numeric/detail/euler/basic_euler.h"

namespace loci {
namespace numeric {
namespace convert
{
    template <typename T>
    basic_quat<T> quat_from_euler(T yaw, T pitch, T roll)
    {
        yaw   = to_rads(yaw);
        pitch = to_rads(pitch);
        roll  = to_rads(roll);

        T c1 = cos(yaw   / T(2.0));
        T c2 = cos(roll  / T(2.0));
        T c3 = cos(pitch / T(2.0));
        T s1 = sin(yaw   / T(2.0));
        T s2 = sin(roll  / T(2.0));
        T s3 = sin(pitch / T(2.0));

        return basic_quat<T>(c1*c2*c3 - s1*s2*s3,
                             s1*s2*c3 + c1*c2*s3,
                             s1*c2*c3 + c1*s2*s3,
                             c1*s2*c3 - s1*c2*s3); 
    }

    template <typename T>
    basic_quat<T> to_quat(const basic_euler<T> & e)
    {
        return quat_from_euler(e.yaw(), e.pitch(), e.roll());
    }

    template <typename T>
    basic_euler<T> to_euler_xyz(const basic_quat<T> & q)
    {
        T test =  q.x()*q.y() + q.z()*q.w();
        if (test > 0.4999)
        {
            return basic_euler<T>(T(0), to_degs( 2.0 * atan2(q.x(), q.w()) ), T(90));
        }
        else if (test < -0.4999)
        {
            return basic_euler<T>(T(0), to_degs( -2.0 * atan2(q.x(), q.w()) ), T(-90));
        }

        T sqrx = q.x() * q.x();
        T sqry = q.y() * q.y();
        T sqrz = q.z() * q.z();
        return basic_euler<T>(to_degs( atan2(2.0 * (q.w()*q.x() - q.z()*q.y()), 1.0 - 2.0 * (sqrx + sqrz)) ),
                              to_degs( atan2(2.0 * (q.w()*q.y() - q.x()*q.z()), 1.0 - 2.0 * (sqry + sqrz)) ),
                              to_degs( asin(2.0 * test) ));
    }
} // namesapce convert
} // namespace numeric
} // namespace loci

#endif // LOCI_NUMERIC_CONVERT_EULER_QUAT_H_
