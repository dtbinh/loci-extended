#ifndef LOCI_CONTROL_XML_DETAIL_PARSE_TRIM_H_
#define LOCI_CONTROL_XML_DETAIL_PARSE_TRIM_H_

#include "loci/control/xml/detail/parse_common.h"
#include "loci/anim/nodes/trim.h"

namespace loci {
namespace control {
namespace xml {
namespace detail
{
    anim_parse_dispatcher::seq_node_ptr parse_trim(const xml_parse_tree & xml, motion_set & mocaps, const anim_parse_dispatcher & node_parser)
    {
        double start = 0;
        double end = 0;
        double duration = 0;
        int overload = 1;
        xml_parse_tree attrs = xml.get_child("<xmlattr>");
        BOOST_FOREACH(const xml_parse_tree::value_type & v, attrs)
        {
            if (boost::iequals(v.first, "start")) { start = boost::lexical_cast<double>(v.second.data()); }
            else if (boost::iequals(v.first, "end")) { end = boost::lexical_cast<double>(v.second.data()); }
            else if (boost::iequals(v.first, "duration")) { duration = boost::lexical_cast<double>(v.second.data()); overload = 2; }
        }
        BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
        {
            if (v.first == "<xmlattr>") { continue; }
            if (overload == 1)
            {
                return boost::make_shared<anim::trim>(
                    node_parser.parse_seq_node(v, mocaps),
                    start, end);
            }
            else
            {
                return boost::make_shared<anim::trim>(
                    start, duration,
                    node_parser.parse_blend_node(v, mocaps));
            }
        }

        throw parse_error("invalid markup for 'trim' node");
    }
} // namespace detail
} // namespace xml
} // namespace control
} // namespace loci

#endif // LOCI_CONTROL_XML_DETAIL_PARSE_TRIM_H_