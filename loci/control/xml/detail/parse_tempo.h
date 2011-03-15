#ifndef LOCI_CONTROL_XML_DETAIL_PARSE_TEMPO_H_
#define LOCI_CONTROL_XML_DETAIL_PARSE_TEMPO_H_

#include "loci/control/xml/detail/parse_common.h"
#include "loci/anim/nodes/tempo.h"

namespace loci {
namespace control {
namespace xml {
namespace detail
{
    anim_parse_dispatcher::blend_node_ptr parse_tempo(const xml_parse_tree & xml, motion_set & mocaps, const anim_parse_dispatcher & node_parser)
    {
        double pace = get_attr<double>(xml, "rate");

        BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
        {
            if (v.first == "<xmlattr>") { continue; }
            return boost::make_shared<anim::tempo>(
                node_parser.parse_blend_node(v, mocaps),
                pace);
        }

        throw parse_error("invalid markup for tempo node");
    }
} // namespace detail
} // namespace xml
} // namespace control
} // namespace loci

#endif // LOCI_CONTROL_XML_DETAIL_PARSE_TEMPO_H_