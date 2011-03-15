#ifndef LOCI_CONTROL_XML_DETAIL_PARSE_COMMON_H_
#define LOCI_CONTROL_XML_DETAIL_PARSE_COMMON_H_

#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/predicate.hpp> // for iequals

#include "loci/control/xml/anim_parse_dispatcher.h"
#include "loci/control/xml/motion_set.h"
#include "loci/control/xml/parse_error.h"

//using namespace boost::property_tree;
//using namespace boost::algorithm;

namespace loci {
namespace control {
namespace xml
{
    typedef anim_parse_dispatcher::xml_parse_tree xml_parse_tree;

    inline anim_parse_dispatcher::blend_node_ptr parse_blend_node(const anim_parse_dispatcher::xml_parse_tree & xml, motion_set & mocaps, const anim_parse_dispatcher & node_parser)
    {
        BOOST_FOREACH(const anim_parse_dispatcher::xml_parse_tree::value_type & v, xml)
        {
            if (v.first == "<xmlattr>") { continue; }
            return node_parser.parse_blend_node(v, mocaps);
        }
        return anim_parse_dispatcher::blend_node_ptr();
    }

    inline anim_parse_dispatcher::seq_node_ptr parse_seq_node(const anim_parse_dispatcher::xml_parse_tree & xml, motion_set & mocaps, const anim_parse_dispatcher & node_parser)
    {
        BOOST_FOREACH(const anim_parse_dispatcher::xml_parse_tree::value_type & v, xml)
        {
            if (v.first == "<xmlattr>") { continue; }
            return node_parser.parse_seq_node(v, mocaps);
        }
        return anim_parse_dispatcher::seq_node_ptr();
    }


    bool inline tag(const anim_parse_dispatcher::xml_parse_tree::value_type & xml, const std::string & match)
    {
        return boost::iequals(xml.first, match);
    }

    template <class T>
    T & get_attr(const anim_parse_dispatcher::xml_parse_tree & xml, T & result, const std::string & attribute)
    {
        return result = xml.get<T>("<xmlattr>." + attribute);
    }

    template <class T>
    T get_attr(const anim_parse_dispatcher::xml_parse_tree & xml, const std::string & attribute)
    {
        return xml.get<T>("<xmlattr>." + attribute);
    }

    template <class T>
    T & try_get_attr(const anim_parse_dispatcher::xml_parse_tree & xml, T & inout, const std::string & attribute)
    {
        boost::optional<T> result = xml.get_optional<T>("<xmlattr>." + attribute);
        if (!result) { return inout; }
        return inout = *result;
    }

    template <class T>
    boost::optional<T> &  get_optional_attr(const anim_parse_dispatcher::xml_parse_tree & xml, boost::optional<T> & result, const std::string & attribute)
    {
        return result = xml.get_optional<T>("<xmlattr>." + attribute);
    }
} // namespace xml
} // namespace control
} // namespace loci

#endif // LOCI_CONTROL_XML_DETAIL_PARSE_COMMON_H_