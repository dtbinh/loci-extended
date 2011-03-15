#ifndef LOCI_ANIM_NODES_ADDITIVE_H_
#define LOCI_ANIM_NODES_ADDITIVE_H_

/**
 * Defines a generator node that modifies the pace of animation.
 * This node modifies the time parameter to change the tempo.
 *
 * @file   tempo.h
 * @author David Gill
 * @date   27/02/2010
 */

#include <algorithm>

#include "loci/anim/sequence_node.h"
#include "loci/anim/blend_info.h"
#include "loci/numeric/quat.h"

namespace loci {
namespace anim
{ 
    class additive_combine : public blend_node
    {
    public:
        additive_combine() : weight(1.0) { }
        additive_combine(const blend_node_ptr & base,
                         const blend_node_ptr & layer,
                         double weight)
            : base(connect(base))
            , layer(connect(layer))
            , weight(weight)
        {
        }

        void base_anim(const blend_node_ptr & anim)
        {
            base = connect(anim);
        }

        void layered_anim(const blend_node_ptr & anim)
        {
            layer = connect(anim);
        }

        void layer_weight(double weight)
        {
            this->weight = weight;
        }

        void reset(const blend_node_ptr & base, const blend_node_ptr & layer, double weight)
        {
            this->base   = connect(base);
            this->layer  = connect(layer);
            this->weight = weight;
        }

    private:
        void master_sync_properties(const time_info & sync_info, const blend_info & blending)
        {
            find_sync_properties(base, sync_info, blending);
        }

        void evaluate(const time_info & timing, const blend_info & info)
        {
            animate(base, timing, info);
            animate(layer, timing, blend_info(info.amount() * weight, info));
        }

        void contract_children(contractor & contract) const
        {
            contract.imbue(base);
            contract.imbue(layer);
        }

        blend_node_slot base;
        blend_node_slot layer;
        double weight;
    };

    class additive_extract : public blend_node
    {
    public:
        additive_extract() { }
        additive_extract(const blend_node_ptr & base,
                         const blend_node_ptr & composite,
                         bool remove_root)
            : base(connect(base))
            , composite(connect(composite))
            , remove_root(remove_root)
        {
        }

        void base_anim(const blend_node_ptr & anim)
        {
            base = connect(anim);
        }

        void composite_anim(const blend_node_ptr & anim)
        {
            composite = connect(anim);
        }

    private:
        void master_sync_properties(const time_info & sync_info, const blend_info & blending)
        {
            find_sync_properties(composite, sync_info, blending);
        }

        void evaluate(const time_info & timing, const blend_info & info)
        {
            bone_pose::rotation_type prev_delta = info.rotation_delta();
            bone_pose prev_root = info.destination().root();


            animate(composite, timing, info);
            animate(base, timing, blend_info(-info.amount(), info));

            info.destination().root().tip_offset.x(prev_root.tip_offset.x());
            info.destination().root().tip_offset.z(prev_root.tip_offset.z());

            /*if (info.destination().bone_count() > weights.size())
            {
                base_buffer = pose(info.destination().bone_count());
                composite_buffer = pose(info.destination().bone_count());
                weights.resize(info.destination().bone_count(), 1.0);
            }

            base_buffer.reset();
            composite_buffer.reset();
            bone_pose::rotation_type base_rot_delta(0, 0, 0, 0);
            bone_pose::rotation_type composite_rot_delta(0, 0, 0, 0);
            animate(base, delta, blend_info(1.0, weights, base_buffer, base_rot_delta, info));
            animate(composite, delta, blend_info(1.0, weights, composite_buffer, composite_rot_delta, info));

            pose::const_bone_iterator base_iter = base_buffer.begin();
            pose::const_bone_iterator composite_iter = composite_buffer.begin();
            pose::bone_iterator dest_iter = info.destination().begin();
            blend_info::weight_list::const_iterator weight_iter = info.weights().begin();

            for (; base_iter != base_buffer.end(); ++base_iter, ++composite_iter, ++dest_iter, ++weight_iter)
            {
                bone_pose::contribute(*dest_iter,
                                      composite_iter->tip_offset - base_iter->tip_offset,
                                      numeric::invert_unit(base_iter->rotation) * composite_iter->rotation,
                                      info.amount() * (*weight_iter));
            }
            numeric::alerp(
                info.rotation_delta(),
                numeric::invert_unit_inplace(base_rot_delta) * composite_rot_delta,
                info.amount() * info.weights().front(),
                0.3f);*/
        }

        void contract_children(contractor & contract) const
        {
            contract.imbue(base);
            contract.imbue(composite);
        }

        blend_node_slot base;
        blend_node_slot composite;
        bool remove_root;

        pose base_buffer;
        pose composite_buffer;
        blend_info::weight_list weights;
    };
} // namespace anim
} // namespace loci

#endif // LOCI_ANIM_NODES_ADDITIVE_H_