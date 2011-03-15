/**
 * Parses a bvh document.
 * Defines a parser for a bvh document, populating a scope list with scopes.
 *
 * @file   parser.cpp
 * @author David Gill
 * @date   21/02/2010
 */

#include <string>
#include <istream>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include "loci/mocaps/parse_error.h"
#include "loci/mocaps/detail/bvh/parser.h"
#include "loci/mocaps/detail/bvh/scope.h"

using namespace boost::algorithm;

namespace loci {
namespace mocaps {
namespace detail {
namespace bvh
{
    namespace // anonymous
    {
        template <class T>
        T & parse_value(std::istream & is, T & result, const char * error = "Unexpected parse error")
        {
            std::string str;
            is >> str;
            if (!is)
            {
                throw parse_error("Unexpected end of tokens");
            }
            try { result = boost::lexical_cast<T>(str); }
            catch (...)
            {
                throw parse_error(error);
            }
            return result;
        }

        void parse_token(std::istream & is, const std::string & what, std::string & result, const char * error)
        {
            if (!(is >> result))
            {
                throw parse_error("Unexpected end of tokens");
            }
            if (!iequals(result, what))
            {
                throw parse_error(error);
            }
        }

        void parse_hierarchy_joints(std::istream & is, scope_list & result, unsigned int depth)
        {
            result.push_back(scope(depth));
            scope & current_scope = result.back();

            std::string token;

            if (parse_value(is, token) != "{")
            {
                current_scope.name = token;
                parse_token(is, "{", token, "Missing opening curly bracket");
            }
            
            for (;;)
            {
                parse_value(is, token);
                if (iequals("OFFSET", token))
                {
                    float xyz;
                    current_scope.offset.x(parse_value(is, xyz, "Unable to parse OFFSET x-component"));
                    current_scope.offset.y(parse_value(is, xyz, "Unable to parse OFFSET y-component"));
                    current_scope.offset.z(parse_value(is, xyz, "Unable to parse OFFSET z-component"));
                }
                else if (iequals("CHANNELS", token))
                {   
                    unsigned int num_channels;
                    parse_value(is, num_channels, "Unable to parse CHANNELS size attribute");
                    for (unsigned int i = 0; i < num_channels; ++i)
                    {
                        channel_indices::type channel;
                        parse_value(is, token);
                        if      (iequals("Xposition", token)) { channel = channel_indices::x_position; }
                        else if (iequals("Yposition", token)) { channel = channel_indices::y_position; }
                        else if (iequals("Zposition", token)) { channel = channel_indices::z_position; }
                        else if (iequals("Xrotation", token)) { channel = channel_indices::x_rotation; }
                        else if (iequals("Yrotation", token)) { channel = channel_indices::y_rotation; }
                        else if (iequals("Zrotation", token)) { channel = channel_indices::z_rotation; }
                        else
                        {
                            throw parse_error("Invalid channel indicator");
                        }
                        current_scope.channels.push_back( channel );
                    }
                }
                else if (iequals("JOINT", token))
                {
                    parse_hierarchy_joints(is, result, depth+1);
                }
                else if (iequals("End", token))
                {
                    parse_token(is, "Site", token, "'End' token found but not 'End Site'");
                    parse_hierarchy_joints(is, result, depth+1);
                }
                else if (token == "}")
                {
                    break;
                }
                else
                {
                    throw parse_error("Unexpected token in joint body");
                }
            }
        }

        void parse_motion_channels(std::istream & is, unsigned int num_frames, scope_list & target)
        {
            float motion_value;

            // first reserve memory for all motions
            for (scope_list::iterator iter = target.begin(); iter != target.end(); ++iter)
            {
                iter->motion.reserve( iter->channels.size() * num_frames );
            }

            for (unsigned int f = 0; f < num_frames; ++f)
            {
                for (scope_list::iterator iter = target.begin(); iter != target.end(); ++iter)
                {
                    scope::channel_index_list::size_type num_channels = iter->channels.size();
                    for (scope::channel_index_list::size_type c = 0; c < num_channels; ++c)
                    {
                        parse_value(is, motion_value, "Unable to parse a motion value");
                        iter->motion.push_back(motion_value);
                    }
                }
            }
        }
    } // anonymous namespace

    void parse_hierarchy(std::istream & is, scope_list & result)
    {
        std::string token;
        parse_token(is, "HIERARCHY", token, "Failed to find HIERARCHY token");
        parse_token(is, "ROOT", token, "Failed to find ROOT joint");

        parse_hierarchy_joints(is, result, 0);
    }

    void parse_motion(std::istream & is, scope_list & target, unsigned int & num_frames, float & time_per_frame)
    {
        std::string token;
        parse_token(is, "MOTION", token, "Failed to find MOTION token");
        
        parse_token(is, "Frames:", token, "Failed to find 'Frames:' token");
        parse_value(is, num_frames, "Unable to parse 'Frames:' value");
        
        parse_token(is, "Frame", token, "Failed to find 'Frame Time' tokens");
        parse_token(is, "Time:", token, "'Frame' token found but not 'Frame Time:'");
        parse_value(is, time_per_frame, "Unable to parse 'Frame Time:' value");

        parse_motion_channels(is, num_frames, target);
    }

    void parse(std::istream & is, scope_list & result, unsigned int & num_frames, float & time_per_frame)
    {
        parse_hierarchy(is, result);
        parse_motion(is, result, num_frames, time_per_frame);
    }

} // namespace bvh
} // namespace detail
} // namespace mocaps
} // namespace loci