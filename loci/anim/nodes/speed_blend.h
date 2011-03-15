#ifndef LOCI_ANIM_NODES_SPEED_BLEND_H_
#define LOCI_ANIM_NODES_SPEED_BLEND_H_

/**
 * Defines a generator node that modifies the pace of animation.
 * This node modifies the time parameter to change the tempo.
 *
 * @file   tempo.h
 * @author David Gill
 * @date   27/02/2010
 */

#include <boost/shared_ptr.hpp>

#include "loci/anim/blend_node.h"
#include "loci/anim/blend_info.h"
#include "loci/numeric/parametric.h"

namespace loci {
namespace anim
{ 
    double inline cap_speed(double speed, double min, double max)
    {
        return speed < min ? min : (speed > max ? max : speed);
    }

    class speed_transition : public blend_contract_node
    {
    public:
        speed_transition()
        {
        }

        speed_transition(const blend_node_ptr & slow, double slow_target_speed, double rate_when_fast,
                         const blend_node_ptr & fast, double fast_target_speed, double rate_when_slow)
            : slow(connect(slow))
            , fast(connect(fast))
            , slow_speed(slow_target_speed)
            , fast_speed(fast_target_speed)
            , rate_when_slow(rate_when_slow)
            , rate_when_fast(rate_when_fast)
        {
        }

        void slow_anim(const blend_node_ptr & node, double target_speed, double rate_when_fast)
        {
            slow = connect(node);
            slow_speed = target_speed;
            this->rate_when_fast = rate_when_fast;
        }

        void fast_anim(const blend_node_ptr & node, double target_speed, double rate_when_slow)
        {
            fast = connect(node);
            fast_speed = target_speed;
            this->rate_when_slow = rate_when_slow;
        }

    private:
        void master_sync_properties(const time_info & sync_info, const blend_info & blending)
        {
            double speed = cap_speed(blending.speed(), slow_speed, fast_speed);
            double fast_weight = numeric::linear_weight(slow_speed, fast_speed, speed);

            if (fast_weight < 0.5)
            {
                find_sync_properties(slow, sync_info, blending);
            }
            else
            {
                find_sync_properties(fast, sync_info, blending);
            }
        }

        void evaluate(const time_info & timing, const blend_info & info)
        {
            if (!slow.connected() || !fast.connected()) { return; }

            double speed = cap_speed(info.speed(), slow_speed, fast_speed);
            double fast_weight = numeric::linear_weight(slow_speed, fast_speed, speed);
            double slow_pace = numeric::linear_scale(slow_speed, fast_speed, speed, 1.0, rate_when_fast);
            double fast_pace = numeric::linear_scale(slow_speed, fast_speed, speed, rate_when_slow, 1.0);

            slow->animate(
                time_info(
                    static_cast<time_type>(total_time() * slow_pace),
                    static_cast<time_type>(timing.time_delta() * slow_pace),
                    timing),
                blend_info(info.amount() * (1.0 - fast_weight), info));

            fast->animate(
                time_info(
                    static_cast<time_type>(total_time() * fast_pace),
                    static_cast<time_type>(timing.time_delta() * fast_pace),
                    timing),
                blend_info(info.amount() * fast_weight, info));
        }

        void contract_children(contractor & contract) const
        {
            contract.imbue(slow);
            contract.imbue(fast);
        }

        double slow_speed;
        double fast_speed;
        double rate_when_slow;
        double rate_when_fast;
        blend_node_slot slow;
        blend_node_slot fast;
    };
} // namespace anim
} // namespace loci

#endif // LOCI_ANIM_NODES_SPEED_BLEND_H_