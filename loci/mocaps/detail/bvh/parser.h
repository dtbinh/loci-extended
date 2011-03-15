#ifndef LOCI_MOCAPS_DETAIL_BVH_PARSER_H_
#define LOCI_MOCAPS_DETAIL_BVH_PARSER_H_

/**
 * A bvh parser.
 * Models a scope block and hierarchy of scopes from a bvh file.
 * Also provides boost::xpressive::function functors for populating scopes.
 *
 * @file   parser.h
 * @author David Gill
 * @date   20/02/2010
 */

#include <istream>

#include "loci/mocaps/detail/bvh/scope.h"

namespace loci {
namespace mocaps {
namespace detail {
namespace bvh
{
    void parse(std::istream & is, scope_list & result, unsigned int & num_frames, float & time_per_frame);
    void parse_hierarchy(std::istream & is, scope_list & result);
    void parse_motion(std::istream & is, scope_list & target, unsigned int & num_frames, float & time_per_frame);
} // namespace bvh
} // namespace detail
} // namespace mocaps
} // namespace loci

#endif // LOCI_MOCAPS_DETAIL_BVH_PARSER_H_