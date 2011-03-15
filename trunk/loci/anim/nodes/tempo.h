#ifndef LOCI_ANIM_NODES_TEMPO_H_
#define LOCI_ANIM_NODES_TEMPO_H_

/**
 * Defines a generator node that modifies the pace of animation.
 * This node modifies the time parameter to change the tempo.
 *
 * @file   tempo.h
 * @author David Gill
 * @date   27/02/2010
 */

#include "loci/anim/blend_node.h"

namespace loci {
namespace anim
{ 
    // forward declaration
    class blend_info;

    class tempo : public blend_contract_node
    {
    public:
        tempo(double pace = 1.0)
            : pace(pace)
        {
        }

        tempo(const blend_node_ptr & input, double pace)
            : pace(pace)
            , input(connect(input))
        {
        }

        void anim(const blend_node_ptr & input)
        {
            this->input = connect(input);
        }

        double play_rate() { return pace; }
        void play_rate(double pace) { this->pace = pace; }

    private:
        void master_sync_properties(const time_info & sync_info, const blend_info & blending)
        {
            find_sync_properties(input, sync_info, blending);
        }

        void evaluate(const time_info & timing, const blend_info & info)
        {
            time_type rescaled_time = static_cast<time_type>(total_time() * pace);
            input->animate(time_info(rescaled_time, timing.time_delta() * pace, timing), info);
        }

        void contract_children(contractor & contract) const
        {
            contract.imbue(input);
        }

        double pace;
        blend_node_slot input;
    };
} // namespace anim
} // namespace loci

#endif // LOCI_ANIM_NODES_TEMPO_H_