#ifndef LOCI_CONTROL_XML_DETAIL_PARSE_TIME_OFFSET_H_
#define LOCI_CONTROL_XML_DETAIL_PARSE_TIME_OFFSET_H_

#include "loci/control/xml/detail/parse_common.h"
#include "loci/anim/nodes/time_offset.h"

namespace loci {
namespace control {
namespace xml {
namespace detail
{
    anim_parse_dispatcher::seq_node_ptr parse_time_offset(const xml_parse_tree & xml, motion_set & mocaps, const anim_parse_dispatcher & node_parser)
    {
        double start = 0;
        xml_parse_tree attrs = xml.get_child("<xmlattr>.start");
        start = boost::lexical_cast<double>(attrs.data());

        BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
        {
            if (v.first == "<xmlattr>") { continue; }
            return boost::make_shared<anim::time_offset>(
                node_parser.parse_seq_node(v, mocaps),
                start);
        }

        throw parse_error("invalid markup for time_offset node");
    }
} // namespace detail
} // namespace xml
} // namespace control
} // namespace loci

#endif // LOCI_CONTROL_XML_DETAIL_PARSE_TIME_OFFSET_H_