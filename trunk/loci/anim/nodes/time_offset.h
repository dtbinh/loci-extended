#ifndef LOCI_ANIM_NODES_TIME_OFFSET_H_
#define LOCI_ANIM_NODES_TIME_OFFSET_H_

/**
 * Defines a node that will offset the play time by some amount
 * This node can be used to synchronise multiple animations together by giving
 * each an offset to bring them into alignment. Only works with sequence_nodes.
 *
 * @file   time_offset.h
 * @author David Gill
 * @date   08/04/2010
 */

#include "loci/anim/sequence_node.h"

namespace loci {
namespace anim
{ 
    // forward declaration
    class blend_info;

    class time_offset : public sequence_node
    {
    public:
        time_offset()
            : offset_(0)
        {
        }

        time_offset(const seq_node_ptr & input, time_type offset)
            : input(connect(input))
            , offset_(offset)
        {
        }

        void anim(const seq_node_ptr & input, time_type offset)
        {
            this->input = connect(input);
            offset_ = offset;
        }

        time_type offset() const { return offset_; }
        void offset(time_type offset) { offset_ = offset; }

    private:
        void master_sync_properties(const time_info & sync_info, const blend_info & blending)
        {
            find_sync_properties(input, sync_info, blending);
        }

        void evaluate(const time_info & timing, const blend_info & info)
        {
            if (!input.connected()) { return; }
            input->animate(time_info(total_time() + offset_, timing.time_delta(), timing), info);
        }

        void contract_children(contractor & contract) const
        {
            contract.imbue(input);
        }

        time_type calculate_duration() const
        {
            return input->duration() - offset_;
        }

        seq_node_slot input;
        time_type offset_;
    };
} // namespace anim
} // namespace loci

#endif // LOCI_ANIM_NODES_TIME_OFFSET_H_