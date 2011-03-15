#ifndef LOCI_CROWD_H_
#define LOCI_CROWD_H_

/**
 * An immutable construct representing a skeletal structure.
 * Composes the various structures together to represent a complete skeleton,
 * this class is entirely immutable and has no functional properties to it.
 *
 * @file   skeleton.h
 * @author David Gill
 * @date   26/01/2010
 */

#include <cstddef>
#include <deque>

#include <boost/shared_ptr.hpp>

#include "loci/skeleton.h"
#include "loci/anim/nodes/anim_graph.h"

namespace loci
{
    class crowd
    {
    public:
        typedef double time_type;
        typedef std::size_t size_type;

        crowd & update(time_type time_delta)
        {
            tick(time_delta);
            return *this;
        }

        void reset()
        {
            reset_anims();
        }

        size_type size() const
        {
            return calculate_size();
        }

        skeleton person(size_type index) const
        {
            return get_person(index);
        }

        boost::shared_ptr<anim::anim_graph> animation(size_type index) const
        {
            return get_animation(index);
        }

        time_type time() const
        {
            return get_time();
        }

    private:
        virtual void tick(time_type time_delta) = 0;
        virtual void reset_anims() = 0;
        virtual size_type calculate_size() const = 0;
        virtual skeleton get_person(size_type index) const = 0;
        virtual boost::shared_ptr<anim::anim_graph> get_animation(size_type index) const = 0;
        virtual time_type get_time() const = 0;
    };

    class simple_crowd_base : public crowd
    {
    protected:
        typedef std::deque< boost::shared_ptr<anim::anim_graph> > animgraph_list;
        animgraph_list animations;

        // protected to prevent instantiation of an otherwise concrete base class
        simple_crowd_base() { }

    private:
        void tick(time_type time)
        {
            for (animgraph_list::iterator anim = animations.begin();
                 anim != animations.end();
                 ++anim)
            {
                (*anim)->animate(time);
            }
        }

        void reset_anims()
        {
            for (animgraph_list::iterator anim = animations.begin();
                 anim != animations.end();
                 ++anim)
            {
                (*anim)->reset();
            }
        }

        size_type calculate_size() const
        {
            return animations.size();
        }

        time_type get_time() const
        {
            return animations.front()->current_time();
        }

        skeleton get_person(size_type index) const
        {
            return animations[index]->active_figure();
        }

        boost::shared_ptr<anim::anim_graph> get_animation(size_type index) const
        {
            return animations[index];
        }
    };

    class simple_crowd : public simple_crowd_base
    {
    public:
        void insert_actor(const boost::shared_ptr<anim::anim_graph> & animation)
        {
            animations.push_back(animation);
        }
    };
} // namespace loci

#endif // LOCI_CROWD_H_