#ifndef LOCI_CONTROL_XML_CONTROLLER_H_
#define LOCI_CONTROL_XML_CONTROLLER_H_

/**
 * The base class for all sequence nodes in a blend tree.
 * Defines a common interface for all blend tree nodes that generate animation
 * data in addition to performing a blend.
 *
 * @file   sequence_node.h
 * @author David Gill
 * @date   29/03/2010
 */

#include <string>
#include <deque>

#include <boost/foreach.hpp>

#include "loci/crowd.h"
#include "loci/anim/nodes/anim_graph.h"
#include "loci/control/xml/parser.h"
#include "loci/control/actor_controller.h"

namespace loci {
namespace control
{
    class xml_crowd_controller : public crowd
    {
    public:
        xml_crowd_controller()
        {
        }

        xml_crowd_controller(const std::string & filename)
        {
           xml::parse(filename, actors, xml_parser);
        }

        void parse(const std::string & filename)
        {
            xml::parse(filename, actors, xml_parser);
        }

        void set_blend_node_parser(const std::string & tag, const xml::parser::blend_node_parser & parser)
        {
            xml_parser.set_blend_node_parser(tag, parser);
        }

        void set_seq_node_parser(const std::string & tag, const xml::parser::seq_node_parser & parser)
        {
            xml_parser.set_seq_node_parser(tag, parser);
        }

        void clear()
        {
            xml_parser = xml::parser();
            actors.clear();
        }


    private:
        void tick(time_type time)
        {
            for (actor_list::iterator actor = actors.begin();
                 actor != actors.end();
                 ++actor)
            {
                (*actor)->update(time);
            }
        }

        void reset_anims()
        {
            for (actor_list::iterator actor = actors.begin();
                 actor != actors.end();
                 ++actor)
            {
                (*actor)->reset();
            }
        }

        size_type calculate_size() const
        {
            return actors.size();
        }

        time_type get_time() const
        {
            return actors.empty() ? 0.0 : actors.front()->animation()->current_time();
        }

        skeleton get_person(size_type index) const
        {
            return actors.at(index)->animation()->active_figure();
        }

        boost::shared_ptr<anim::anim_graph> get_animation(size_type index) const
        {
            return actors.at(index)->animation();
        }

    private:
        typedef std::deque< boost::shared_ptr<keyed_actor_controller> > actor_list;
        actor_list actors;

        xml::parser xml_parser;
    };
} // namespace control
} // namespace loci

#endif // LOCI_CONTROL_XML_CONTROLLER_H_