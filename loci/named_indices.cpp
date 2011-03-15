/**
 * Implementation of named_indices.
 * Implements the named_indices class.
 *
 * @file   named_indices.cpp
 * @author David Gill
 * @date   27/01/2009
 */

#include "loci/named_indices.h"

namespace loci
{
    std::pair<named_indices::iterator, bool> named_indices::insert(const name_type & name, index_type index)
    {
        typedef indices_view::value_type citation;
        return bindings.by<indices>().insert( citation(index, name) );
    }

    void named_indices::erase(const name_type & name)
    {
        bindings.by<names>().erase(name);
    }

    void named_indices::erase(index_type index)
    {
        bindings.by<indices>().erase(index);
    }

    void named_indices::erase(iterator position)
    {
        bindings.by<indices>().erase(position);
    }

    bool named_indices::rename(const name_type & name, const name_type & replacement)
    {
        return replace_names_key(
            bindings.by<names>().find(name),
            replacement);
    }

    bool named_indices::rename(index_type index, const name_type & replacement)
    {
        return rename(find(index), replacement);
    }

    bool named_indices::rename(iterator position, const name_type & replacement)
    {
        return replace_names_key(
            bindings.project<names>(position),
            replacement);
    }

    named_indices::iterator named_indices::find(const name_type & name)
    {
        return bindings.project<indices>(
            bindings.by<names>().find(name));
    }

    named_indices::const_iterator named_indices::find(const name_type & name) const
    {
        return bindings.project<indices>(
            bindings.by<names>().find(name));
    }

    named_indices::iterator named_indices::find(index_type index)
    {
        return bindings.by<indices>().find(index);
    }

    named_indices::const_iterator named_indices::find(index_type index) const
    {
        return bindings.by<indices>().find(index);
    }

    named_indices::index_type named_indices::index_of(const name_type & name) const
    {
        return bindings.by<names>().at(name);
    }

    const named_indices::name_type & named_indices::name_of(index_type index) const
    {
        return bindings.by<indices>().at(index);
    }

    named_indices::iterator named_indices::begin()
    {
        return bindings.by<indices>().begin();
    }

    named_indices::const_iterator named_indices::begin() const
    {
        return bindings.by<indices>().begin();
    }

    named_indices::iterator named_indices::end()
    {
        return bindings.by<indices>().end();
    }

    named_indices::const_iterator named_indices::end() const {
        return bindings.by<indices>().end();
    }

    bool named_indices::empty() const
    {
        return bindings.by<names>().empty();
    }

    named_indices::size_type named_indices::size() const
    {
        return bindings.by<names>().size();
    }

    void named_indices::max_load_factor(float f)
    {
        bindings.by<names>().max_load_factor(f);
    }

    bool named_indices::replace_names_key(names_view::iterator pos, const name_type & replacement)
    {
        names_view & from_names = bindings.by<names>();
        if (pos != from_names.end())
        {
            return from_names.replace_key(pos, replacement);
        }
        return false;
    }
} // loci namespace