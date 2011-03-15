#ifndef LOCI_CONTROL_XML_DETAIL_PARSE_CLIP_H_
#define LOCI_CONTROL_XML_DETAIL_PARSE_CLIP_H_

#include <sstream>

#include "loci/control/xml/detail/parse_common.h"
#include "loci/anim/nodes/clip.h"

namespace loci {
namespace control {
namespace xml {
namespace detail
{
    anim_parse_dispatcher::seq_node_ptr parse_clip(const xml_parse_tree & xml, motion_set & mocaps, const anim_parse_dispatcher &)
    {
        xml_parse_tree nameattr = xml.get_child("<xmlattr>.motion");

        motion_set::motion_details anim = mocaps.acquire_motion(nameattr.get_value<std::string>());
        float yoffset = anim.yoffset;
        try_get_attr(xml, yoffset, "yoffset");

#ifdef LOCI_DEBUGGING
        return boost::make_shared<anim::clip>(anim.motion, anim.syncher, yoffset, anim.r, anim.g, anim.b);
#else
        return boost::make_shared<anim::clip>(anim.motion, anim.syncher, yoffset);
#endif
    }
} // namespace detail
} // namespace xml
} // namespace control
} // namespace loci

#endif // LOCI_CONTROL_XML_DETAIL_PARSE_CLIP_H_