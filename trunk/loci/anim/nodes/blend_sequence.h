#ifndef LOCI_ANIM_NODES_BLEND_SEQUENCE_H_
#define LOCI_ANIM_NODES_BLEND_SEQUENCE_H_

/**
 * Defines a blend node that cross-blends other sequence nodes together.
 * This node applies a blending at the start and end of individual animations.
 *
 * @file   sequencer.h
 * @author David Gill
 * @date   30/03/2010
 */


#include <vector>

#include "loci/anim/sequence_node.h"
#include "loci/anim/blend_info.h"
#include "loci/numeric/parametric.h"

namespace loci {
namespace anim
{
    class blend_sequence : public sequence_node
    {
    private:
        struct stage
        {
            stage(const seq_node_slot & node, time_type overlap)
                : node(node)
                , overlap(overlap)
                , duration(node->duration())
            {
            }
            
            seq_node_slot node;
            time_type overlap;
            time_type duration;
        };

        typedef std::vector<stage> stage_list;

    public:
        typedef stage_list::size_type size_type;

        blend_sequence(bool loop = false)
            : looping_(loop)
            , cached_duration(0)
            , time_at_stage_start(0)
            , current_stage(0)
        {
        }

        void append(const seq_node_ptr & node, time_type overlap)
        {
            inputs.push_back(stage(connect(node), overlap));
            cached_duration += (inputs.back().duration - overlap);
        }

        size_type size() const { return inputs.size(); }
        bool empty() const { return inputs.empty(); }

        bool looping() { return looping_; }
        void looping(bool loop) { looping_ = loop; }

    private:
        void master_sync_properties(const time_info & sync_info, const blend_info & blending)
        {
            if (inputs.empty()) { return; }
            time_type time = total_time();
            if (looping_) { wrap_around(time); }
            else          {       clamp(time); }

            stage_list::iterator       first = seek_to(time);   
            stage_list::const_iterator last  = expand_window(time);
            if (inputs.size() == 1)
            {
                find_sync_properties(first->node, sync_info, blending);
                return;
            }

            stage_list::iterator master =
                find_master(first, last, time_at_stage_start, time, blending, -1.0, first);

            find_sync_properties(master->node, sync_info, blending);
            
        }

        void evaluate(const time_info & timing, const blend_info & info)
        {
            if (inputs.empty()) { return; }
            time_type time = total_time();
            if (looping_) { wrap_around(time); }
            else          {       clamp(time); }

            stage_list::iterator       first = seek_to(time);   
            stage_list::const_iterator last  = expand_window(time);
            if (inputs.size() == 1)
            {
                inputs.front().node->animate(timing, info);
                return;
            }
            cross_fade(first, last, time_at_stage_start, time, timing, info);
        }

        void contract_children(contractor & contract) const
        {
            for(stage_list::const_iterator child = inputs.begin();
                child != inputs.end();
                ++child)
            {
                contract.imbue(child->node);
            }
        }

        time_type calculate_duration() const
        {
            return cached_duration;
        }

        void cross_fade
            (stage_list::iterator current,
            stage_list::const_iterator end,
            time_type start_time,
            time_type time,
            const time_info & timing,
            const blend_info & info)
        {
            stage & s = *current;
            ++current;
            if (current == inputs.end() && looping_) { current = inputs.begin(); }
            if (current == end)
            {
                return s.node->animate(time_info(time - start_time, timing.time_delta(), timing), info);
            }
            
            time_type end_time = start_time + s.duration;
            time_type overlap_time = end_time - current->overlap;
           
            double inv_weight = numeric::linear_weight(overlap_time, end_time, time);
            double current_weight = info.amount() * (1.0 - inv_weight);
            double next_weight = info.amount() * inv_weight;

            s.node->animate(time_info(time - start_time, timing.time_delta(), timing), blend_info(current_weight, info));
            cross_fade(current, end, overlap_time, time, timing, blend_info(next_weight, info));
        }

        stage_list::iterator find_master
            (stage_list::iterator current,
            stage_list::const_iterator end,
            time_type start_time,
            time_type time,
            const blend_info & info,
            double best_weight,
            stage_list::iterator best_master)
        {
            stage & s = *current;
            stage_list::iterator current_for_master = current;
            ++current;
            if (current == inputs.end() && looping_) { current = inputs.begin(); }
            if (current == end)
            {
                if (info.amount() > best_weight) { best_master = current_for_master; }
                return best_master;
            }
            
            time_type end_time = start_time + s.duration;
            time_type overlap_time = end_time - current->overlap;
           
            double inv_weight = numeric::linear_weight(overlap_time, end_time, time);
            double current_weight = info.amount() * (1.0 - inv_weight);
            double next_weight = info.amount() * inv_weight;

            if (current_weight > best_weight) { best_master = current_for_master; best_weight = current_weight; }
            return find_master(current, end, overlap_time, time, blend_info(next_weight, info), best_weight, best_master);
        }

        time_type time_at_stage_end(stage_list::const_iterator current) const
        {
            return time_at_stage_start + current->duration;
        }

        void wrap_around(time_type & time) const
        {
            time_type total_duration = cached_duration + inputs.front().overlap;
            time_type overlap = inputs.size() > 1 ? inputs.front().overlap : 0;
            time_type time_at_end = total_duration - overlap;

            if (time > total_duration)
            {
                time -= overlap;
                while (time > time_at_end) { time -= time_at_end; }
                time += overlap;
            }
            else if (time < 0)
            {
                while (-time > cached_duration) { time += cached_duration; }
                time = cached_duration - (-time);
                time += inputs.front().overlap;
            }
        }

        void clamp(time_type & time) const
        {
            const time_type upper = cached_duration + inputs.front().overlap;
            if (time > upper)
            {
                time = upper;
            }
            else if (time < 0)
            {
                time = 0;
            }
        }

        stage_list::iterator seek_to(time_type time)
        {
            stage_list::iterator current = inputs.begin() + current_stage;
            size_type stop = inputs.size() - 1;

            while (time > time_at_stage_end(current) && current_stage < stop)
            {
                time_at_stage_start += current->duration;
                ++current;
                ++current_stage;
                time_at_stage_start -= current->overlap;
            }
            while (time < time_at_stage_start + current->overlap && current != inputs.begin())
            {
                time_at_stage_start += current->overlap;
                --current;
                time_at_stage_start -= current->duration;
            }

            current_stage = current - inputs.begin();
            return current;
        }

        stage_list::const_iterator expand_window(time_type time) const
        {
            stage_list::const_iterator iter = inputs.begin() + current_stage;
            
            time_type time_at_window_end = time_at_stage_start + iter->duration;

            for (;;)
            {
                if (++iter == inputs.end())
                {
                    if (!looping_) { break; }
                    iter = inputs.begin();
                }
                time_at_window_end -= iter->overlap;
                if (time < time_at_window_end) { break; }
                time_at_window_end += iter->duration;
            }

            return iter;
        }

    private:
        bool looping_;
        time_type cached_duration;
        time_type time_at_stage_start;
        stage_list::size_type current_stage;
        stage_list inputs;
    };
} // namespace anim
} // namespace loci

#endif // LOCI_ANIM_NODES_BLEND_SEQUENCE_H_