#ifndef LOCI_CONTROL_XML_DETAIL_PARSE_SPEED_TRANSITION_H_
#define LOCI_CONTROL_XML_DETAIL_PARSE_SPEED_TRANSITION_H_

#include "loci/control/xml/detail/parse_common.h"
#include "loci/anim/nodes/speed_blend.h"

namespace loci {
namespace control {
namespace xml {
namespace detail
{
    anim_parse_dispatcher::blend_node_ptr parse_speed_transition(const xml_parse_tree & xml, motion_set & mocaps, const anim_parse_dispatcher & node_parser)
    {
        boost::shared_ptr<anim::speed_transition> transition =
            boost::make_shared<anim::speed_transition>();

        anim_parse_dispatcher::blend_node_ptr node;
        double slowspeed, fastspeed;
        double ratechange;

        ratechange = boost::lexical_cast<double>(xml.get_child("<xmlattr>.ratechange").data());

        slowspeed = boost::lexical_cast<double>(xml.get_child("slow.<xmlattr>.speed").data());
        fastspeed = boost::lexical_cast<double>(xml.get_child("fast.<xmlattr>.speed").data());

        BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
        {
            if (boost::iequals(v.first, "slow"))
            {
                BOOST_FOREACH(const xml_parse_tree::value_type & sv, v.second)
                {
                    if (sv.first == "<xmlattr>") { continue; }
                    node = node_parser.parse_blend_node(sv, mocaps);
                    transition->slow_anim(node, slowspeed, ratechange);
                    break;
                }
            }
            else if (boost::iequals(v.first, "fast"))
            {
                BOOST_FOREACH(const xml_parse_tree::value_type & fv, v.second)
                {
                    if (fv.first == "<xmlattr>") { continue; }
                    node = node_parser.parse_blend_node(fv, mocaps);
                    transition->fast_anim(node, fastspeed, 1.0 / ratechange);
                    break;
                }
            }
        }

        return transition;
    }
} // namespace detail
} // namespace xml
} // namespace control
} // namespace loci

#endif // LOCI_CONTROL_XML_DETAIL_PARSE_SPEED_TRANSITION_H_