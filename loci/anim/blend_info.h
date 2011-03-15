#ifndef LOCI_ANIM_BLEND_INFO_H_
#define LOCI_ANIM_BLEND_INFO_H_

/**
 * Describes the properties of a blend.
 * blend_info structures are propagated through the blend-tree and carry information
 * about how a node should perform a blend, such as which weights to use.
 *
 * @file   blend_info.h
 * @author David Gill
 * @date   29/03/2010
 */

#include <vector>

#include "loci/pose.h"
#include "loci/skeleton.h"
#include "loci/numeric/vector.h"

namespace loci {
namespace anim
{
    class blend_info
    {
    public:
        typedef std::vector<double> weight_list;
        typedef numeric::vector2f direction_type;

        blend_info(double amount,
                   weight_list & weights,
                   pose & destination,
                   skeleton & figure,
                   double speed,
                   const direction_type & direction,
                   bone_pose::rotation_type & rotation_delta)
            : amount_(amount)
            , weights_(&weights)
            , destination_(&destination)
            , figure_(&figure)
            , speed_(speed)
            , direction_(&direction)
            , rotation_delta_(&rotation_delta)
        {
        }

        blend_info(double amount, const blend_info & other)
            : amount_(amount)
            , weights_(other.weights_)
            , destination_(other.destination_)
            , figure_(other.figure_)
            , speed_(other.speed_)
            , direction_(other.direction_)
            , rotation_delta_(other.rotation_delta_)
        {
        }

        blend_info(weight_list & weights, const blend_info & other)
            : amount_(other.amount_)
            , weights_(&weights)
            , destination_(other.destination_)
            , figure_(other.figure_)
            , speed_(other.speed_)
            , direction_(other.direction_)
            , rotation_delta_(other.rotation_delta_)
        {
        }

        blend_info(double amount, weight_list & weights, const blend_info & other)
            : amount_(amount)
            , weights_(&weights)
            , destination_(other.destination_)
            , figure_(other.figure_)
            , speed_(other.speed_)
            , direction_(other.direction_)
            , rotation_delta_(other.rotation_delta_)
        {
        }

        blend_info(double amount, weight_list & weights, pose & destination, bone_pose::rotation_type & rotation_delta, const blend_info & other)
            : amount_(amount)
            , weights_(&weights)
            , destination_(&destination)
            , figure_(other.figure_)
            , speed_(other.speed_)
            , direction_(other.direction_)
            , rotation_delta_(&rotation_delta)
        {
        }

        double amount() const { return amount_; }
        const weight_list & weights() const { return *weights_; }
        pose & destination() const { return *destination_; }
        boost::shared_ptr<const hierarchy> structure() const { return figure_->structure(); }
        boost::shared_ptr<const named_indices> name_bindings() const { return figure_->name_bindings(); }
        const skeleton & figure() const { return *figure_; }
        double speed() const { return speed_; }
        const direction_type & direction() const { return *direction_; }
        bone_pose::rotation_type & rotation_delta() const { return *rotation_delta_; }

    private:
        double amount_;
        weight_list * weights_;
        pose * destination_;
        skeleton * figure_;
        double speed_;
        const direction_type * direction_;

        bone_pose::rotation_type * rotation_delta_;

    };
} // namespace anim
} // namespace loci

#endif // LOCI_ANIM_BLEND_INFO_H_