#ifndef LOCI_ANIM_NODES_SYNC_GROUP_H_
#define LOCI_ANIM_NODES_SYNC_GROUP_H_

/**
 * Defines a generator node that modifies synchronises animations.
 * This node manages sync parameters to ensure child animations play
 * in synch.
 *
 * @file   sync_group.h
 * @author David Gill
 * @date   29/04/2010
 */

#include "loci/anim/blend_node.h"

namespace loci {
namespace anim
{ 
    // forward declaration
    class blend_info;

    class sync_blend : public blend_contract_node
    {
    public:
        sync_blend(const blend_node_ptr & input)
            : input(connect(input))
        {
        }

        void anim(const blend_node_ptr & input)
        {
            this->input = connect(input);
        }

    private:
        void master_sync_properties(const time_info & sync_info, const blend_info & blending)
        {
            find_sync_properties(input, sync_info, blending);
        }

        void evaluate(const time_info & timing, const blend_info & blending)
        {
            double sync_position = -1;
            time_info::sync_group sync_stage = 0;
            time_info syncing(timing.time(), timing.time_delta(), sync_position, sync_stage);
            find_sync_properties(input, syncing, blending);
            animate(input, syncing, blending);
        }

        void contract_children(contractor & contract) const
        {
            contract.imbue(input);
        }

        blend_node_slot input;
    };
} // namespace anim
} // namespace loci

#endif // LOCI_ANIM_NODES_SYNC_GROUP_H_