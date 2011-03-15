#ifndef LOCI_CONTROL_ACTOR_CONTROLLER_H_
#define LOCI_CONTROL_ACTOR_CONTROLLER_H_

/**
 * The base class for all locomotion nodes in an anim tree.
 * Defines a common interface for all locomotion tree nodes that control the
 * overall character movement.
 *
 * @file   loco_node.h
 * @author David Gill
 * @date   10/04/2010
 */

#include "loci/numeric/maths.h"
#include "loci/numeric/convert/angles.h"
#include "loci/numeric/parametric.h"

#include "loci/anim/keyed_motion.h"
#include "loci/anim/nodes/anim_graph.h"

namespace loci {
namespace control
{
    class actor_controller
    {
    public:
        typedef double time_type;

        actor_controller() { }
        actor_controller(const boost::shared_ptr<anim::anim_graph> & g) : g(g) { }

        void update(time_type time_delta)
        {
            return calculate(time_delta);
        }

        void reset()
        {
            reset_state();
        }

        time_type total_time() const { return g->current_time(); }

        boost::shared_ptr<anim::anim_graph> animation() const { return g; }
        void animation(const boost::shared_ptr<anim::anim_graph> & g) { this->g = g; }

    protected:
        boost::shared_ptr<anim::anim_graph> g;

    private:
        virtual void calculate(time_type time_delta) = 0;
        virtual void reset_state() = 0;
    };

    class keyed_actor_controller : public actor_controller
    {
    public:
        keyed_actor_controller(double speed = 0, double angle = 0)
            : speed_pos(0)
            , angle_pos(0)
        {
            speed_curve.insert(0, speed);
            angle_curve.insert(0, numeric::convert::to_rads(angle));
        }

        void append_speed(double speed, time_type time)
        {
            speed_curve.append(time, speed);
        }

        void append_heading(double angle, time_type time)
        {
            angle_curve.append(time, numeric::convert::to_rads(angle));
        }

    private:
        void calculate(time_type time_delta)
        {
            time_type speed_time = animation()->current_time() + time_delta;
            time_type angle_time = animation()->current_time() + time_delta;
            speed_time = speed_time < 0 ? 0 : speed_time;
            angle_time = angle_time < 0 ? 0 : angle_time;
            speed_time = speed_time > speed_curve.duration() ? speed_curve.duration() : speed_time;
            angle_time = angle_time > angle_curve.duration() ? angle_curve.duration() : angle_time;
            
            sampler speed, angle;
            speed_curve.sample(speed_pos, speed_time, speed);
            angle_curve.sample(angle_pos, angle_time, angle);

            double s = numeric::sin(angle.value);
            double c = numeric::cos(angle.value);

            animation()->speed(speed.value);
            animation()->direction(anim::anim_graph::direction_type(-s, c));
            animation()->animate(time_delta);
        }

        void reset_state()
        {
            animation()->reset();
            speed_pos = 0;
            angle_pos = 0;
        }

    private:
        class sampler
        {
        public:
            double value;
            
            void operator()(double from, double to, double amount)
            {
                value = numeric::linear(from, to, amount);
            }
        };
        
        typedef anim::keyed_motion<double> curve_t;
        curve_t speed_curve;
        curve_t angle_curve;
        curve_t::position_type speed_pos;
        curve_t::position_type angle_pos;
    };
} // namespace control
} // namespace loci

#endif // LOCI_CONTROL_ACTOR_CONTROLLER_H_