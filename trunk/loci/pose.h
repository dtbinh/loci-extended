#ifndef LOCI_POSE_H_
#define LOCI_POSE_H_

/**
 * Defines data structure for storing pose data
 * A pose is a snapshot of the time-dependant data that describes a skeleton.
 *
 * @file   pose.h
 * @author David Gill
 * @date   13/01/2010
 */

#include <vector>

#include "loci/numeric/vector.h"
#include "loci/numeric/quat.h"
#include "loci/numeric/parametric.h"

namespace loci
{
    class bone_pose
    {
    public:
        typedef loci::numeric::quatf rotation_type;
        typedef loci::numeric::vector3f displacement_type;

        rotation_type rotation;
        displacement_type tip_offset;
        
#ifdef LOCI_DEBUGGING
        float r, g, b;
        bone_pose() : rotation(0, 0, 0, 0), tip_offset(0, 0, 0), r(0), g(0), b(0) { }
#else
        bone_pose() : rotation(0, 0, 0, 0), tip_offset(0, 0, 0) { }
#endif

        
        bone_pose(const rotation_type & rotation, displacement_type tip_offset)
            : rotation(rotation)
            , tip_offset(tip_offset)
        {
        }

        bone_pose(const rotation_type & rotation, displacement_type::value_type length)
            : rotation(rotation)
            , tip_offset(0, length, 0)
        {
        }

        static void contribute(bone_pose & dest, const displacement_type & disp, const rotation_type & rot, double weight)
        {
            using namespace loci::numeric;
            if (weight == 0) { return; }
            dest.tip_offset += (disp * weight);
            alerp(dest.rotation, rot, weight, 0.1f);
        }

#ifdef LOCI_DEBUGGING
        static void contribute(bone_pose & dest, const displacement_type & disp, const rotation_type & rot, double weight, float r, float g, float b)
        {
            contribute(dest, disp, rot, weight);

            dest.r += r * weight;
            dest.g += g * weight;
            dest.b += b * weight;
        }
#endif
    };

    class pose
    {
    public:
        typedef std::vector<bone_pose> bone_list;
        
        typedef bone_list::iterator       bone_iterator;
        typedef bone_list::const_iterator const_bone_iterator;

        typedef bone_list::size_type size_type;

        pose() { }

        pose(size_type num_bones)
            : bones_(num_bones)
        {
        }

        template <class ForwardIterator>
        pose(ForwardIterator first, ForwardIterator last, size_type expected = 20)
        {
            bones_.reserve(expected);
            bones_.insert(bones_.end(), first, last);
        }

        pose(std::auto_ptr<bone_list> bs)
        {
            bones_.swap(*bs);
        }

        void swap(pose & other)
        {
            bones_.swap(other.bones_);
        }

        void reset()
        {
            for (bone_list::iterator iter = bones_.begin();
                 iter != bones_.end();
                 ++iter)
            {
                *iter = bone_pose();
            }
        }

        bone_pose &       operator[](std::size_t i)       { return bones_[i]; }
        const bone_pose & operator[](std::size_t i) const { return bones_[i]; }

        bone_iterator       begin()       { return bones_.begin(); }
        bone_iterator       end()         { return bones_.end();   }
        const_bone_iterator begin() const { return bones_.begin(); }
        const_bone_iterator end()   const { return bones_.end();   }

        bone_pose &       root()       { return bones_[0]; }
        const bone_pose & root() const { return bones_[0]; }

        size_type bone_count() const { return bones_.size(); }

    private:
        bone_list bones_;
    };
    
    template <class WeightIterator>
    struct contribute_pose_lerp
    {
        bone_pose::displacement_type root_tip_offset;
        bone_pose::rotation_type root_rotation;

#ifdef LOCI_DEBUGGING
        float r, g, b;

        contribute_pose_lerp(float r, float g, float b) : r(r), g(g), b(b) { }
#endif

        void operator()(pose & destination, const pose & from, const pose & to, double amount, WeightIterator weights, double weight)
        {
            typedef pose::bone_iterator dst_bone_iter;
            typedef pose::const_bone_iterator src_bone_iter;

            dst_bone_iter dest_bone = destination.begin();
            src_bone_iter from_bone = from.begin();
            src_bone_iter to_bone   = to.begin();

            root_tip_offset = numeric::linear(from_bone->tip_offset, to_bone->tip_offset, amount);
            root_rotation   = numeric::nlerp(from_bone->rotation, to_bone->rotation, amount);
#ifdef LOCI_DEBUGGING
            dest_bone->r = 0;
            dest_bone->g = 0;
            dest_bone->b = 0;
#endif

            ++dest_bone; ++from_bone; ++to_bone; ++weights;
            for (; dest_bone != destination.end(); ++dest_bone, ++from_bone, ++to_bone, ++weights)
            {
#ifndef LOCI_DEBUGGING
                bone_pose::contribute(*dest_bone,
                                      numeric::linear(from_bone->tip_offset, to_bone->tip_offset, amount),
                                      numeric::nlerp(from_bone->rotation, to_bone->rotation, amount),
                                      *weights * weight);
#else
                bone_pose::contribute(*dest_bone,
                                      numeric::linear(from_bone->tip_offset, to_bone->tip_offset, amount),
                                      numeric::nlerp(from_bone->rotation, to_bone->rotation, amount),
                                      *weights * weight,
                                      r, g, b);
#endif
            }
        }
    };

    class root_motion_sampler
    {
    public:
        bone_pose::displacement_type root_tip_offset;
        bone_pose::rotation_type root_rotation;

        void operator()(const pose & from, const pose & to, double amount)
        {
            root_tip_offset = numeric::linear(from.root().tip_offset, to.root().tip_offset, amount);
            root_rotation   = numeric::nlerp(from.root().rotation, to.root().rotation, amount);
        }
    };
} // namespace loci

#endif // LOCI_POSE_H_