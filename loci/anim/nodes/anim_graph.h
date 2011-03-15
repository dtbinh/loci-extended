#ifndef LOCI_ANIM_NODES_ANIM_GRAPH_H_
#define LOCI_ANIM_NODES_ANIM_GRAPH_H_

/**
 * Defines a root node for the animation graph.
 * Acts as the root of the animation graph.
 *
 * @file   anim_graph.h
 * @author David Gill
 * @date   30/03/2010
 */

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "loci/anim/blend_info.h"
#include "loci/anim/blend_node.h"
#include "loci/pose.h"
#include "loci/skeleton.h"
#include "loci/numeric/maths.h"
#include "loci/numeric/convert/axisangle_quat.h"
#include "loci/numeric/transform/by_quat.h"

namespace loci {
namespace anim
{ 
    class anim_graph : private blend_contract_node
    {
    private:
        typedef bone_pose::displacement_type offset_t;
        typedef bone_pose::rotation_type rotation_t;

    public:
        typedef blend_info::direction_type direction_type;

        anim_graph(const blend_node_ptr & root_child, const loci::skeleton & figure,
                   const numeric::vector2f & initial_position = numeric::vector2f(0.0f),
                   double speed = 1.0, const direction_type & direction = direction_type(0, 1))
            : figure_(figure)
            , buffer(boost::make_shared<pose>(figure.bind_pose()->bone_count()))
            , initial_position(initial_position)
            , accum_position(initial_position.x(), 0, initial_position.y())
            , accum_rotation(1, 0, 0, 0)
            , speed_(speed)
            , direction_(direction)
            , weights(buffer->bone_count(), 1.0)
        {
            node = connect(root_child);
        }

        double speed() const { return speed_; }
        void speed(double s) { speed_ = s; }
        direction_type direction() const { return direction_; }
        void direction(const direction_type & d) { direction_ = d; }

        boost::shared_ptr<const pose> animate(time_type time_delta)
        {
            buffer->reset();
            rotation_t rotational_delta(0, 0, 0, 0);
            double sync_position = -1;
            time_info::sync_group sync_stage = 0;
            blend_node::animate(time_info(total_time() + time_delta,
                                          time_delta,
                                          sync_position, sync_stage),
                                blend_info(1.0, weights, *buffer, figure_,
                                           speed_, direction_, rotational_delta));

            // accumulate the absolute rotation
            accum_rotation = rotational_delta.normalise() * accum_rotation;
            buffer->root().rotation = accum_rotation * buffer->root().rotation;

            // accumulate the absolute position
            accum_position.y(0); // y is already absolute
            accum_position += numeric::transform::rotated_vector(buffer->root().tip_offset, accum_rotation);
            buffer->root().tip_offset = accum_position;

            return buffer;
        }

        time_type current_time() const { return total_time(); }

        skeleton active_figure() const { return skeleton(figure_, buffer); }

        skeleton figure() const { return figure_; }

        void reset()
        {
            reset_time();
            accum_position.reset(initial_position.x(), 0, initial_position.y());
            accum_rotation.reset(1, 0, 0, 0);
            animate(0);
        }

    private:
        void contract_children(contractor & contract) const
        {
            contract.imbue(node);
        }

        void master_sync_properties(const time_info & sync_info, const blend_info & blending)
        {
            find_sync_properties(node, sync_info, blending);
        }

        void evaluate(const time_info & timing, const blend_info & info)
        {
            blend_node::animate(node, timing, info);
            
            // normalise all quaternions
            for (pose::bone_iterator bone = buffer->begin();
                 bone != buffer->end();
                 ++bone)
            {
                bone->rotation.normalise();
            }
        }

        blend_node_slot node;
        skeleton figure_;
        numeric::vector2f initial_position;
        boost::shared_ptr<pose> buffer;
        offset_t accum_position;
        rotation_t accum_rotation;
        double speed_;
        direction_type direction_;
        blend_info::weight_list weights;
    };
} // namespace anim
} // namespace loci

#endif // LOCI_ANIM_NODES_ANIM_GRAPH_H_