
#include "loci/anim/blend_node.h"
#include "loci/anim/time_info.h"

namespace loci {
namespace anim
{
    blend_node::blend_node(bool contracting)
        : contract_count(0)
        , contracting(contracting)
        , time(0)
    {
    }

    blend_node::blend_node(const blend_node & bn)
        : contract_count(0)
        , contracting(bn.contracting)
        , time(bn.time)
    {
    }

    void blend_node::reset_time()
    {
        time = 0;
        tick(0, true);
    }

    void blend_node::animate(const time_info & timing, const blend_info & blending)
    {
        time_type delta = timing.time_delta();
        if (this->time == timing.time())
        {
            delta = 0;
        }
        else
        {
            time_type calculated_delta = timing.time() - this->time;
            bool reset = calculated_delta < 0 && delta >= 0 ||
                         calculated_delta > 0 && delta <= 0;
            this->time = timing.time();
            tick(delta, reset);
        }
        evaluate(time_info(time, delta, timing), blending);
    }

    void blend_node::contract(const blend_node_ptr & child) const
    {
        if (contracted() || contracting)
        {
            blend_node_ptr contracting_node = contractor_for_child(child);
            contractor contract(this, contracting_node);
            return child->contract_to(contract);
        }
    }

    bool blend_node::try_contract(const blend_node_ptr & child) const
    {
        try {
            contract(child);
            return true;
        } catch(const node_connect_error &) { }
        return false;
    }

    void blend_node::disconnect()
    {
        if (contracted() || contracting)
        {
            contractor terminating_contract;
            contract_to(terminating_contract);
        }
    }

    blend_node::blend_node_ptr blend_node::contractor_for_child(const blend_node_ptr & child) const
    {
        return contracting ? child : contracting_node.lock();
    }

    void blend_node::contract_to(contractor & contract)
    {
        // detect cycles
        if (contract.instigator == this)
        {
            throw node_connect_error("detected cycle: node descends from itself");
        }

        // inactive contract but carry on to detect any cycles
        if (!contract.active())
        {
            contract_children(contract);
            return;
        }

        // de-contracting
        if (contract.ending())
        {
            if (contract_count > 0) // propagate decrement to children
            {
                propagate(contract);
                --contract_count;
            }
            return;
        }

        if (contracted()) // already has a contract...
        {
            if (!contracted_to(contract)) // ...but a different contract, so fail
            {
                throw node_connect_error("node descends from multiple contracting nodes");
            }
            if (contract.contracting_node->get() == this) // ...but is contracting itself, so fail
            {
                throw node_connect_error("node descends from same contractor more than once");
            }
            // propagate increment to children
            propagate(contract);
            ++contract_count;
            return;
        }
        
        propagate(contract); // if able to contract children...
        contracting_node = contracting_node_ptr(*contract.contracting_node); // ... then contract self too
        contract_count = 1;
    }

    bool blend_node::contracted_to(const contractor & contract) const
    {
        blend_node_ptr own = contracting_node.lock();
        return own.get() == contract.contracting_node->get();
    }

    bool blend_node::contracted() const
    {
        return contract_count > 0 && !contracting_node.expired();
    }

    void blend_node::propagate(contractor & contract) const
    {
        if (contracting)
        {
            const blend_node_ptr * to_reactivate = contract.contracting_node;
            contract.deactivate();
            contract_children(contract);
            contract.contracting_node = to_reactivate;
            return;
        }
        contract_children(contract);
    }
} // namespace anim
} // namespace loci