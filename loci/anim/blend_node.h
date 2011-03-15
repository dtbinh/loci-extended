#ifndef LOCI_ANIM_BLEND_NODE_H_
#define LOCI_ANIM_BLEND_NODE_H_

/**
 * The base class for all nodes in a blend tree.
 * Defines a common interface and validation system for all nodes in a blend
 * tree to inherit from.
 *
 * @file   blend_node.h
 * @author David Gill
 * @date   29/03/2010
 */

#include <string>
#include <stdexcept>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/utility.hpp> // for boost::noncopyable

#include "loci/anim/time_info.h"

namespace loci {
namespace anim
{
    class node_connect_error : public std::runtime_error
    {
    public:
        node_connect_error(const std::string & message)
            : std::runtime_error(message)
        {
        }
    };

    // forward declaration
    class blend_info;
    class time_info;

    class blend_node
    {
        template <class T>
        friend class slot;
        friend class contracter;

    public:
        typedef double time_type;
        typedef boost::shared_ptr<blend_node> blend_node_ptr;

        explicit blend_node(bool contracting = false);
        blend_node(const blend_node & bn);

        void animate(const time_info & timing, const blend_info & blending);
        time_type total_time() const { return time; }
        void reset_time();

        void find_sync_properties(const time_info & sync_info, const blend_info & blending)
        {
            master_sync_properties(sync_info, blending);
        }

    protected:
        template <class T>
        class slot
        {
            friend class blend_node;

        public:
            slot() { }
            T & operator*()  const { return *node(); }
            T * operator->() const { return node().get(); }
            bool connected() const { return link; }
            void reset()           { link.reset(); }

        private:
            typedef boost::shared_ptr<T> node_ptr;

            struct holder
            {
                node_ptr node;
                bool contracting;
                holder(const node_ptr & node) : node(node) { }
                ~holder() { node->disconnect(); }
            };

            slot(const node_ptr & node) : link(new holder(node)) { }
            node_ptr & node() const { return link->node; }

            boost::shared_ptr<holder> link;
        };

        // alias for convenience
        typedef slot<blend_node> blend_node_slot;

        class contractor : boost::noncopyable
        {
            friend blend_node;

        public:
            template <class T>
            void imbue(const slot<T> & node)
            {
                if (!node.connected()) { return; }
                node->contract_to(*this);
            }

        private:
            contractor() : instigator(0), contracting_node(0) { }
            contractor(const blend_node * instigator,
                       blend_node_ptr & contracting_node)
                : instigator(instigator)
                , contracting_node(&contracting_node)
            {
            }

            bool ending() const { return !contracting_node || !*contracting_node; }
            void deactivate() { contracting_node = 0; }
            bool active() { return !!contracting_node; }

            const blend_node     * instigator;
            const blend_node_ptr * contracting_node;
        };


        template <class T>
        slot<T> connect(const boost::shared_ptr<T> & child) const
        {
            contract(child);
            return slot<T>(child);
        }

        template <class T>
        slot<T> try_connect(const boost::shared_ptr<T> & child) const
        {
            if (try_contract(child))
            {
                return slot<T>(child);
            }
            return slot<T>();
        }

        template <class T>
        void animate(slot<T> & child, const time_info & timing, const blend_info & blending)
        {
            if (!child.connected()) { return; }
            child->animate(timing, blending);
        }

        template <class T>
        void find_sync_properties(slot<T> & child, const time_info & sync_info, const blend_info & blending)
        {
            if (!child.connected()) { return; }
            child->find_sync_properties(sync_info, blending);
        }

    protected: // accessible but almost an implementation detail, allows users to build custom slot types
        void contract(const blend_node_ptr & child) const;
        bool try_contract(const blend_node_ptr & child) const;
        void disconnect();

    private:
        blend_node_ptr contractor_for_child(const blend_node_ptr & child) const;
        void contract_to(contractor & contract);
        bool contracted_to(const contractor & contract) const;
        bool contracted() const;
        void propagate(contractor & contract) const;
        blend_node & operator=(const blend_node &);

    private:
        virtual void evaluate(const time_info & timing, const blend_info & blending) = 0;
        virtual void tick(time_type time_delta, bool reset) { time_delta; reset; /* optional */ }
        virtual void contract_children(contractor & contract) const = 0;
        virtual void master_sync_properties(const time_info & sync_info, const blend_info & blending) = 0;

    private:
        typedef boost::weak_ptr<blend_node> contracting_node_ptr;

        contracting_node_ptr contracting_node;
        unsigned int contract_count;
        const bool contracting;

        time_type time;
    };

    class blend_contract_node : public blend_node
    {
    protected:
        blend_contract_node() : blend_node(true) { }
    };
} // namespace anim
} // namespace loci

#endif // LOCI_ANIM_BLEND_NODE_H_