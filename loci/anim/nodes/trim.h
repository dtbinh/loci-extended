#ifndef LOCI_ANIM_NODES_TRIM_H_
#define LOCI_ANIM_NODES_TRIM_H_

/**
 * Defines a node that trims off frames from the start and end of an animation.
 * This node only samples animation between a start and end time.
 *
 * @file   trim.h
 * @author David Gill
 * @date   08/04/2010
 */

#include "loci/anim/sequence_node.h"

namespace loci {
namespace anim
{ 
    // forward declaration
    class blend_info;

    class trim : public sequence_node
    {
    public:
        trim()
            : start(0)
            , duration(0)
        {
        }

        trim(const seq_node_ptr & input, time_type start, time_type end)
            : input(connect<blend_node>(input))
            , start(start)
            , duration(input->duration() - (start + end))
        {
        }

        trim(time_type start, time_type duration, const blend_node_ptr & input)
            : input(connect(input))
            , start(start)
            , duration(duration)
        {
        }

        void trim_ends_anim(const seq_node_ptr & input, time_type start, time_type end)
        {
            trim_duration_anim(input, start, input->duration() - start - end);
        }

        void trim_duration_anim(const blend_node_ptr & input, time_type start, time_type duration)
        {
            this->input = connect(input);
            this->start = start;
            this->duration = duration;
        }

    private:
        void master_sync_properties(const time_info & sync_info, const blend_info & blending)
        {
            find_sync_properties(input, sync_info, blending);
        }

        void evaluate(const time_info & timing, const blend_info & info)
        {
            time_type delta = timing.time_delta();
            if (!input.connected()) { return; }
            time_type time = total_time() + start;
            time_type next_delta = delta;
            if (total_time() > duration)
            {
                time = duration + start;
                next_delta = total_time() - duration;
                if (next_delta < delta)
                {
                    next_delta = delta - next_delta;
                }
                else
                {
                    next_delta = 0;
                }
            }
            input->animate(time_info(time, next_delta, timing), info);
        }

        void contract_children(contractor & contract) const
        {
            contract.imbue(input);
        }

        time_type calculate_duration() const
        {
            return duration;
        }


        blend_node_slot input;
        time_type start;
        time_type duration;
    };
} // namespace anim
} // namespace loci

#endif // LOCI_ANIM_NODES_TRIM_H_