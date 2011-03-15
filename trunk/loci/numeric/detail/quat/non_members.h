#ifndef LOCI_NUMERIC_DETAIL_EULER_NON_MEMBERS_H_
#define LOCI_NUMERIC_DETAIL_EULER_NON_MEMBERS_H_

/**
 * Quaternion functionality.
 * Defines non member functions for basic_quats.
 *
 * @file   non_members.h
 * @author David Gill
 * @date   22/02/2010
 */

#include "loci/numeric/detail/quat/basic_quat.h"
#include "loci/numeric/parametric.h"
#include "loci/numeric/maths.h"
#include "loci/numeric/detail/simple_maths_sequence.h"

namespace loci {
namespace numeric
{
    LOCI_ONESIZE_SIMPLE_MATHS_SEQUENCE_ALL_NON_MEMBERS(basic_quat)

    template <typename T>
    basic_quat<T> operator*(const basic_quat<T> & lhs, const basic_quat<T> & rhs)
    {
        return basic_quat<T>(lhs) *= rhs;
        
    }

    template <typename T>
    inline bool approx_zero(const basic_quat<T> & q)
    {
        return approx_equal(q.magnitude_squared(), T(0));
    }

    template <typename T>
    inline basic_quat<T> & try_normalise_inplace(basic_quat<T> & q)
    {
        return approx_zero(q) ? q : q.normalise();
    }

    template <typename T>
    basic_quat<T> normalise(const basic_quat<T> & q)
    {
        return basic_quat<T>(q).normalise();
    }

    template <typename T>
    T dot(const basic_quat<T> & lhs, const basic_quat<T> & rhs)
    {
        return lhs.w() * rhs.w() + lhs.x() * rhs.x() +
               lhs.y() * rhs.y() + lhs.z() * rhs.z();
    }

    template <typename T>
    basic_quat<T> conjugate(const basic_quat<T> & q)
    {
        return basic_quat<T>(q).conjugate();
    }

    template <typename T>
    basic_quat<T> & invert_inplace(basic_quat<T> & q)
    {
        return q.conjugate().normalise();
    }

    template <typename T>
    basic_quat<T> & invert_unit_inplace(basic_quat<T> & q)
    {
        return q.conjugate();
    }

    template <typename T>
    basic_quat<T> invert(const basic_quat<T> & q)
    {
        return basic_quat<T>(q).conjugate().normalise();
    }

    template <typename T>
    basic_quat<T> invert_unit(const basic_quat<T> & q)
    {
        return basic_quat<T>(q).conjugate();
    }

    template <typename T>
    basic_quat<T> ln(const basic_quat<T> & q)
    {
        float len = sqrt(q.x()*q.x() + q.y()*q.y() + q.z()*q.z());
        if (len < 0.000001)
        {
            return basic_quat<T>(0.0, q.x(), q.y(), q.z());
        }
        float coef = acos(q.w()) / len;
        return basic_quat<T>(0.0, q.x() * coef, q.y() * coef, q.z() * coef);
    }

    template <typename T>
    basic_quat<T> exp(const basic_quat<T> & q)
    {
        float theta = sqrt(q.x()*q.x() + q.y()*q.y() + q.z()*q.z());
        if (theta < 0.000001)
        {
            return basic_quat<T>(cos(theta), q.x(), q.y(), q.z());
        }
        float coef = sin(theta) / theta;
        return basic_quat<T>(cos(theta), q.x() * coef, q.y() * coef, q.z() * coef);
    }

    template <typename T>
    basic_quat<T> lerp(const basic_quat<T> & from, basic_quat<T> to, double t)
    {
        T inv_to = dot(from, to) < 0 ? -1 : 1;
        return basic_quat<T>(linear(from.w(), to.w() * inv_to, t),
                             linear(from.x(), to.x() * inv_to, t),
                             linear(from.y(), to.y() * inv_to, t),
                             linear(from.z(), to.z() * inv_to, t));
    }

