#ifndef LOCI_ANIM_WEIGHT_MASK_H_
#define LOCI_ANIM_WEIGHT_MASK_H_

/**
 * Defines a weight-masking function.
 * Calculates weight masks over a weight-list by masking away a
 * specified branch in the hierarchy.
 *
 * @file   weight_mask.h
 * @author David Gill
 * @date   27/04/2010
 */

#include <string>

#include "loci/anim/blend_info.h"

namespace loci
{
    // forward declarations
    class named_indicies;
    class hierarchy;

    namespace anim
    {
        void weight_mask_branch(blend_info::weight_list & weights, const named_indices & name_bindings, const hierarchy & structure,
                                const std::string & parent_name, unsigned int depth, double start_weight, double end_weight);
    } // namespace anim
} // namespace loci

#endif // LOCI_ANIM_WEIGHT_MASK_H_