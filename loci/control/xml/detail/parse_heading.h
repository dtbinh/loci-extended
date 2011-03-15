#ifndef LOCI_CONTROL_XML_DETAIL_PARSE_HEADING_H_
#define LOCI_CONTROL_XML_DETAIL_PARSE_HEADING_H_

#include "loci/control/xml/detail/parse_common.h"
#include "loci/anim/nodes/heading.h"

namespace loci {
namespace control {
namespace xml {
namespace detail
{
    anim_parse_dispatcher::seq_node_ptr parse_fix_heading(const xml_parse_tree & xml, motion_set & mocaps, const anim_parse_dispatcher & node_parser)
    {
        double angle = get_attr<double>(xml, "angle");

        BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
        {
            if (v.first == "<xmlattr>") { continue; }
            return boost::make_shared<anim::fix_heading>(
                node_parser.parse_seq_node(v, mocaps),
                angle);
        }

        throw parse_error("invalid markup for fix_heading node");
    }

    anim_parse_dispatcher::seq_node_ptr parse_follow_heading(const xml_parse_tree & xml, motion_set & mocaps, const anim_parse_dispatcher & node_parser)
    {
        BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
        {
            if (v.first == "<xmlattr>") { continue; }
            return boost::make_shared<anim::follow_heading>(
                node_parser.parse_seq_node(v, mocaps));
        }

        throw parse_error("invalid markup for follow_heading node");
    }
} // namespace detail
} // namespace xml
} // namespace control
} // namespace loci

#endif // LOCI_CONTROL_XML_DETAIL_PARSE_HEADING_H_