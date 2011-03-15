#ifndef LOCI_CONTROL_XML_PARSER_H_
#define LOCI_CONTROL_XML_PARSER_H_

#include <string>
#include <map>
#include <fstream>

#include "loci/control/xml/detail/parse_common.h"
#include <boost/property_tree/xml_parser.hpp>

#include "loci/anim/blend_info.h"
#include "loci/anim/blend_node.h"
#include "loci/anim/sequence_node.h"
#include "loci/anim/nodes/anim_graph.h"
#include "loci/anim/nodes/blend_sequence.h"
#include "loci/anim/nodes/trim.h"
#include "loci/anim/nodes/additive.h"
#include "loci/anim/nodes/duration.h"
#include "loci/anim/nodes/feather.h"
#include "loci/control/actor_controller.h"

namespace loci {
namespace control {
namespace xml
{
    class parser
    {
    public:
        typedef anim::blend_node::blend_node_ptr         blend_node_ptr;
        typedef anim::sequence_node::seq_node_ptr        seq_node_ptr;
        
        typedef anim_parse_dispatcher::xml_parse_tree    xml_parse_tree;
        typedef anim_parse_dispatcher::blend_node_parser blend_node_parser;
        typedef anim_parse_dispatcher::seq_node_parser   seq_node_parser;

    private:
        typedef boost::shared_ptr<anim::blend_sequence> schedule_ptr;

        class activity
        {
        public:
            class transition
            {
            public:
                transition(const xml_parse_tree & xml)
                    : xml(xml)
                {
                    get_attr(xml, in, "in");
                    get_attr(xml, out, "out");
                }

                blend_node_ptr parse(const anim_parse_dispatcher & anim_parser, motion_set & motions, double & ease_in_duration) const
                {
                    if (ease_in_duration < 0)
                    {
                        BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
                        {
                            if (tag(v, "<xmlattr>")) { continue; }
                            seq_node_ptr anim = anim_parser.parse_seq_node(v, motions);
                            ease_in_duration = anim->duration();
                            return anim;
                        }
                    }

                    BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
                    {
                        if (tag(v, "<xmlattr>")) { continue; }
                        return anim_parser.parse_blend_node(v, motions);
                    }

                    throw parse_error("no transition animation to parse");
                }

                double blend_in_time() const { return in; }
                double blend_out_time() const { return out; }

            private:
                xml_parse_tree xml;
                double in, out;
            };

            activity(const xml_parse_tree & xml)
            {
                BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
                {
                    if (tag(v, "transition"))
                    {
                        add_transition(v.second);
                    }
                    else if (tag(v, "animation"))
                    {
                        animation = v.second;
                    }
                }
            }

            blend_node_ptr parse_transition(const std::string & from, const anim_parse_dispatcher & anim_parser, motion_set & motions, double & in, double & out, double & ease_in_duration) const
            {
                transitions_map::const_iterator iter = transitions.find(from);
                if (iter == transitions.end()) { return blend_node_ptr(); }

                const transition & t = iter->second;
                in  = t.blend_in_time();
                out = t.blend_out_time();
                return t.parse(anim_parser, motions, ease_in_duration);
            }

            blend_node_ptr parse_animation(const anim_parse_dispatcher & anim_parser, motion_set & motions, double & duration) const
            {
                if (duration < 0)
                {
                    BOOST_FOREACH(const xml_parse_tree::value_type & v, animation)
                    {
                        if (tag(v, "<xmlattr>")) { continue; }
                        seq_node_ptr anim = anim_parser.parse_seq_node(v, motions);
                        duration = anim->duration();
                        return anim;
                    }
                }

                return parse_animation(anim_parser, motions);
            }

            blend_node_ptr parse_animation(const anim_parse_dispatcher & anim_parser, motion_set & motions) const
            {
                BOOST_FOREACH(const xml_parse_tree::value_type & v, animation)
                {
                    if (tag(v, "<xmlattr>")) { continue; }
                    return anim_parser.parse_blend_node(v, motions);
                }
                throw parse_error("no animation");
            }

        private:
            void add_transition(const xml_parse_tree & xml)
            {
                std::string from;
                get_attr(xml, from, "from");
                transitions.insert(std::make_pair(from, transition(xml)));
            }

            typedef std::map<std::string, transition> transitions_map;

            transitions_map transitions;
            xml_parse_tree animation;
        };

        activity & find_activity(const std::string & act)
        {
            activity_map::iterator it = activities.find(act);
            if (it == activities.end()) // activity not found, make one that points directly at a clip
            {
                xml_parse_tree pt;
                pt.put("animation.clip.<xmlattr>.motion", act);
                it = activities.insert(std::make_pair(act, activity(pt))).first;
            }
            return it->second;
        }

