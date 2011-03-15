#ifndef LOCI_SKELETON_H_
#define LOCI_SKELETON_H_

/**
 * An immutable construct representing a skeletal structure.
 * Composes the various structures together to represent a complete skeleton,
 * this class is entirely immutable and has no functional properties to it.
 *
 * @file   skeleton.h
 * @author David Gill
 * @date   26/01/2010
 */

#include <memory>

#include <boost/shared_ptr.hpp>

#include "loci/pose.h"
#include "loci/hierarchy.h"
#include "loci/named_indices.h"

namespace loci
{
    class skeleton
    {
    public:
        skeleton(
            std::auto_ptr<hierarchy> connectivity,
            std::auto_ptr<named_indices> names,
            std::auto_ptr<pose> rest_pose)
                : connectivity(connectivity)
                , names(names)
                , rest_pose(rest_pose)
        {
        }

        skeleton(const skeleton & s, boost::shared_ptr<const pose> p)
            : connectivity(s.connectivity)
            , names(s.names)
            , rest_pose(p)
        {
        }

        boost::shared_ptr<const hierarchy> structure() const
        {
            return connectivity;
        }

        boost::shared_ptr<const named_indices> name_bindings() const
        {
            return names;
        }

        boost::shared_ptr<const pose> bind_pose() const
        {
            return rest_pose;
        }

    private:
        boost::shared_ptr<const hierarchy>     connectivity;
        boost::shared_ptr<const named_indices> names;
        boost::shared_ptr<const pose>          rest_pose;
    };
} // namespace loci

#endif // LOCI_SKELETON_H_