/**
 * Implementation of weight masking.
 * Implements the weight masking algorithm.
 *
 * @file   weight_mask.cpp
 * @author David Gill
 * @date   27/04/2010
 */

#include <string>

#include "loci/named_indices.h"
#include "loci/hierarchy.h"
#include "loci/anim/blend_info.h"

namespace loci {
namespace anim
{
    namespace // anonymous
    {
        void weight_depth_recurse(blend_info::weight_list & weights, hierarchy::cursor c, unsigned int depth, double start_weight, double weight_change)
        {
            weights[c.position()] = start_weight + depth * weight_change;

            if (depth == 0) { return; }
            --depth;
            c.to_children();
            while (c.valid())
            {
                weight_depth_recurse(weights, c, depth, start_weight, weight_change);
                c.next();
            }
        }
    } // anonymous namespace

    void weight_mask_branch(blend_info::weight_list & weights, const named_indices & name_bindings, const hierarchy & structure,
                            const std::string & parent_name, unsigned int depth, double start_weight, double end_weight)
    {
            named_indices::index_type parent_index = name_bindings.index_of(parent_name);
            hierarchy::cursor parent = structure.cursor_at(parent_index);

            double weight_per_depth = (end_weight - start_weight) / static_cast<double>(depth);
            weight_depth_recurse(weights, parent, depth, start_weight, weight_per_depth);
    }
} // namespace anim
} // namespace loci