        blend_node_ptr layer_animation(double weight, const std::string & mask,
                                       const blend_node_ptr & layer, const blend_node_ptr & source)
        {
            if (weight < 0 && mask.empty()) { weight = 1.0; }

            blend_node_ptr anim = layer;

            if (weight > 0)
            {
                anim = boost::make_shared<anim::additive_combine>(source, anim, weight);
            }

            if (!mask.empty())
            {
                mask_map::const_iterator iter = masks.find(mask);
                if (iter != masks.end())
                {
                    boost::shared_ptr<anim::feather> masked = boost::make_shared<anim::feather>(motions.figure().name_bindings(), motions.figure().structure());
                    masked->source_anim(source);
                    masked->mask_anim(anim);
                    masked->mask_weights( iter->second );
                    anim = masked;
                }
            }

            return anim;
        }

        seq_node_ptr trim_animation(double duration, const blend_node_ptr & anim)
        {
            return boost::make_shared<anim::trim>(0, duration, anim);
        }

        void parse_perform_children(const xml_parse_tree & xml, blend_node_ptr & anim, const activity & act, unsigned int reparse_stop = 0)
        {
            unsigned int reparse_tracker = 1;
        
            BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
            {
                if (tag(v, "perform"))
                {
                    parse_layered_perform(v.second, anim);
                }
                else if (tag(v, "schedule"))
                {
                    anim = parse_layered_schedule(v.second, xml, act, reparse_tracker);
                }

                if (reparse_tracker++ == reparse_stop)
                {
                    break;
                }
            }
        }

        void parse_perform_motion(const xml_parse_tree & xml)
        {
            double over;
            BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
            {
                if (tag(v, "accelerate"))
                {
                    over = get_attr<double>(v.second, "over");
                    speed_total += over;
                    final_speed = get_attr<double>(v.second, "to");
                    actor_control->append_speed(final_speed, over);
                }
                else if (tag(v, "turning"))
                {
                    over = get_attr<double>(v.second, "over");
                    turn_total += over;
                    final_heading = get_attr<double>(v.second, "to");
                    actor_control->append_heading(final_heading, over);
                }
            }
        }

        blend_node_ptr parse_perform_anim(const xml_parse_tree & xml, const activity & act, unsigned int reparse_stop = 0)
        {
            blend_node_ptr anim = act.parse_animation(anim_parser, motions);
            parse_perform_children(xml, anim, act, reparse_stop);
            return anim;
        }

        std::string parse_layered_scheduled_perform(const xml_parse_tree & xml,
                                                    const std::string & from, boost::shared_ptr<anim::blend_sequence> & schedule,
                                                    const xml_parse_tree & parent_xml, const activity & parent_act, unsigned int reparse_stop)
        {
            // have to reparse the source since it needs to be connected multiple times
            blend_node_ptr source = parse_perform_anim(parent_xml, parent_act, reparse_stop);

            std::string act_id;
            double duration = -1;
            double trans_duration = -1;
            double weight = -1;
            std::string mask;

            get_attr(xml, act_id, "act");
            try_get_attr(xml, duration, "for");
            try_get_attr(xml, trans_duration, "ease_in");
            try_get_attr(xml, mask, "on");
            try_get_attr(xml, weight, "overlay");

            activity & act = find_activity(act_id);
            blend_node_ptr anim = act.parse_animation(anim_parser, motions, duration);
            parse_perform_children(xml, anim, act);
            anim = layer_animation(weight, mask, anim, source);

            double trans_in;
            double anim_in = trans_duration;
            blend_node_ptr transition = act.parse_transition(from, anim_parser, motions, trans_in, anim_in, trans_duration);
            if (transition)
            {
                source = parse_perform_anim(parent_xml, parent_act, reparse_stop);
                transition = layer_animation(weight, mask, transition, source);
                schedule->append(trim_animation(trans_duration, transition), trans_in);
            }
            schedule->append(trim_animation(duration, anim), anim_in);

            return act_id;
        }

        blend_node_ptr parse_layered_schedule(const xml_parse_tree & xml, const xml_parse_tree & parent_xml, const activity & parent_act, unsigned int reparse_stop)
        {
            boost::shared_ptr<anim::blend_sequence> schedule = boost::make_shared<anim::blend_sequence>();
            std::string from;

            BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
            {
                if (tag(v, "perform"))
                {
                    from = parse_layered_scheduled_perform(v.second, from, schedule,
                                                           parent_xml, parent_act, reparse_stop);
                }
            }

            return schedule;
        }

        void parse_layered_perform(const xml_parse_tree & xml, blend_node_ptr & source)
        {
            std::string act_id;
            double weight = -1.0;
            std::string mask;
            get_attr(xml, act_id, "act");
            try_get_attr(xml, mask, "on");
            try_get_attr(xml, weight, "overlay");

            activity & act = find_activity(act_id);
            blend_node_ptr anim = parse_perform_anim(xml, act);
            source = layer_animation(weight, mask, anim, source);
        }

