/**
 * Implementation of bvh_document.
 * Implements the bvh_document class.
 *
 * @file   bvh_document.cpp
 * @author David Gill
 * @date   21/02/2010
 */

#include <string>
#include <istream>
#include <sstream>
#include <fstream>
#include <iterator>
#include <iostream> // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#include "loci/mocaps/bvh_document.h"

#include "loci/pose.h"
#include "loci/named_indices.h"
#include "loci/hierarchy.h"
#include "loci/skeleton.h"
#include "loci/skeleton_builder.h"
#include "loci/anim/keyed_motion.h"

#include "loci/numeric/vector.h"
#include "loci/numeric/quat.h"
#include "loci/numeric/euler.h"
#include "loci/numeric/convert/axisangle_quat.h"
#include "loci/numeric/convert/euler_quat.h"
#include "loci/numeric/transform/by_quat.h"

#include "loci/mocaps/detail/bvh/scope.h"
#include "loci/mocaps/detail/bvh/parser.h"
#include "loci/mocaps/detail/bvh/save.h"

namespace loci {
namespace mocaps
{
    namespace // anonymous
    {
        void compile_channels(loci::numeric::vector3f & position, loci::numeric::quatf & rotation,
                              detail::bvh::scope_list::const_iterator pos, unsigned int frame)
        {
            using namespace loci::numeric;
            typedef detail::bvh::scope::channel_index_list::const_iterator channel_iterator;
            typedef detail::bvh::scope::motion_channel_list::const_iterator motion_iterator;
            typedef detail::bvh::channel_indices channel_indices;

            unsigned int euler_index = 0;
            static const unsigned int num_axis = 3;
            quatf euler_rotations[num_axis] =
            {
                quatf(1, 0, 0, 0), quatf(1, 0, 0, 0), quatf(1, 0, 0, 0)
            };
            position = pos->offset;

            channel_iterator channel_iter = pos->channels.begin();
            motion_iterator motion_iter   = pos->motion.begin() + pos->channels.size() * frame;
            while (channel_iter != pos->channels.end() &&
                   euler_index < num_axis)
            {
                if (*channel_iter == channel_indices::x_position)
                {
                    position.x(*motion_iter);
                }
                else if (*channel_iter == channel_indices::y_position)
                {
                    position.y(*motion_iter);
                }
                else if (*channel_iter == channel_indices::z_position)
                {
                    position.z(*motion_iter);
                }
                else if (*channel_iter == channel_indices::x_rotation)
                {
                    euler_rotations[euler_index++] =
                        convert::quat_from_axisangle(vector3f(1.0, 0.0, 0.0), *motion_iter);
                }
                else if (*channel_iter == channel_indices::y_rotation)
                {
                    euler_rotations[euler_index++] =
                        convert::quat_from_axisangle(vector3f(0.0, 1.0, 0.0), *motion_iter);
                }
                else if (*channel_iter == channel_indices::z_rotation)
                {
                    euler_rotations[euler_index++] =
                        convert::quat_from_axisangle(vector3f(0.0, 0.0, 1.0), *motion_iter);
                }

                ++channel_iter;
                ++motion_iter;
            }

            //position += pos->offset;
            rotation = euler_rotations[0] * euler_rotations[1] * euler_rotations[2];
        }
    } // anonymous namespace


    bvh_document::bvh_document(const skeleton & figure, double frame_time, double expected_duration)
        : sample_rate(1000.0 * frame_time)
        , frame_count(0)
    {
        scopes.reserve(figure.bind_pose()->bone_count());
        build_hierarchy_part(*figure.name_bindings(), *figure.bind_pose(), figure.structure()->root(), 0, static_cast<unsigned int>(expected_duration / frame_time + 1));
    }

    bvh_document::bvh_document(std::istream & source)
    {
        parse(source);
    }

    bvh_document::bvh_document(const std::string & content)
    {
        std::istringstream iss(content);
        parse(iss);
    }

    bvh_document::mocap bvh_document::compile(bool first_frame_as_bind_pose) const
    {
        skeleton s = compile_skeleton();
        std::auto_ptr<motion_type> m = compile_motion(first_frame_as_bind_pose);
        return mocap(s, m);
    }

    void bvh_document::save(std::ostream & sink) const
    {
        detail::bvh::save(sink, scopes, frame_count, static_cast<float>(sample_rate / 1000.0));
    }

    void bvh_document::append_frame(const pose & p)
    {
        scope_list::iterator scope_iter = scopes.begin();
        pose::const_bone_iterator bone_iter = p.begin();

        scope_iter->motion.push_back( bone_iter->tip_offset.x() );
        scope_iter->motion.push_back( bone_iter->tip_offset.y() );
        scope_iter->motion.push_back( bone_iter->tip_offset.z() );

        numeric::eulerf rot;
        for (; bone_iter != p.end() && scope_iter != scopes.end(); ++scope_iter, ++bone_iter)
        {
            rot = numeric::convert::to_euler_xyz(bone_iter->rotation);
            scope_iter->motion.push_back( rot.y() );
            scope_iter->motion.push_back( rot.z() );
            scope_iter->motion.push_back( rot.x() );
        }
        ++frame_count;
    }

