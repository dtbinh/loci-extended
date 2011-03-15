#ifndef LOCI_VIDEO_MOCAP_H_
#define LOCI_VIDEO_MOCAP_H_

/**
 * An immutable construct representing a motion capture.
 * Composes the various structures together to represent a complete mocap,
 * this class is entirely immutable and has no functional properties to it.
 *
 * @file   mocap.h
 * @author David Gill
 * @date   17/02/2010
 */

#include <memory>

#include <boost/shared_ptr.hpp>

#include "loci/skeleton.h"

namespace loci
{
    // forward declarations
    class named_indicies;
    class hierarchy;
    class pose;

namespace mocaps
{
    template <class MotionT>
    class mocap
    {
    public:
        typedef MotionT motion_type;

        mocap(
            std::auto_ptr<hierarchy> connectivity,
            std::auto_ptr<named_indices> names,
            std::auto_ptr<pose> rest_pose,
            std::auto_ptr<motion_type> captured_motion)
                : figure_(connectivity, names, rest_pose)
                , motion_(captured_motion)
        {
        }

        mocap(
            const skeleton & skeletal_figure,
            std::auto_ptr<motion_type> captured_motion)
                : figure_(skeletal_figure)
                , motion_(captured_motion)
        {
        }

        boost::shared_ptr<const hierarchy> structure() const
        {
            return figure_.structure();
        }

        boost::shared_ptr<const named_indices> name_bindings() const
        {
            return figure_.name_bindings();
        }

        boost::shared_ptr<const pose> bind_pose() const
        {
            return figure_.bind_pose();
        }

        skeleton figure() const
        {
            return figure_;
        }

        boost::shared_ptr<const motion_type> motion() const
        {
            return motion_;
        }

    private:
        skeleton figure_;
        boost::shared_ptr<const motion_type> motion_;
    };
} // namespace mocaps
} // namespace loci

#endif // LOCI_VIDEO_MOCAP_H_