        std::string parse_scheduled_perform(const xml_parse_tree & xml, const std::string & from, const schedule_ptr & schedule)
        {
            std::string act_id;
            double duration = -1;
            double trans_duration = -1;
            get_attr(xml, act_id, "act");
            try_get_attr(xml, duration, "for");
            try_get_attr(xml, trans_duration, "ease_in");

            double trans_in;
            double anim_in = trans_duration;

            activity & act = find_activity(act_id);
            blend_node_ptr anim = act.parse_animation(anim_parser, motions, duration);
            parse_perform_children(xml, anim, act);
            parse_perform_motion(xml);

            blend_node_ptr transition = act.parse_transition(from, anim_parser, motions, trans_in, anim_in, trans_duration);
            if (transition)
            {
                schedule->append(trim_animation(trans_duration, transition), trans_in);
                accum_time += (trans_duration - trans_in);
            }
            if (anim_in < 0) { anim_in = 0; }
            schedule->append(trim_animation(duration, anim), anim_in);
            accum_time += (duration - anim_in);

            if (accum_time > speed_total)
            {
                actor_control->append_speed(final_speed, accum_time - speed_total);
                speed_total = accum_time;
            }
            if (accum_time > turn_total)
            {
                actor_control->append_heading(final_heading, accum_time - turn_total);
                turn_total = accum_time;
            }

            return act_id;
        }

        seq_node_ptr parse_schedule(const xml_parse_tree & xml)
        {
            schedule_ptr schedule = boost::make_shared<anim::blend_sequence>();
            bool loop = false;
            std::string loop_attr;
            try_get_attr(xml, loop, "repeat");
            if (!loop_attr.empty())
            {
                loop = boost::iequals(loop_attr, "true");
            }
            schedule->looping(loop);

            std::string from;
            BOOST_FOREACH(const xml_parse_tree::value_type & v, xml)
            {
                if (tag(v, "perform"))
                {
                    from = parse_scheduled_perform(v.second, from, schedule);
                }
            }

            return schedule;
        }

        void parse_motions(const xml_parse_tree & xml);
        void parse_masks(const xml_parse_tree & xml);
        void parse_actions(const xml_parse_tree & xml);
        boost::shared_ptr<keyed_actor_controller> & parse_actor(const xml_parse_tree & xml);

    public:
        parser();

        template <class SeqContT>
        void parse(const xml_parse_tree & xml, SeqContT & actors)
        {
            BOOST_FOREACH(const xml_parse_tree::value_type & lv, xml)
            {
                if (!tag(lv, "lociscript")) { continue; }
                BOOST_FOREACH(const xml_parse_tree::value_type & v, lv.second)
                {
                    if (tag(v, "motions"))
                    {
                        parse_motions(v.second);
                    }
                    else if (tag(v, "masks"))
                    {
                        parse_masks(v.second);
                    }
                    else if (tag(v, "actions"))
                    {
                        parse_actions(v.second);
                    }
                    else if (tag(v, "actor"))
                    {
                        actors.push_back(parse_actor(v.second));
                    }
                }
            }
        }

        void set_blend_node_parser(const std::string & tag, const blend_node_parser & parser)
        {
            anim_parser.set_blend_node_parser(tag, parser);
        }

        void set_seq_node_parser(const std::string & tag, const seq_node_parser & parser)
        {
            anim_parser.set_seq_node_parser(tag, parser);
        }

    private:
        typedef std::map<std::string, activity> activity_map;
        typedef std::map<std::string, anim::blend_info::weight_list> mask_map;

        activity_map activities;
        motion_set motions;
        anim_parse_dispatcher anim_parser;
        mask_map masks;
        boost::shared_ptr<keyed_actor_controller> actor_control;

        double speed_total, turn_total, final_speed, final_heading;
        double accum_time;
    };

    template <class SeqContT>
    void parse(std::istream & is, SeqContT & actors, parser & p)
    {
        xml_parse_tree xml;
        read_xml(is, xml, boost::property_tree::xml_parser::no_comments &&
                          boost::property_tree::xml_parser::trim_whitespace);
        p.parse(xml, actors);
    }

    template <class SeqContT>
    void parse(const std::string & filename, SeqContT & actors, parser & p)
    {
        std::ifstream ifs(filename.c_str());
        parse(ifs, actors, p);
    }

    template <class SeqContT>
    void parse(std::istream & is, SeqContT & actors)
    {
        parser p;
        parse(is, actors, p);
    }

    template <class SeqContT>
    void parse(const std::string & filename, SeqContT & actors)
    {
        parser p;
        parse(is, p);
    }
}
}
}

#endif // LOCI_CONTROL_XML_PARSER_H_