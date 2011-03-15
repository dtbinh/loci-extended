#ifndef LOCI_SKELETON_BUILDER_H_
#define LOCI_SKELETON_BUILDER_H_

/**
 * short
 * long
 *
 * @file   skeleton_builder.h
 * @author David Gill
 * @date   27/01/2010
 */

#include <deque>
#include <memory>

#include "loci/hierarchy.h"
#include "loci/named_indices.h"
#include "loci/pose.h"
#include "loci/skeleton.h"

namespace loci
{
    class skeleton_builder
    {
    public:
        typedef hierarchy::cursor cursor;

        skeleton_builder()
            : bones(1)
        {
        }

        skeleton_builder(const std::string & name, const bone_pose & root_pose)
            : bones(1)
        {
            bones[root().position()] = root_pose;
            names.insert(name, root().position());
        }

        cursor root() const
        {
            return connectivity.root();
        }

        void reset_root(const std::string & name, const bone_pose & root_pose)
        {
            bones[root().position()] = root_pose;
            if (!names.rename(root().position(), name))
            {
                names.insert(name, root().position());
            }
        }

        cursor add_bone(const cursor & parent,
                        const std::string & name,
                        const bone_pose & bones_pose)
        {
            if (!parent.valid())
            {
                throw std::out_of_range("parent cursor is invalid");
            }

            hierarchy::size_type bone_idx = connectivity.size();
            if (!names.insert(name, bone_idx).second)
            {
                throw std::invalid_argument("bone name is not unique: " + name);
            }

            bones.push_back(bones_pose);
            return connectivity.attach_to(parent);
        }


        std::auto_ptr<named_indices> bone_names() const
        {
            return std::auto_ptr<named_indices>( new named_indices(names) );
        }

        std::auto_ptr<hierarchy> structure() const
        {
            return std::auto_ptr<hierarchy>( new hierarchy(connectivity) );
        }

        std::auto_ptr<pose> bind_pose() const
        {
            return std::auto_ptr<pose>( 
                new pose(bones.begin(), bones.end(), bones.size()) );
        }

        skeleton build() const
        {
            return skeleton(structure(), bone_names(), bind_pose());
        }

    private:
        named_indices names;
        hierarchy connectivity;
        std::deque<bone_pose> bones;
    };
} // namespace loci

#endif // LOCI_SKELETON_BUILDER_H_