    template <typename T>
    basic_quat<T> nlerp(const basic_quat<T> & from, basic_quat<T> to, double t)
    {
        T inv_to = dot(from, to) < 0 ? -1 : 1;
        return basic_quat<T>(linear(from.w(), to.w() * inv_to, t),
                             linear(from.x(), to.x() * inv_to, t),
                             linear(from.y(), to.y() * inv_to, t),
                             linear(from.z(), to.z() * inv_to, t)).normalise();
    }

    template <typename T>
    basic_quat<T> lerp2(const basic_quat<T> & from, const basic_quat<T> & to, double t, T smooth_margin)
    {
        const T dp = dot(from, to);
        const T sqr_halfangle = dp*dp / (from.magnitude_squared() * to.magnitude_squared()) * (dp < 0 ? -1 : 1);//dot(normalise(from), normalise(to));

        smooth_margin *= smooth_margin;
        bool inversion = sqr_halfangle < smooth_margin;
        bool smooth_inversion = inversion && sqr_halfangle > -smooth_margin;

        const T inv_to = inversion ? -1 : 1;

        basic_quat<T> q(linear(from.w(), to.w() * inv_to, t),
                        linear(from.x(), to.x() * inv_to, t),
                        linear(from.y(), to.y() * inv_to, t),
                        linear(from.z(), to.z() * inv_to, t));

        if (smooth_inversion)
        {
            basic_quat<T> non_inv_q(linear(from.w(), to.w(), t),
                                    linear(from.x(), to.x(), t),
                                    linear(from.y(), to.y(), t),
                                    linear(from.z(), to.z(), t));

            return lerp2(q, non_inv_q, linear_weight(-smooth_margin, smooth_margin, sqr_halfangle), smooth_margin);
        }

        return q;
    }

    template <typename T>
    basic_quat<T> nlerp2(const basic_quat<T> & from, const basic_quat<T> & to, double t, T smooth_margin)
    {
        T halfangle = dot(from, to);

        bool inversion = halfangle < smooth_margin;
        bool smooth_inversion = inversion && halfangle > -smooth_margin &&
                                t < (1.0 - numeric::similarity_epsilon) && t > (0.0 + numeric::similarity_epsilon);

        T inv_to = inversion ? -1 : 1;

        basic_quat<T> q(linear(from.w(), to.w() * inv_to, t),
                        linear(from.x(), to.x() * inv_to, t),
                        linear(from.y(), to.y() * inv_to, t),
                        linear(from.z(), to.z() * inv_to, t));
        q.normalise();

        if (smooth_inversion)
        {
            basic_quat<T> non_inv_q(linear(from.w(), to.w(), t),
                                    linear(from.x(), to.x(), t),
                                    linear(from.y(), to.y(), t),
                                    linear(from.z(), to.z(), t));
            non_inv_q.normalise();
            return nlerp2(q, non_inv_q, linear_weight(-smooth_margin, smooth_margin, halfangle), smooth_margin);
        }

        return q;
    }

    template <typename T>
    void alerp(basic_quat<T> & destination, const basic_quat<T> & q, double t, T smooth_margin = T(0.2))
    {
        const T dp = dot(destination, q);
        const T sqr_halfangle = dp*dp / destination.magnitude_squared() * (dp < 0 ? -1 : 1);//dot(normalise(from), normalise(to));

        smooth_margin *= smooth_margin;
        const bool inversion = sqr_halfangle < smooth_margin;
        const bool smooth_inversion = inversion && sqr_halfangle > -smooth_margin;

        const T inv_q = inversion ? -1 : 1;

        basic_quat<T> r(destination.w() + q.w() * inv_q * t,
                        destination.x() + q.x() * inv_q * t,
                        destination.y() + q.y() * inv_q * t,
                        destination.z() + q.z() * inv_q * t);

        if (smooth_inversion)
        {
            basic_quat<T> non_inv_r(destination.w() + q.w() * t,
                                    destination.x() + q.x() * t,
                                    destination.y() + q.y() * t,
                                    destination.z() + q.z() * t);
            
            destination = lerp2(r, non_inv_r, linear_weight(-smooth_margin, smooth_margin, sqr_halfangle), smooth_margin);
            return;
        }

        destination = r;
    }

} // namespace numeric
} // namespace loci

#endif // LOCI_NUMERIC_DETAIL_EULER_NON_MEMBERS_H_