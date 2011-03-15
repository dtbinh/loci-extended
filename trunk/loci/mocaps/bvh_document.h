#ifndef LOCI_MOCAPS_BVH_DOCUMENT_H_
#define LOCI_MOCAPS_BVH_DOCUMENT_H_

/**
 * Defines a bvh_document for parsing the BioVision Hierarchy format.
 * Parses the BioVision Hierarchy mocap format from a string.
 *
 * @file   bvh_document.cpp
 * @author David Gill
 * @date   21/02/2010
 */

#include <string>
#include <istream>

#include "loci/skeleton.h"
#include "loci/skeleton_builder.h"
#include "loci/mocaps/mocap.h"
#include "loci/mocaps/detail/bvh/parser.h"

namespace loci
{
    // forward declarations
    class pose;
    class named_indices;
    namespace anim
    {
        template <class T>
        class keyed_motion;
    }

namespace mocaps
{
    class bvh_document
    {
    public:
        typedef mocap< anim::keyed_motion<pose> > mocap;

        explicit bvh_document(const skeleton & figure, double frame_time, double expected_duration = 3000);
        explicit bvh_document(std::istream & source);
        explicit bvh_document(const std::string & content);

        mocap compile(bool first_frame_as_bind_pose = false) const;
        void save(std::ostream & sink) const;

        void append_frame(const pose & p);

    private:
        typedef skeleton_builder::cursor cursor;
        typedef detail::bvh::scope_list scope_list;
        typedef scope_list::const_iterator scope_iterator;
        typedef mocap::motion_type motion_type;
        typedef detail::bvh::scope::motion_channel_list::const_iterator motion_iterator;

        void parse(std::istream & source);

        skeleton compile_skeleton() const;
        std::auto_ptr<motion_type> compile_motion(bool first_frame_as_bind_pose) const;

        void compile_skeleton_recurse(skeleton_builder & builder, cursor parent_joint, scope_iterator & pos, const numeric::vector3f & up) const;
        void compile_bone_motion_recurse(pose::bone_list & bones, scope_iterator & pos, unsigned int frame) const;

        void build_hierarchy_part(const named_indices & name_bindings, const pose & bind_pose, cursor c, unsigned int depth, unsigned int motion_reserve);

        scope_list scopes;
        unsigned int frame_count;
        double sample_rate;
    };

    bvh_document::mocap import_bvh_file(const std::string & path, bool first_frame_as_bind_pose = false);
    void export_bvh_to_file(const bvh_document & bvh_doc, const std::string & path);
} // namespace mocaps
} // namespace loci

#endif // LOCI_MOCAPS_BVH_DOCUMENT_H_