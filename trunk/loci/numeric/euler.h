#ifndef LOCI_NUMERIC_EULER_H_
#define LOCI_NUMERIC_EULER_H_

/**
 * Spatial Euler classes.
 * Provides an #include point for Eulers, providing convenient typedefs
 *
 * @file   euler.h
 * @author David Gill
 * @date   20/12/2009
 */

#include "loci/numeric/detail/euler/basic_euler.h"
#include "loci/numeric/detail/euler/non_members.h"

namespace loci {
namespace numeric
{
    // alias some common concrete euler types
    typedef basic_euler<float>  eulerf;
    typedef basic_euler<double> eulerd;
    typedef basic_euler<int>    euleri;
    typedef basic_euler<long>   eulerl;
} // namespace numeric
} // namespace loci

#endif // LOCI_NUMERIC_EULER_H_
