/**
 * Implementation of a hierarchy and hierarchy cursor.
 * Implements the hierarchy class.
 *
 * @file   hierarchy.cpp
 * @author David Gill
 * @date   09/01/2009
 */

#include "loci/hierarchy.h"

namespace loci
{
    hierarchy::node_linkage::node_linkage(size_type parent,
                                          size_type left_sibling)
        : parent(parent)
        , first_child(npos)
        , left_sibling(left_sibling)
        , right_sibling(npos)
    {
    }

    hierarchy::cursor::cursor()
        : links(0)
        , current(npos)
    {
    }

    hierarchy::cursor::cursor(const linkages & links, size_type current)
        : links(&links)
        , current(current)
    {
    }

    bool hierarchy::cursor::operator==(const cursor & rhs) const
    {
        return current == rhs.current && (links == rhs.links || current == npos);
    }

    hierarchy::cursor & hierarchy::cursor::to_parent()
    {
        current = linkage().parent;
        return *this;
    }

    hierarchy::cursor & hierarchy::cursor::to_children()
    {
        current = linkage().first_child;
        return *this;

    }

    hierarchy::cursor & hierarchy::cursor::prev()
    {
        current = linkage().left_sibling;
        return *this;
    }

    hierarchy::cursor & hierarchy::cursor::next()
    {
        current = linkage().right_sibling;
        return *this;
    }

    hierarchy::cursor & hierarchy::cursor::to_parent_safe()
    {
        if (!valid()) { return *this; }
        return to_parent();
    }

    hierarchy::cursor & hierarchy::cursor::to_children_safe()
    {
        if (!valid()) { return *this; }
        return to_children();
    }

    hierarchy::cursor & hierarchy::cursor::next_safe()
    {
        if (!valid()) { return *this; }
        return next();
    }

    hierarchy::cursor & hierarchy::cursor::prev_safe()
    {
        if (!valid()) { return *this; }
        return prev();
    }

    bool hierarchy::cursor::valid() const
    {
        return position() != npos;
    }

    bool hierarchy::cursor::leaf() const
    {
        return valid() && linkage().first_child != npos;
    }

    hierarchy::size_type hierarchy::cursor::position() const
    {
        return current;
    }


    hierarchy::cursor hierarchy::clear(size_type reserve)
    {
        linkages().swap(links);
        links.reserve(reserve);
        links.push_back(node_linkage(npos, npos));
        return root();
    }

    hierarchy::cursor hierarchy::root() const
    {
        return cursor(links, 0);
    }

    hierarchy::cursor hierarchy::attach_to(const cursor & parent)
    {
        node_linkage & parent_link = links.at(parent.position());
        size_type new_id = static_cast<size_type>(links.size());
        size_type left_id = new_id;

        if (parent_link.first_child != npos)
        {
            size_type & last_child_id = links[parent_link.first_child].left_sibling;
            size_type & next_sibling_id = links[last_child_id].right_sibling;
            left_id = last_child_id;
            next_sibling_id = last_child_id = new_id;
        }
        else
        {
            parent_link.first_child = new_id;
        }
            
        links.push_back(node_linkage(parent.position(), left_id));
        return cursor(links, new_id);
    }

    hierarchy::cursor hierarchy::cursor_at(size_type position) const
    {
        return cursor(links, (position < size()) ? position : npos);
    }

    hierarchy::size_type hierarchy::size() const
    {
        return links.size();
    }

    void hierarchy::swap(hierarchy & rhs)
    {
        links.swap(rhs.links);
    }
} // loci namespace