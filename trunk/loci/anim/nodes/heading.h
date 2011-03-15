#ifndef LOCI_ANIM_NODES_HEADING_H_
#define LOCI_ANIM_NODES_HEADING_H_

/**
 * Defines a generator node that modifies the heading of an animation.
 * This node modifies the time parameter to change the tempo.
 *
 * @file   heading.h
 * @author David Gill
 * @date   08/04/2010
 */

#include <boost/shared_ptr.hpp>

#include "loci/pose.h"
#include "loci/anim/keyed_motion.h"
#include "loci/anim/sequence_node.h"
#include "loci/anim/blend_info.h"
#include "loci/anim/nodes/clip.h"
#include "loci/numeric/convert/axisangle_quat.h"
#include "loci/numeric/transform/by_quat.h"
#include "loci/numeric/quat.h"
#include "loci/numeric/parametric.h"

#include <iostream>

namespace loci {
namespace anim
{ 
    class fix_heading : public sequence_node
    {
    public:
        fix_heading() { }

        fix_heading(const seq_node_ptr & node, double angle)
        {
            anim(node, angle);
        }

        void anim(const seq_node_ptr & node, double angle)
        {
            this->node = connect(node);
            angle_ = numeric::convert::quat_from_axisangle<float>(0.0, 1.0, 0.0, angle);
        }

    private:
        void master_sync_properties(const time_info & sync_info, const blend_info & blending)
        {
            find_sync_properties(node, sync_info, blending);
        }

        void evaluate(const time_info & timing, const blend_info & info)
        {
            if (!node.connected()) { return; }

            animate(node, timing, info);

            bone_pose::displacement_type & root = info.destination().root().tip_offset;
            numeric::vector3f dir(root.x(), 0, root.z());
            if (numeric::approx_zero(dir)) { dir = forward; }

            dir = numeric::transform::rotated_vector(dir, info.rotation_delta());
            numeric::quatf yrot = angle_ * numeric::transform::quat_between(dir, forward);
            
            root = numeric::transform::rotated_vector(root, yrot);
            
            info.destination().root().rotation = (yrot) * info.destination().root().rotation;

            info.rotation_delta().reset(0, 0, 0, 0);
            float root_weight = info.amount() * info.weights().front();
            numeric::alerp(info.rotation_delta(), numeric::quatf(1, 0, 0, 0), root_weight, 0.3f);
        }

        void contract_children(contractor & contract) const
        {
            contract.imbue(node);
        }

        time_type calculate_duration() const
        {
            return node->duration();
        }

        numeric::quatf angle_;
        seq_node_slot node;

        static const numeric::vector3f forward;
    };


    class follow_heading : public sequence_node
    {
    public:
        follow_heading() : previous(0), previous_rot(1, 0, 0, 0) { }

        follow_heading(const seq_node_ptr & node)
            : node(connect(node))
            , previous(0)
            , previous_rot(1, 0, 0, 0)
        {
        }

        void anim(const seq_node_ptr & node)
        {
            this->node = connect(node);
        }

    private:
        void master_sync_properties(const time_info & sync_info, const blend_info & blending)
        {
            find_sync_properties(node, sync_info, blending);
        }

        void evaluate(const time_info & timing, const blend_info & info)
        {
            if (!node.connected()) { return; }

            bone_pose::displacement_type & root = info.destination().root().tip_offset;
            bone_pose::displacement_type prev_root = root;
            numeric::quatf prev_rot_delta = info.rotation_delta();

            animate(node, timing, info);
            float root_weight = info.amount() * info.weights().front();

            numeric::vector3f dir(root.x() - prev_root.x(), 0.0f, root.z() - prev_root.z());
            numeric::vector3f heading(numeric::xyz_from_xz(info.direction()));
            if (numeric::approx_zero(previous)) { previous = heading; }
            if (dir.length_squared() < 0.005f) { dir = previous; }
            previous = dir;
            
            dir = numeric::transform::rotated_vector(dir, info.rotation_delta());

            numeric::quatf yrot = numeric::transform::quat_between(dir, heading);

            info.rotation_delta() = prev_rot_delta;
            numeric::alerp(info.rotation_delta(), numeric::invert(previous_rot) * yrot, root_weight, 0.3f);

            previous_rot = yrot;
        }

        void tick(time_type time_delta, bool reset_flag)
        {
            if (reset_flag)
            {
                previous.reset(0, 0, 0);
                previous_rot.reset(1, 0, 0, 0);
            }
        }

        void contract_children(contractor & contract) const
        {
            contract.imbue(node);
        }

        time_type calculate_duration() const
        {
            return node->duration();
        }

        seq_node_slot node;
        numeric::vector3f previous;
        numeric::quatf    previous_rot;
    };
} // namespace anim
} // namespace loci

#endif // LOCI_ANIM_NODES_HEADING_H_