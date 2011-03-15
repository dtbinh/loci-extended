#ifndef LOCI_NUMERIC_DETAIL_BASE_SEQUENCE_H_
#define LOCI_NUMERIC_DETAIL_BASE_SEQUENCE_H_

/**
 * common statically-sized sequence class
 * Defines a class with common functionality intended to be inherited from
 * by mathematical sequence types such as vectors, eulers and quaternions.
 *
 * @file   base_sequence.h
 * @author David Gill
 * @date   20/12/2009
 */

#include <iterator>
#include <cstddef>
#include <ostream>

#include "loci/numeric/maths.h"

namespace loci {
namespace numeric {
namespace detail
{
    template <typename T, std::size_t N>
    class sequence_types
    {
    public:
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;

        typedef T* iterator;
        typedef const T* const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        typedef std::size_t size_type;
        static size_type size() { return N; }
        enum { static_size = N };
    };

    template <class dT, typename T, std::size_t N>
    class base_sequence : public sequence_types<T, N>
    {
    protected:
        T elems[N];

        typedef dT derived_type;
        dT & derived_this_ref() { return static_cast<dT &>(*this); }

    public:
        // raw data accessors (read-only/read-write)
        T * data() { return elems; }
        const T * data() const { return elems; }
        T * c_array() { return elems; }

        void swap(dT & other)
        {
            std::swap_ranges(begin(), end(), other.begin());
        }

        reference       operator[](std::size_t i) { return elems[i]; }
        const_reference operator[](std::size_t i) const { return elems[i]; }
        reference       operator()(std::size_t i) { return elems[i]; }
        const_reference operator()(std::size_t i) const { return elems[i]; }

        bool approx_equal(const derived_type & rhs) const
        {
            for (std::size_t i = 0; i < static_size; ++i)
            {
                if ( !approx_equal(elems[i], rhs.elems[i]) )
                {
                    return false;
                }
            }
            return true;
        }

        template <typename U>
        void assign(const U & value)
        {
            std::fill_n(begin(), size(), value);
        }

        iterator                begin()        { return elems; }
        const_iterator          begin()  const { return elems; }
        iterator                end()          { return elems + size(); }
        const_iterator          end()    const { return elems + size(); }

        reverse_iterator        rbegin()       { return reverse_iterator(end()); }
        const_reverse_iterator  rbegin() const { return const_reverse_iterator(end()); }
        reverse_iterator        rend()         { return reverse_iterator(begin()); }
        const_reverse_iterator  rend()   const { return const_reverse_iterator(begin()); }
    };

    template <class dT, typename T, std::size_t N, class CharT>
    std::basic_ostream<CharT> & operator<<(std::basic_ostream<CharT> & os, const base_sequence<dT, T, N> & bs)
    {
        os << '(' << bs[0];
        for (std::size_t i = 1; i < base_sequence<dT, T, N>::static_size; ++i)
        {
            os << ", " << bs[i];
        }
        return os << ')';
    }
} // namespace detail
} // namespace numeric
} // namespace loci

#endif // LOCI_NUMERIC_DETAIL_BASE_SEQUENCE_H_