    void bvh_document::parse(std::istream & source)
    {
        float sample_rate_secs;
        detail::bvh::parse(source, scopes, frame_count, sample_rate_secs);
        sample_rate = 1000.0 * static_cast<double>(sample_rate_secs);
    }

    skeleton bvh_document::compile_skeleton() const
    {
        typedef numeric::quatf rotation;

        skeleton_builder builder(
            scopes[0].name,
            bone_pose( rotation(1, 0, 0, 0), scopes[0].offset ));
        
        scope_list::const_iterator iter = scopes.begin();
        compile_skeleton_recurse(builder, builder.root(), iter, numeric::vector3f(0, 1, 0));
        
        return builder.build();
    }

    void bvh_document::compile_skeleton_recurse(skeleton_builder & builder, cursor parent, scope_iterator & pos, const numeric::vector3f & up) const
    {
        const std::string & parent_name = pos->name;

        const unsigned int depth = pos->depth + 1;
        
        unsigned int bone_id = 0;
        ++pos;
        while(pos != scopes.end() &&
              pos->depth == depth)
        {
            std::ostringstream bone_name;  
            bone_name << parent_name << " " << ++bone_id;

            numeric::vector3f direction = numeric::try_normalise(pos->offset, up);

            cursor joint = builder.add_bone(
                parent,
                bone_name.str(),
                bone_pose(
                    numeric::quatf(1, 0, 0, 0),
                    pos->offset));

            if ( pos->end_site() )
            {
                ++pos;
                continue;
            }

            compile_skeleton_recurse(builder, joint, pos, direction);
        }
    }

    std::auto_ptr<bvh_document::motion_type> bvh_document::compile_motion(bool first_frame_as_bind_pose) const
    {
        unsigned int frame_count_offset = first_frame_as_bind_pose ? 1 : 0;
        std::auto_ptr<motion_type> motion(new motion_type(frame_count - frame_count_offset));

        for (unsigned int frame = frame_count_offset; frame < frame_count; ++frame)
        {
            std::auto_ptr<pose::bone_list> bones(new pose::bone_list());
            bones->reserve(scopes.size());

            scope_iterator pos = scopes.begin();

            numeric::vector3f position;
            numeric::quatf    rotation;
            compile_channels(position, rotation, pos, frame);
            bones->push_back(bone_pose(rotation, position));

            compile_bone_motion_recurse(*bones, pos, frame);

            std::auto_ptr<pose> frame_pose( new pose(bones) );

            motion->insert(
                static_cast<motion_type::time_type>(frame) * sample_rate,
                frame_pose);
        }
        
        return motion;
    }

    void bvh_document::compile_bone_motion_recurse(pose::bone_list & bones, scope_iterator & pos, unsigned int frame) const
    {
        const unsigned int depth = pos->depth + 1;
        ++pos;

        while(pos != scopes.end() &&
              pos->depth == depth)
        {
            numeric::vector3f tip_offset;
            numeric::quatf rotation;
            compile_channels(tip_offset, rotation, pos, frame);

            bones.push_back( bone_pose(rotation, tip_offset) );

            if ( pos->end_site() )
            {
                ++pos;
                continue;
            }
            
            compile_bone_motion_recurse(bones, pos, frame);
        }
    }

    void bvh_document::build_hierarchy_part(const named_indices & name_bindings, const pose & bind_pose, cursor c, unsigned int depth, unsigned int motion_reserve)
    {
        if (depth > 0 && !scopes.back().end_site())
        {
            std::stringstream bone_name;  
            bone_name << name_bindings.name_of(c.position());
            bone_name >> scopes.back().name;
        }

        scopes.push_back(
            detail::bvh::scope(
                numeric::transform::rotated_vector(bind_pose[c.position()].tip_offset,
                                                   bind_pose[c.position()].rotation),
                depth,
                motion_reserve));

        c.to_children();

        if (c.valid()) // if it's a joint and not an end-site then add channels
        {
            // set the channel ordering: "[xpos ypos zpos] zrot xrot yrot"
            detail::bvh::scope & s = scopes.back();
            if (depth == 0)
            {
                s.channels.push_back( detail::bvh::channel_indices::x_position );
                s.channels.push_back( detail::bvh::channel_indices::y_position );
                s.channels.push_back( detail::bvh::channel_indices::z_position );
            }
            s.channels.push_back( detail::bvh::channel_indices::y_rotation );
            s.channels.push_back( detail::bvh::channel_indices::z_rotation );
            s.channels.push_back( detail::bvh::channel_indices::x_rotation );
        }

        while (c.valid())
        {
            build_hierarchy_part(name_bindings, bind_pose, c, depth+1, motion_reserve);
            c.next();
        }
    }




    bvh_document::mocap import_bvh_file(const std::string & path, bool first_frame_as_bind_pose)
    {
        std::ifstream ifs(path.c_str());
        return bvh_document(ifs).compile(first_frame_as_bind_pose);
    }

    void export_bvh_to_file(const bvh_document & bvh_doc, const std::string & path)
    {
        std::ofstream ofs(path.c_str());
        bvh_doc.save(ofs);
    }
} // namespace mocaps
} // namespace loci