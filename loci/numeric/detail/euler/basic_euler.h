#ifndef LOCI_NUMERIC_DETAIL_EULER_BASIC_EULER_H_
#define LOCI_NUMERIC_DETAIL_EULER_BASIC_EULER_H_

/**
 * Spatial euler classes.
 * Defines a templates base class for 3D euler angles to specialise.
 *
 * @file   basic_euler.h
 * @author David Gill
 * @date   19/11/2009
 */

#include "loci/numeric/detail/simple_maths_sequence.h"
#include "loci/numeric/detail/xyzw_manipulators.h"

namespace loci {
namespace numeric
{
    template <typename T>
    class basic_euler : public detail::with_xyz<detail::onesize_simple_maths_sequence<basic_euler, T, 3> >
    {
    public:
        basic_euler() { }
        explicit basic_euler(const T & value) { assign(value); }
        basic_euler(const T & x, const T & y, const T & z)
        {
            elems[0] = x;
            elems[1] = y;
            elems[2] = z;
        }

        T yaw() const
        {
            return y();
        }

        T pitch() const
        {
            return x();
        }

        T roll() const
        {
            return z();
        }
    };
} // namespace numeric
} // namespace loci

#endif // LOCI_NUMERIC_DETAIL_EULER_BASIC_EULER_H_