#ifndef LOCI_CONTROL_XML_DETAIL_PARSE_ADDITIVE_H_
#define LOCI_CONTROL_XML_DETAIL_PARSE_ADDITIVE_H_

#include "loci/control/xml/detail/parse_common.h"
#include "loci/anim/nodes/additive.h"

namespace loci {
namespace control {
namespace xml {
namespace detail
{
    anim_parse_dispatcher::blend_node_ptr parse_additive_combine(const xml_parse_tree & xml, motion_set & mocaps, const anim_parse_dispatcher & node_parser)
    {
        boost::shared_ptr<anim::additive_combine> add = boost::make_shared<anim::additive_combine>();

        double layer_weight = 1.0;
        try_get_attr(xml.get_child("layer"), layer_weight, "weight");
        add->layer_weight( layer_weight );

        BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
        {
            if (boost::iequals(v.first, "base"))
            {
                add->base_anim( parse_blend_node(v.second, mocaps, node_parser) );
            }
            else if (boost::iequals(v.first, "layer"))
            {
                add->layered_anim( parse_blend_node(v.second, mocaps, node_parser) );
            }
        }

        return add;
    }

    anim_parse_dispatcher::blend_node_ptr parse_additive_extract(const xml_parse_tree & xml, motion_set & mocaps, const anim_parse_dispatcher & node_parser)
    {
        boost::shared_ptr<anim::additive_extract> sub = boost::make_shared<anim::additive_extract>();

        BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
        {
            if (boost::iequals(v.first, "base"))
            {
                sub->base_anim( parse_blend_node(v.second, mocaps, node_parser) );
            }
            else if (boost::iequals(v.first, "composite"))
            {
                sub->composite_anim( parse_blend_node(v.second, mocaps, node_parser) );
            }
        }

        return sub;
    }
} // namespace detail
} // namespace xml
} // namespace control
} // namespace loci

#endif // LOCI_CONTROL_XML_DETAIL_PARSE_ADDITIVE_H_