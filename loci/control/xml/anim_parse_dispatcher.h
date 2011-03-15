#ifndef LOCI_CONTROL_XML_ANIM_PARSE_DISPATCHER_H_
#define LOCI_CONTROL_XML_ANIM_PARSE_DISPATCHER_H_

#include <string>
#include <map>

#include <boost/property_tree/ptree.hpp>
#include <boost/function.hpp>

#include "loci/anim/blend_node.h"
#include "loci/anim/sequence_node.h"
#include "loci/control/xml/motion_set.h"
#include "loci/control/xml/parse_error.h"

namespace loci {
namespace control {
namespace xml
{
    class anim_parse_dispatcher
    {
    public:
        typedef boost::property_tree::ptree xml_parse_tree;
        typedef anim::blend_node::blend_node_ptr blend_node_ptr;
        typedef anim::sequence_node::seq_node_ptr seq_node_ptr;
        typedef boost::function3<blend_node_ptr, const xml_parse_tree &, motion_set &, const anim_parse_dispatcher &> blend_node_parser;
        typedef boost::function3<seq_node_ptr, const xml_parse_tree &, motion_set &, const anim_parse_dispatcher &> seq_node_parser;

        void set_blend_node_parser(const std::string & tag, const blend_node_parser & parser)
        {
            blend_node_parsers[tag] = parser;
        }

        void set_seq_node_parser(const std::string & tag, const seq_node_parser & parser)
        {
            seq_node_parsers[tag] = parser;
        }

        blend_node_ptr parse_blend_node(const boost::property_tree::ptree::value_type & xml,
                                        motion_set & mocaps) const
        {
            blend_parsers_map::const_iterator result = blend_node_parsers.find(xml.first);
            if (result != blend_node_parsers.end())
            {
                return result->second(xml.second, mocaps, *this);
            }
            return parse_seq_node(xml, mocaps);
        }

        seq_node_ptr parse_seq_node(const boost::property_tree::ptree::value_type & xml,
                                    motion_set & mocaps) const
        {
            seq_parsers_map::const_iterator result = seq_node_parsers.find(xml.first);
            if (result == seq_node_parsers.end())
            {
                throw parse_error("invalid node");
            }
            return result->second(xml.second, mocaps, *this);
        }

    private:
        typedef std::map<std::string, blend_node_parser> blend_parsers_map;
        typedef std::map<std::string, seq_node_parser>   seq_parsers_map;
        blend_parsers_map blend_node_parsers;
        seq_parsers_map   seq_node_parsers;
    };
} // namespace xml
} // namespace control
} // namespace loci


#endif // LOCI_CONTROL_XML_ANIM_PARSE_DISPATCHER_H_
