#ifndef LOCI_ANIM_NODES_MIN_DURATION_H_
#define LOCI_ANIM_NODES_MIN_DURATION_H_

/**
 * Defines a sequence_node that adapts a blend_node giving it a duration.
 * This node is an adapter interface for blend_node in order to use one in a
 * parent node that expects sequence_node's. It takes a user-provided duration
 * value to use as the blend_node's duration. Once time has exceeded this
 * duration value this node will stop animating.
 *
 * @file   apply_duration.h
 * @author David Gill
 * @date   07/04/2010
 */

#include "loci/anim/sequence_node.h"

namespace loci {
namespace anim
{
    // forward declaration
    class blend_info;

    class apply_duration : public sequence_node
    {
    public:
        apply_duration()
            : duration_(0)
        {
        }

        apply_duration(const blend_node_ptr & input, time_type duration)
            : input(connect(input))
            , duration_(duration)
        {
        }

        void anim(const blend_node_ptr & input, time_type time)
        {
            this->input = connect(input);
            duration_ = time;
        }

        void duration(time_type time)
        {
            duration_ = time;
        }

    private:
        void master_sync_properties(const time_info & sync_info, const blend_info & blending)
        {
            find_sync_properties(input, sync_info, blending);
        }

        void evaluate(const time_info & timing, const blend_info & info)
        {
            if (!input.connected()) { return; }
            time_type time = total_time();
            time_type next_delta = timing.time_delta();
            if (total_time() > duration_)
            {
                time = duration_;
                next_delta = total_time() - duration_;
                if (next_delta < timing.time_delta())
                {
                    next_delta = timing.time_delta() - next_delta;
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
            return duration_;
        }

        blend_node_slot input;
        time_type duration_;
    };

    class alter_duration : public sequence_node
    {
    public:
        alter_duration()
            : pace(1)
        {
        }

        alter_duration(time_type desired_duration, const seq_node_ptr & input)
            : input(connect(input))
        {
            duration(desired_duration);
        }

        alter_duration(const seq_node_ptr & input, double pace)
            : input(connect(input))
            , pace(pace)
        {
        }

        void anim_with_duration(const seq_node_ptr & input, time_type time)
        {
            this->input = connect(input);
            duration(time);
        }

        void anim_with_pace(const seq_node_ptr & input, double pace)
        {
            this->input = connect(input);
            this->pace = pace;
        }

        void repace(double pace)
        {
            this->pace = pace;
        }

        void duration(time_type time)
        {
            if (!input.connected()) { return; }
            pace = input->duration() / time;
        }

    private:
        void master_sync_properties(const time_info & sync_info, const blend_info & blending)
        {
            find_sync_properties(input, sync_info, blending);
        }

        void evaluate(const time_info & timing, const blend_info & info)
        {
            if (!input.connected()) { return; }
            time_type rescaled_time = static_cast<time_type>(total_time() * pace);
            input->animate(time_info(rescaled_time, timing.time_delta() * pace, timing), info);
        }

        void contract_children(contractor & contract) const
        {
            contract.imbue(input);
        }

        time_type calculate_duration() const
        {
            if (!input.connected()) { return 0; }
            return input->duration() / pace;
        }

        seq_node_slot input;
        double pace;
    };
} // namespace anim
} // namespace loci

#endif // LOCI_ANIM_NODES_MIN_DURATION_H_