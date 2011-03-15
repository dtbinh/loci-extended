#ifndef LOCI_NAMED_INDICES_H_
#define LOCI_NAMED_INDICES_H_

/**
 * short
 * long
 *
 * @file   named_indices.h
 * @author David Gill
 * @date   27/01/2010
 */

#include <string>
#include <cstddef> // for size_t
#include <utility> // for pair

#pragma warning(push)
#pragma warning(disable: 4512) // suppress: 'assignment operator could not be generated'
#pragma warning(disable: 4100)

#include <boost/bimap/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>
#include <boost/bimap/set_of.hpp>
#include <boost/bimap/unconstrained_set_of.hpp>

#pragma warning(pop)

namespace loci
{
    class named_indices
    {
    public:
        typedef std::string name_type;
        typedef std::size_t index_type;

    private:
        struct names;   // bimap tags
        struct indices;

        typedef boost::bimaps::bimap<
            boost::bimaps::unordered_set_of< boost::bimaps::tagged<name_type , names> >,
            boost::bimaps::set_of< boost::bimaps::tagged<index_type, indices> >,
            boost::bimaps::unconstrained_set_of_relation
        > bindings_bimap;

        typedef bindings_bimap::map_by< names >::type names_view;
        typedef bindings_bimap::map_by<indices>::type indices_view;

    public:
        typedef indices_view::iterator       iterator;
        typedef indices_view::const_iterator const_iterator;
        typedef names_view::size_type        size_type;

        std::pair<iterator, bool> insert(const name_type & name, index_type index);

        void erase(const name_type & name);
        void erase(index_type       index);
        void erase(iterator      position);

        bool rename(const name_type & name, const name_type & replacement);
        bool rename(index_type       index, const name_type & replacement);
        bool rename(iterator      position, const name_type & replacement);

        iterator       find(const name_type & name);
        const_iterator find(const name_type & name) const;
        iterator       find(index_type index);
        const_iterator find(index_type index) const;

        index_type       index_of(const name_type & name) const;
        const name_type & name_of(index_type index) const;

        iterator       begin();
        const_iterator begin() const;
        iterator       end();
        const_iterator end()   const;

        bool      empty() const;
        size_type size()  const;

        void max_load_factor(float f);

    private:
        bool replace_names_key(names_view::iterator pos, const name_type & replacement);

        bindings_bimap bindings;
    };
} // namespace loci

#endif // LOCI_NAMED_INDICES_H_