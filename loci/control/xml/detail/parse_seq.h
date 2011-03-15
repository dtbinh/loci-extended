#ifndef LOCI_CONTROL_XML_DETAIL_PARSE_SEQ_H_
#define LOCI_CONTROL_XML_DETAIL_PARSE_SEQ_H_

#include "loci/control/xml/detail/parse_common.h"
#include "loci/anim/nodes/blend_sequence.h"

namespace loci {
namespace control {
namespace xml {
namespace detail
{
    anim_parse_dispatcher::seq_node_ptr parse_seq_stage(
        const xml_parse_tree & xml,
        anim::blend_node::time_type & overlap,
        motion_set & mocaps,
        const anim_parse_dispatcher & node_parser)
    {
        xml_parse_tree attrib = xml.get_child("<xmlattr>.overlap");
        overlap = attrib.get_value<anim::blend_node::time_type>();
        BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
        {
            if (v.first == "<xmlattr>") { continue; }
            return node_parser.parse_seq_node(v, mocaps);
        }
        throw parse_error("stage must contain a sequence node");
    }

    bool parse_seq_loop_attrib(const xml_parse_tree & xml)
    {
        BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
        {
            if (!boost::iequals(v.first, "loop")) { continue; }
            return boost::iequals(v.second.data(), "true");
        }
        return false;
    }

    anim_parse_dispatcher::seq_node_ptr parse_sequence(const xml_parse_tree & xml, motion_set & mocaps, const anim_parse_dispatcher & node_parser)
    {
        boost::shared_ptr<anim::blend_sequence> seq =
            boost::make_shared<anim::blend_sequence>(parse_seq_loop_attrib(xml.get_child("<xmlattr>")));

        anim_parse_dispatcher::seq_node_ptr stage_node;
        anim::blend_node::time_type overlap;

        do
        {
            BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
            {
                if (boost::iequals(v.first, "stage"))
                {
                    stage_node = parse_seq_stage(v.second, overlap, mocaps, node_parser);
                    seq->append(stage_node, overlap);
                }
            }
            if (seq->empty())
            {
                throw parse_error("sequence node without any nested stage members");
            }
        }
        while (seq->size() < 2);

        return seq;
    }
} // namespace detail
} // namespace xml
} // namespace control
} // namespace loci

#endif // LOCI_CONTROL_XML_DETAIL_PARSE_SEQ_H_