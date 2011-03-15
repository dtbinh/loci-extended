#ifndef LOCI_MOCAPS_DETAIL_BVH_SAVE_H_
#define LOCI_MOCAPS_DETAIL_BVH_SAVE_H_

/**
 * A bvh exporter.
 * Implements an exporter for bvh files, writing a scope_list in the bvh file format.
 *
 * @file   save.h
 * @author David Gill
 * @date   23/04/2010
 */

#include <istream>

#include "loci/mocaps/detail/bvh/scope.h"

namespace loci {
namespace mocaps {
namespace detail {
namespace bvh
{
    void save_hierarchy(std::ostream & sink, const scope_list & scopes);
    void save_motion(std::ostream & sink, const scope_list & scopes, unsigned int frame_count, float time_per_frame);
    void save(std::ostream & sink, const scope_list & scopes, unsigned int frame_count, float time_per_frame);
} // namespace bvh
} // namespace detail
} // namespace mocaps
} // namespace loci

#endif // LOCI_MOCAPS_DETAIL_BVH_SAVE_H_