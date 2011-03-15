#ifndef LOCI_ANIM_SEQUENCE_NODE_H_
#define LOCI_ANIM_SEQUENCE_NODE_H_

/**
 * The base class for all sequence nodes in a blend tree.
 * Defines a common interface for all blend tree nodes that generate animation
 * data in addition to performing a blend.
 *
 * @file   sequence_node.h
 * @author David Gill
 * @date   29/03/2010
 */

#include "loci/anim/blend_node.h"

namespace loci {
namespace anim
{
    class sequence_node : public blend_node
    {
    public:
        typedef boost::shared_ptr<sequence_node> seq_node_ptr;

        sequence_node(bool contracting = true) : blend_node(contracting) { }
        
        time_type duration() const
        {
            return calculate_duration();
        }

    protected:
        typedef slot<sequence_node> seq_node_slot; // alias for convenience

    private:
        virtual time_type calculate_duration() const = 0;
    };

    class noncontracting_sequence_node : public sequence_node
    {
    protected:
        noncontracting_sequence_node() : sequence_node(false) { }
    };

    class sequence_leaf_node : public sequence_node
    {
        void contract_children(contractor &) const { }
    };
} // namespace anim
} // namespace loci

#endif // LOCI_ANIM_SEQUENCE_NODE_H_