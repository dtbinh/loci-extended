#ifndef LOCI_ANIM_NODES_FEATHER_H_
#define LOCI_ANIM_NODES_FEATHER_H_

/**
 * Defines a sequence node node that samples keyframed motion.
 * This node essentially adapts the motion interface to that of the anim-tree.
 *
 * @file   feather.h
 * @author David Gill
 * @date   31/03/2010
 */

#include <string>

#include "loci/named_indices.h"
#include "loci/hierarchy.h"
#include "loci/anim/blend_info.h"
#include "loci/anim/blend_node.h"
#include "loci/anim/weight_mask.h"

namespace loci {
namespace anim
{
    class feather : public blend_node
    {
    public:
        feather(const boost::shared_ptr<const named_indices> & name_bindings,
                const boost::shared_ptr<const hierarchy> & structure,
                const blend_info::weight_list & weights)
            : name_bindings(name_bindings)
            , structure(structure)
            , weights(weights)
        {
        }

        feather(const boost::shared_ptr<const named_indices> & name_bindings,
                const boost::shared_ptr<const hierarchy> & structure)
            : name_bindings(name_bindings)
            , structure(structure)
            , src_weights(structure->size())
            , msk_weights(structure->size())
            , weights(structure->size(), 0.0)
        {

        }

        void source_anim(const blend_node_ptr & node)
        {
            source = connect(node);
        }

        void mask_anim(const blend_node_ptr & node)
        {
            mask = connect(node);
        }

        void mask_branch(const std::string & parent_name, unsigned int depth, double start_weight, double end_weight)
        {
            weight_mask_branch(weights, *name_bindings, *structure, parent_name, depth, start_weight, end_weight);
        }

        void mask_weights(const blend_info::weight_list & weights)
        {
            this->weights = weights;
        }

    private:
        void contract_children(contractor & contract) const
        {
            contract.imbue(source);
            contract.imbue(mask);
        }

        void master_sync_properties(const time_info & sync_info, const blend_info & blending)
        {
            if (src_weights[0] > msk_weights[0])
            {
                find_sync_properties(source, sync_info, blending);
            }
            else
            {
                find_sync_properties(mask, sync_info, blending);
            }
        }
        
        void evaluate(const time_info & timing, const blend_info & info)
        {
            for (blend_info::weight_list::size_type i = 0; i < weights.size(); ++i)
            {
                src_weights[i] = info.weights()[i] * weights[i];
                msk_weights[i] = info.weights()[i] * (1 - weights[i]);
            }
            animate(source, timing, blend_info(src_weights, info));
            animate(mask, timing,  blend_info(msk_weights, info));
        }

    private:
        blend_node_slot source;
        blend_node_slot mask;

        boost::shared_ptr<const named_indices> name_bindings;
        boost::shared_ptr<const hierarchy>     structure;

        blend_info::weight_list src_weights;
        blend_info::weight_list msk_weights;
        blend_info::weight_list weights;
    };
} // namespace anim
} // namespace loci


#endif // LOCI_ANIM_NODES_FEATHER_H_