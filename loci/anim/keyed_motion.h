#ifndef LOCI_ANIM_TRACK_H_
#define LOCI_ANIM_TRACK_H_

/**
 * An keyframed animation track.
 * Stores a sequence of keyframes
 *
 * @file   keyed_motion.h
 * @author David Gill
 * @date   01/01/2010
 */

#include <vector>
#include <memory>
#include <stdexcept>

namespace loci {
namespace anim
{
    template <class T>
    class keyed_motion
    {
    private:
        struct keyframe
        {
            typedef T      value_type;
            typedef double time_type;

            keyframe(time_type time_stamp, const value_type & value)
                : time_stamp(time_stamp)
                , value(value)
            {
            }

            keyframe(time_type time_stamp, std::auto_ptr<value_type> value)
                : time_stamp(time_stamp)
            {
                this->value.swap(*value);
            }

            time_type time_stamp;
            value_type value;
        };

        typedef std::vector<keyframe> frame_list;

    public:
        typedef typename keyframe::value_type  value_type;
        typedef typename keyframe::time_type   time_type;
        typedef typename frame_list::size_type position_type;
        typedef typename frame_list::size_type size_type;

    public:
        keyed_motion(size_type reserve_frames = 100)
        {
            frames.reserve(reserve_frames);
        }

        void insert(time_type time, const value_type & frame)
        {
            frames.insert(find_insertion_point(time), keyframe(time, frame));
        }

        void insert(time_type time, std::auto_ptr<value_type> frame)
        {
            frames.insert(find_insertion_point(time), keyframe(time, frame));
        }

        void append(time_type after, const value_type & frame)
        {
            frames.push_back(keyframe(next_timestamp(after), frame));
        }

        void append(time_type after, std::auto_ptr<value_type> frame)
        {
            frames.push_back(keyframe(next_timestamp(after), frame));
        }

        time_type duration() const
        {
            return empty() ? 0 : (frames.back().time_stamp - frames.front().time_stamp);
        }

        position_type start_position() const
        {
            return 0;
        }

        position_type end_position() const
        {
            return empty() ? start_position() : (frames.size() - 1);
        }

        const value_type & frame(size_type i) const
        {
            return frames.at(i).value;
        }

        value_type & frame(size_type i)
        {
            return frames.at(i).value;
        }

        bool empty() const
        {
            return frames.empty();
        }

        template <class SamplerT>
        void sample(position_type & position, time_type time_from_start, SamplerT & sampler) const
        {
            double amount;
            calculate(position, time_from_start, amount);
            sampler(frames[position].value, frames[position + 1].value, amount);
        }

        template <class DestPoseT, class WeightIterator, class TweenerT>
        void contribute_sample(position_type & position, time_type time_from_start, DestPoseT & destination, WeightIterator weights, double weight, TweenerT & tween) const
        {
            double amount;
            calculate(position, time_from_start, amount);
            tween(destination, frames[position].value, frames[position + 1].value, amount, weights, weight);
        }
 
    private:
        void calculate(position_type & position, time_type time_from_start, double & amount) const
        {
            validate(position, time_from_start);
            time_type time = frames[0].time_stamp + time_from_start;
            adjust_position(position, time);

            const keyframe & first  = frames[position];
            const keyframe & second = frames[position + 1];

            time_type time_range = second.time_stamp - first.time_stamp;
            time_type time_delta = time - first.time_stamp;
            amount = static_cast<double>(time_delta) / time_range;
        }

        void validate(position_type & position, time_type time_from_start) const
        {
            if (position < start_position() || position >= end_position())
            {
                throw std::out_of_range("position is out of range.");
            }
            if (time_from_start < 0 || time_from_start > duration())
            {
                throw std::out_of_range("time is out of range.");
            }
        }

        void adjust_position(position_type & position, time_type time) const
        {
            size_type stop = frames.size() - 1;
            while (position > 0 && time < frames[position].time_stamp) // rewind
            {
                --position;
            }
            while (position+1 < stop && time > frames[position+1].time_stamp) // advance
            {
                ++position;
            }
        }

        typename frame_list::iterator find_insertion_point(time_type time)
        {
            typename frame_list::reverse_iterator pos = frames.rbegin();
            while (pos != frames.rend() && time < pos->time_stamp)
            {
                ++pos;
            }
            return pos.base();
        }

        time_type next_timestamp(time_type after)
        {
            return after + (empty() ? 0 : frames.back().time_stamp);
        }

        frame_list frames;
    };
} // namespace anim
} // namespace loci

#endif // LOCI_ANIM_TRACK_H_