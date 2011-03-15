#ifndef LOCI_NUMERIC_DETAIL_QUAT_BASIC_QUAT_H_
#define LOCI_NUMERIC_DETAIL_QUAT_BASIC_QUAT_H_

/**
 * Spatial euler classes.
 * Defines a templates base class for 3D euler angles to specialise.
 *
 * @file   basic_euler.h
 * @author David Gill
 * @date   19/11/2009
 */

#include "loci/numeric/maths.h"
#include "loci/numeric/detail/simple_maths_sequence.h"
#include "loci/numeric/detail/xyzw_manipulators.h"

namespace loci {
namespace numeric
{
    template <typename T>
    class basic_quat : public detail::with_wxyz<detail::onesize_simple_maths_sequence<basic_quat, T, 4> >
    {
    public:
        basic_quat() { }
        basic_quat(const T & w, const T & x, const T & y, const T & z)
        {
            elems[0] = w;
            elems[1] = x;
            elems[2] = y;
            elems[3] = z;
        }
        template <class Vec>
        basic_quat(const T & w, const Vec & v)
        {
            elems[0] = w;
            elems[1] = v[0];
            elems[2] = v[1];
            elems[3] = v[2];
        }

        T magnitude_squared() const
        {
            return elems[0]*elems[0] + elems[1]*elems[1] +
                   elems[2]*elems[2] + elems[3]*elems[3];
        }

        T magnitude() const
        {
            return sqrt(magnitude_squared());
        }

        basic_quat & normalise()
        {
            const T mag = magnitude();
            return *this /= mag;
        }

        basic_quat & conjugate()
        {
            elems[1] = -elems[1];
            elems[2] = -elems[2];
            elems[3] = -elems[3];
            return *this;
        }

        template <typename U>
        basic_quat & operator*=(const basic_quat<U> & rhs)
        {
            reset(
                -x() * rhs.x() - y() * rhs.y() - z() * rhs.z() + w() * rhs.w(),
                 x() * rhs.w() + y() * rhs.z() - z() * rhs.y() + w() * rhs.x(),
                -x() * rhs.z() + y() * rhs.w() + z() * rhs.x() + w() * rhs.y(),
                 x() * rhs.y() - y() * rhs.x() + z() * rhs.w() + w() * rhs.z());
            return *this;
        }

        // re-expose scalar version
        basic_quat & operator*=(const T & rhs)
        {
            return maths_sequence_type::operator*=(rhs);
        }
    };
} // namespace numeric
} // namespace loci

#endif // LOCI_NUMERIC_DETAIL_QUAT_BASIC_QUAT_H_