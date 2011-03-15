#ifndef LOCI_ANIM_TIME_INFO_H_
#define LOCI_ANIM_TIME_INFO_H_

/**
 * Describes the properties of time.
 * time_info structures are propagated through the blend-tree and carry information
 * about how a node should time-sync itself.
 *
 * @file   time_info.h
 * @author David Gill
 * @date   28/04/2010
 */

#include <vector>

#include "loci/numeric/parametric.h"

namespace loci {
namespace anim
{
    typedef double time_type;

    class time_info
    {
    public:
        typedef unsigned int sync_group;

        time_info(time_type time, time_type time_delta, double & sync_position, sync_group & sync_stage)
            : time_(time)
            , time_delta_(time_delta)
            , sync_position_(&sync_position)
            , sync_stage_(&sync_stage)
        {
        }

        time_info(time_type time, time_type time_delta, const time_info & other)
            : time_(time)
            , time_delta_(time_delta)
            , sync_position_(other.sync_position_)
            , sync_stage_(other.sync_stage_)
        {
        }

        time_type time() const { return time_; }
        time_type time_delta() const { return time_delta_; }
        double & sync_position() const { return *sync_position_; }
        sync_group & sync_stage() const { return *sync_stage_; }

    private:
        time_type time_;
        time_type time_delta_;
        double * sync_position_;
        sync_group * sync_stage_;
    };

    class sync_warp
    {
    public:
        typedef unsigned int sync_group;

    private:
        struct group
        {
            group(sync_group type, time_type time) : type(type), time(time) { }
            sync_group type;
            time_type time;
        };

    public:
        void add_frame(sync_group g, time_type time_offset)
        {
            if (groups.empty())
            {
                groups.push_back(group(g, time_offset));
                return;
            }
            groups.push_back(group(g, groups.back().time + time_offset));
        }

        void master_sync_settings(const time_info & info) const
        {
            std::vector<group>::const_iterator iter = groups.begin();
            while (iter != groups.end() && iter->time < info.time())
            {
                ++iter;
            }
            if (iter == groups.end())
            {
                return; 
            }
            if (iter != groups.begin())
            {
                --iter;
            }
            info.sync_stage() = iter->type;
            info.sync_position() = (info.time() - iter->time) / ((iter + 1)->time - iter->time);
        }

        time_type sync(const time_info & timing, time_type sync_time, unsigned int & cache_pos) const
        {
            double position = timing.sync_position();
            sync_group g = timing.sync_stage();

            if (position < 0) { return sync_time + timing.time_delta(); }

            if (sync_time > 98)
            {
                sync_time += 0;
            }
           
            std::vector<group>::const_iterator iter = groups.begin() + cache_pos;
            while (iter != groups.end() && iter->time <= sync_time)
            {
                ++iter;
            }
            if (iter != groups.begin())
            {
                --iter;
            }

            while (iter != groups.end() && iter->type != g)
            {
                ++iter;
            }
            if (iter == groups.end() || (iter + 1) == groups.end())
            {
                return sync_time + timing.time_delta();
            }

            cache_pos = iter - groups.begin();
            return numeric::linear(iter->time, (iter + 1)->time, position);
        }

    private:
        std::vector<group> groups;
    };
} // namespace anim
} // namespace loci

#endif // LOCI_ANIM_TIME_INFO_H_