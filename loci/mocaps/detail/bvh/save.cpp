/**
 * Exports a bvh document.
 * Defines an exporter for a bvh document, saving a scope_list to a
 * file.
 *
 * @file   save.cpp
 * @author David Gill
 * @date   23/04/2010
 */

#include <string>
#include <ostream>

#include "loci/mocaps/detail/bvh/scope.h"

namespace loci {
namespace mocaps {
namespace detail {
namespace bvh
{
    namespace // anonymous
    {
        std::ostream & indent(std::ostream & sink, unsigned int depth, bool in_scope = true)
        {
            depth += (in_scope ? 1 : 0);
            for (unsigned int i = 0; i < depth; ++i)
            {
                sink << "  ";
            }
            return sink;
        }

        scope_list::const_iterator save_hierarchy_recurse(std::ostream & sink, scope_list::const_iterator current, scope_list::const_iterator end)
        {
            unsigned int depth = current->depth;

            sink << current->name << '\n';
            indent(sink, depth, false) << "{\n";
            indent(sink, depth) << "OFFSET " << current->offset.x() << ' '
                                             << current->offset.y() << ' '
                                             << current->offset.z() << '\n';
            
            if (!current->end_site())
            {
                indent(sink, depth) << "CHANNELS " << current->channels.size();
                for (scope::channel_index_list::const_iterator channel_iter = current->channels.begin();
                        channel_iter != current->channels.end();
                        ++channel_iter)
                {
                    if      (*channel_iter == channel_indices::x_position) { sink << " Xposition"; }
                    else if (*channel_iter == channel_indices::y_position) { sink << " Yposition"; }
                    else if (*channel_iter == channel_indices::z_position) { sink << " Zposition"; }
                    else if (*channel_iter == channel_indices::x_rotation) { sink << " Xrotation"; }
                    else if (*channel_iter == channel_indices::y_rotation) { sink << " Yrotation"; }
                    else if (*channel_iter == channel_indices::z_rotation) { sink << " Zrotation"; }
                }
                sink << '\n';

                ++current;
                while (current != end && current->depth == depth + 1)
                {
                    indent(sink, depth) << (current->end_site() ? "End Site " : "JOINT ");
                    current = save_hierarchy_recurse(sink, current, end);
                }
            }
            else
            {
                ++current;
            }
            
            indent(sink, depth, false) << "}\n";
            return current;
        }
    } // anonymous namespace

    void save_hierarchy(std::ostream & sink, const scope_list & scopes)
    {
        sink << "HIERARCHY\n";
        sink << "ROOT ";
        save_hierarchy_recurse(sink, scopes.begin(), scopes.end());
    }

    void save_motion(std::ostream & sink, const scope_list & scopes, unsigned int frame_count, float time_per_frame)
    {
        sink << "MOTION\n"
             << "Frames: " << frame_count << '\n'
             << "Frame Time: " << time_per_frame << '\n';

        for (unsigned int frame = 0; frame < frame_count; ++frame)
        {
            for (scope_list::const_iterator joint_iter = scopes.begin(); joint_iter != scopes.end(); ++joint_iter)
            {
                unsigned int num_channels = joint_iter->channels.size();
                scope::motion_channel_list::const_iterator motion_iter = joint_iter->motion.begin() + num_channels * frame;
                for (unsigned int channel = 0; channel < num_channels; ++channel, ++motion_iter)
                {
                    sink << *motion_iter << ' ';
                }
            }
            sink << '\n';
        }
    }

    void save(std::ostream & sink, const scope_list & scopes, unsigned int frame_count, float time_per_frame)
    {
        save_hierarchy(sink, scopes);
        save_motion(sink, scopes, frame_count, time_per_frame);
    }
} // namespace bvh
} // namespace detail
} // namespace mocaps
} // namespace loci