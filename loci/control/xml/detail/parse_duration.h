#ifndef LOCI_CONTROL_XML_DETAIL_PARSE_DURATION_H_
#define LOCI_CONTROL_XML_DETAIL_PARSE_DURATION_H_

#include "loci/control/xml/detail/parse_common.h"
#include "loci/anim/nodes/duration.h"

namespace loci {
namespace control {
namespace xml {
namespace detail
{
    anim_parse_dispatcher::seq_node_ptr parse_apply_duration(const xml_parse_tree & xml, motion_set & mocaps, const anim_parse_dispatcher & node_parser)
    {
        double duration = get_attr<double>(xml, "of");

        BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
        {
            if (v.first == "<xmlattr>") { continue; }
            return boost::make_shared<anim::apply_duration>(
                node_parser.parse_blend_node(v, mocaps),
                duration);
        }

        throw parse_error("invalid markup for apply_duration node");
    }

    anim_parse_dispatcher::seq_node_ptr parse_alter_duration(const xml_parse_tree & xml, motion_set & mocaps, const anim_parse_dispatcher & node_parser)
    {
        double duration = -1;
        double pace = -1;
        try_get_attr(xml, duration, "to");
        try_get_attr(xml, pace, "pace");

        BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
        {
            if (v.first == "<xmlattr>") { continue; }
            if (duration == -1)
            {         
                return boost::make_shared<anim::alter_duration>(
                    node_parser.parse_seq_node(v, mocaps),
                    pace);
            }
            return boost::make_shared<anim::alter_duration>(
                duration,
                node_parser.parse_seq_node(v, mocaps));
        }

        throw parse_error("invalid markup for alter_duration node");
    }
} // namespace detail
} // namespace xml
} // namespace control
} // namespace loci

#endif // LOCI_CONTROL_XML_DETAIL_PARSE_DURATION_H_