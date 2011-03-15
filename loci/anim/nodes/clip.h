#ifndef LOCI_ANIM_NODES_CLIP_H_
#define LOCI_ANIM_NODES_CLIP_H_

/**
 * Defines a sequence node node that samples keyframed motion.
 * This node essentially adapts the motion interface to that of the anim-tree.
 *
 * @file   clip.h
 * @author David Gill
 * @date   30/03/2010
 */

#include "loci/pose.h"
#include "loci/anim/sequence_node.h"
#include "loci/anim/blend_info.h"
#include "loci/anim/time_info.h"
#include "loci/anim/keyed_motion.h"
#include "loci/numeric/transform/by_quat.h"

namespace loci {
namespace anim
{ 
    class clip : public sequence_leaf_node
    {
    public:
        typedef keyed_motion<pose> motion_type;

        clip(const boost::shared_ptr<const motion_type> & motion,
             const boost::shared_ptr<const sync_warp> & syncher = boost::shared_ptr<const sync_warp>(),
             float y = 0
#ifdef LOCI_DEBUGGING
            , float r = 1.0f, float g = 1.0f, float b = 0.0
#endif
            ) : motion(motion)
              , syncher(syncher)
              , position(motion->start_position())
              , y(y)
              , resetting(true)
              , sync_time(0)
              , sync_pos(0)
#ifdef LOCI_DEBUGGING
              , r(r), g(g), b(b)
#endif
        {
        }

#ifdef LOCI_DEBUGGING
        void debug_colour(float r, float g, float b)
        {
            this->r = r;
            this->g = g;
            this->b = b;
        }
#endif

    private:
        typedef bone_pose::displacement_type offset_t;
        typedef bone_pose::rotation_type rotation_t;

        void master_sync_properties(const time_info & sync_info, const blend_info &)
        {
            if (!syncher) { return; }
            syncher->master_sync_settings(sync_info);
        }

        void evaluate(const time_info & timing, const blend_info & info)
        {
            time_type time = timing.time();
            if (syncher)
            {
                sync_time = syncher->sync(timing, sync_time, sync_pos);
                time = sync_time;
            }

            //time_type time = timing.time();
            if (time > motion->duration())   { time = motion->duration(); resetting = true; }
            else if (time < 0)               { time = 0; }
            if (time == timing.time_delta()) { resetting = true; }

            
#ifdef LOCI_DEBUGGING
            contribute_pose_lerp<blend_info::weight_list::const_iterator> tweener(r, g, b);
#else
            contribute_pose_lerp<blend_info::weight_list::const_iterator> tweener;
#endif

            motion->contribute_sample(
                position, time,
                info.destination(),
                info.weights().begin(), info.amount(),
                tweener);

            float root_weight = info.amount() * info.weights().front();
            
            using namespace detail;

            const numeric::vector2f heading_dir =
                numeric::transform::rotated_xz_heading(forward, 10.0f, tweener.root_rotation);

            numeric::quatf yrot = numeric::transform::yrot_quat_between(heading_dir, forward);

            if (resetting)
            {
                update_cached(tweener.root_tip_offset, yrot);
                resetting = false;
            }

            offset_t delta_pos = tweener.root_tip_offset - reference;
            delta_pos = numeric::transform::rotated_vector(delta_pos, yrot);

            numeric::alerp(info.rotation_delta(), numeric::invert_unit(yrot) * heading_rot, root_weight, 0.3f);
            bone_pose::contribute(info.destination().root(), delta_pos, yrot * tweener.root_rotation, root_weight);

            if (timing.time_delta() != 0)
            {
                update_cached(tweener.root_tip_offset, yrot);
            }
        }

        void tick(time_type time_delta, bool reset_flag)
        {
            resetting |= reset_flag;
            if (resetting)
            {
                sync_time = total_time();
                sync_pos = 0;
            }
        }

        time_type calculate_duration() const
        {
            return motion->duration();
        }

        void update_cached(const offset_t & root_offset, const rotation_t & inv_heading)
        {
            reference = root_offset;
            reference.y(-y);
            heading_rot = inv_heading;
        }

        boost::shared_ptr<const motion_type> motion;
        boost::shared_ptr<const sync_warp> syncher;

        motion_type::position_type position;
        float y;
        offset_t reference;
        bool resetting;

        rotation_t heading_rot;

        unsigned int sync_pos;
        time_type sync_time;

#ifdef LOCI_DEBUGGING
        float r, g, b;
#endif

        static const numeric::vector2f forward;
    };

    
} // namespace anim
} // namespace loci

#endif // LOCI_ANIM_NODES_CLIP_H_