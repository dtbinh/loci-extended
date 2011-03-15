#include "loci/control/xml/parser.h"
#include "loci/control/xml/detail/default_node_parsers.h"
#include "loci/anim/weight_mask.h"
#include "loci/control/actor_controller.h"
#include "loci/numeric/vector.h"
#include "loci/anim/nodes/sync.h"


namespace loci {
namespace control {
namespace xml
{
    parser::parser()
    {
        set_seq_node_parser("sequence", detail::parse_sequence);
        set_seq_node_parser("clip", detail::parse_clip);
        set_seq_node_parser("trim", detail::parse_trim);
        set_seq_node_parser("timeoffset", detail::parse_time_offset);
        set_seq_node_parser("applyduration", detail::parse_apply_duration);
        set_seq_node_parser("alterduration", detail::parse_alter_duration);
        set_seq_node_parser("heading", detail::parse_fix_heading);
        set_seq_node_parser("autoturn", detail::parse_follow_heading);
        
        set_blend_node_parser("speedtransition", detail::parse_speed_transition);
        set_blend_node_parser("add", detail::parse_additive_combine);
        set_blend_node_parser("sub", detail::parse_additive_extract);
        set_blend_node_parser("tempo", detail::parse_tempo);
        set_blend_node_parser("sync", detail::parse_sync_blend);
    }

    void parser::parse_motions(const xml_parse_tree & xml)
    {
        BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
        {
            if (tag(v, "bvh"))
            {
                boost::shared_ptr<loci::anim::sync_warp> syncher;
                loci::anim::sync_warp::sync_group group = 0;

                BOOST_FOREACH(const xml_parse_tree::value_type & sv, v.second)
                {
                    if (sv.first == "<xmlattr>") { continue; }
                    else if (tag(sv, "sync"))
                    {
                        if (!syncher) { syncher = boost::make_shared<loci::anim::sync_warp>(); }

                        std::istringstream iss(get_attr<std::string>(sv.second, "frames"));
                        anim::time_type frame;
                        while (iss >> frame)
                        {
                            syncher->add_frame(group, frame);
                        }
                        ++group;
                    }
                }

                float yoffset = 0.0f;
                try_get_attr(v.second, yoffset, "yoffset");
                yoffset = -yoffset;

#ifdef LOCI_DEBUGGING
                float r = 1.0;
                float g = 1.0;
                float b = 0.0;

                std::string colours;
                try_get_attr(v.second, colours, "colour");

                if (!colours.empty())
                {
                    std::istringstream iss(colours);
                    iss >> r >> g >> b;

                    r /= 255.0;
                    g /= 255.0;
                    b /= 255.0;

                    r = r < 0.0 ? 0.0 : r;
                    r = r > 1.0 ? 1.0 : r;
                    g = r < 0.0 ? 0.0 : g;
                    g = r > 1.0 ? 1.0 : g;
                    b = r < 0.0 ? 0.0 : b;
                    b = r > 1.0 ? 1.0 : b;
                }

                motions.add(
                    get_attr<std::string>(v.second, "id"),
                    get_attr<std::string>(v.second, "src"),
                    yoffset,
                    syncher,
                    r, g, b);
#else
                motions.add(
                    get_attr<std::string>(v.second, "id"),
                    get_attr<std::string>(v.second, "src"),
                    yoffset,
                    syncher);
#endif
            }
        }
    }

    void parser::parse_masks(const xml_parse_tree & xml)
    {
        BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
        {
            if (tag(v, "region"))
            {

                anim::blend_info::weight_list & weights =
                    masks.insert(std::make_pair(get_attr<std::string>(v.second, "id"),
                                 anim::blend_info::weight_list(motions.figure().structure()->size(), 0.0))).first->second;

                BOOST_FOREACH(const xml_parse_tree::value_type & vb, v.second)
                {
                    if (tag(vb, "branch"))
                    {
                        anim::weight_mask_branch(weights, *(motions.figure().name_bindings()), *(motions.figure().structure()),
                                                 get_attr<std::string>(vb.second, "parent"),
                                                 get_attr<unsigned int>(vb.second, "depth"),
                                                 get_attr<double>(vb.second, "startweight"),
                                                 get_attr<double>(vb.second, "endweight"));
                    }
                }
            }
        }
    }

    void parser::parse_actions(const xml_parse_tree & xml)
    {
        std::string act_id;
        BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
        {
            if (tag(v, "activity"))
            {
                get_attr(v.second, act_id, "id");
                activities.insert(std::make_pair(act_id, activity(v.second)));
            }
        }
    }

    boost::shared_ptr<keyed_actor_controller> & parser::parse_actor(const xml_parse_tree & xml)
    {
        float x = 0.0f;
        float z = 0.0f;
        std::string at;
        try_get_attr(xml, at, "at");
        if (!at.empty())
        {
            std::istringstream iss(at);
            iss >> x >> z;
        }

        final_speed = 1.0;
        final_heading = 0.0;
        try_get_attr(xml, final_speed, "speed");
        try_get_attr(xml, final_heading, "heading");

        speed_total = turn_total = accum_time = 0.0;
        actor_control = boost::make_shared<keyed_actor_controller>(final_speed, final_heading);
        actor_control->animation(
            boost::make_shared<anim::anim_graph>(
                boost::make_shared<anim::sync_blend>(parse_schedule(xml)),
                motions.figure(), numeric::vector2f(x, z)) );

        return actor_control;
    }
}
}
}