#ifndef LOCI_NUMERIC_QUAT_H_
#define LOCI_NUMERIC_QUAT_H_

/**
 * Spatial unit quaternion classes.
 * Provides an #include point for quaternions, providing convenient typedefs
 *
 * @file   quat.h
 * @author David Gill
 * @date   22/12/2009
 */

#include "loci/numeric/detail/quat/basic_quat.h"
#include "loci/numeric/detail/quat/non_members.h"

namespace loci {
namespace numeric
{
    // alias some common concrete euler types
    typedef basic_quat<float>  quatf, quaternionf;
    typedef basic_quat<double> quatd, quaterniond;
} // namespace numeric
} // namespace loci

#endif // LOCI_NUMERIC_QUAT_H_
