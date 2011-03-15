#ifndef LOCI_NUMERIC_VECTOR_H_
#define LOCI_NUMERIC_VECTOR_H_

/**
 * Spatial vector classes.
 * Provides an #include point for vectors, providing convenient typedefs
 *
 * @file   vector.h
 * @author David Gill
 * @date   21/05/2009
 */

#include "loci/numeric/detail/vector/basic_vector.h"
#include "loci/numeric/detail/vector/non_members.h"

namespace loci {
namespace numeric
{
    // alias some common concrete vector types
    typedef basic_vector<float, 2> vector2f;
    typedef basic_vector<float, 3> vector3f;
    typedef basic_vector<float, 4> vector4f;

    typedef basic_vector<double, 2> vector2d;
    typedef basic_vector<double, 3> vector3d;
    typedef basic_vector<double, 4> vector4d;

    typedef basic_vector<int, 2> vector2i;
    typedef basic_vector<int, 3> vector3i;
    typedef basic_vector<int, 4> vector4i;

    typedef basic_vector<long, 2> vector2l;
    typedef basic_vector<long, 3> vector3l;
    typedef basic_vector<long, 4> vector4l;
} // namespace numeric
} // namespace loci

#endif // LOCI_NUMERIC_VECTOR_H_
