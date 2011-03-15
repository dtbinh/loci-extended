#ifndef LOCI_HIERARCHY_H_
#define LOCI_HIERARCHY_H_

/**
 * Defines a hierarchy tree, cursor and depth-first iteration strategies.
 * A hierarchy is/builds a tree structure, it uses a cursor to permit navigation
 * and iteration over the tree.
 *
 * @file   hierarchy.h
 * @author David Gill
 * @date   06/01/2010
 */

#include <cstddef>
#include <vector>

namespace loci
{
    class hierarchy
    {
    public:
        typedef std::size_t size_type;
        static const size_type npos = size_type(-1);
        
    private:
        struct node_linkage
        {
            node_linkage(size_type parent, size_type left_sibling);
            size_type parent, first_child, left_sibling, right_sibling;
        };

        typedef std::vector<node_linkage> linkages;

    public:
        class cursor
        {
        public:
            typedef hierarchy::size_type size_type;

            cursor();
            cursor(const linkages & links, size_type current);

            bool operator==(const cursor & rhs) const;
            bool operator!=(const cursor & rhs) const { return !(*this == rhs); }

            cursor & to_parent();
            cursor & to_children();
            cursor & prev();
            cursor & next();

            cursor & to_parent_safe();
            cursor & to_children_safe();
            cursor & next_safe();
            cursor & prev_safe();

            bool valid() const;
            bool leaf() const;

            size_type position() const;

        private:
            const node_linkage & linkage() const { return (*links)[current]; }

            const linkages * links;
            size_type current;
        };

    public:
        hierarchy(size_type reserve = 20) { clear(reserve); }
        cursor clear(size_type reserve = 20);
        cursor root() const;
        cursor attach_to(const cursor & parent);
        cursor cursor_at(size_type position) const;
        size_type size() const;
        void swap(hierarchy & rhs);
        
    private:
        linkages links;
    };

    namespace detail
    {
        template <class UnaryFunction>
        void depth_first_recurse(hierarchy::cursor c, UnaryFunction & f)
        {
            f(hierarchy::cursor(c));
            c.to_children();
            while (c.valid())
            {
                depth_first_recurse(c, f);
                c.next();
            }
        }

        template <class RandomAccessIterator, class UnaryFunction>
        void depth_first_recurse(hierarchy::cursor c, const RandomAccessIterator & first, UnaryFunction & f)
        {
            f( *(first + c.position()) );
            c.to_children();
            while (c.valid())
            {
                depth_first_recurse(c, first, f);
                c.next();
            }
        }
    } // namespace detail

    template <class UnaryFunction>
    UnaryFunction depth_first(hierarchy::cursor root, UnaryFunction f)
    {
        if (!root.valid()) { return f; }
        detail::depth_first_recurse(root, f);
        return f;
    }

    template <class UnaryFunction>
    UnaryFunction depth_first(hierarchy h, UnaryFunction f)
    {
        detail::depth_first_recurse(h.root(), f);
        return f;
    }

    template <class RandomAccessIterator, class UnaryFunction>
    UnaryFunction depth_first(hierarchy::cursor root, RandomAccessIterator first, UnaryFunction f)
    {
        if (!root.valid()) { return f; }
        detail::depth_first_recurse(root, first, f);
        return f;
    }

    template <class RandomAccessIterator, class UnaryFunction>
    UnaryFunction depth_first(hierarchy h, RandomAccessIterator first, UnaryFunction f)
    {
        detail::depth_first_recurse(h.root(), first, f);
        return f;
    }
} // namespace loci

#endif // LOCI_HIERARCHY_H_