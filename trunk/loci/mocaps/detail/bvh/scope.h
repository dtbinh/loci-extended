#ifndef LOCI_MOCAPS_DETAIL_BVH_SCOPE_H_
#define LOCI_MOCAPS_DETAIL_BVH_SCOPE_H_

/**
 * A bvh scope.
 * Models a scope block and hierarchy of scopes from a bvh file.
 *
 * @file   scope.h
 * @author David Gill
 * @date   22/04/2010
 */

#include <string>
#include <vector>

#include "loci/numeric/vector.h"

namespace loci {
namespace mocaps {
namespace detail {
namespace bvh
{
    struct channel_indices
    {
        typedef unsigned int type;
        static const type x_position = 0;
        static const type y_position = 1;
        static const type z_position = 2;
        static const type x_rotation = 3;
        static const type y_rotation = 4;
        static const type z_rotation = 5;
    };

    struct scope
    {
        typedef std::vector<channel_indices::type> channel_index_list;
        typedef std::vector<float> motion_channel_list;

        std::string name;
        numeric::vector3f offset;
        channel_index_list channels;
        motion_channel_list motion;
        unsigned int depth;

        explicit scope(unsigned int depth) : depth(depth), offset(0)
        {
            channels.reserve(3); // typical case: xrot, yrot, zrot
        }

        scope(const numeric::vector3f & offset,
              unsigned int depth,
              unsigned int motion_reserve)
            : offset(offset)
            , depth(depth)
        {
            channels.reserve(3); // typical case: xrot, yrot, zrot
            motion.reserve(motion_reserve);
        }

        bool end_site() const
        {
            return channels.empty();
        }
    };

    typedef std::vector<scope> scope_list;
} // namespace bvh
} // namespace detail
} // namespace mocaps
} // namespace loci

#endif // LOCI_MOCAPS_DETAIL_BVH_SCOPE_H_