#ifndef LOCI_NUMERIC_DETAIL_XYZW_MANIPULATORS_H_
#define LOCI_NUMERIC_DETAIL_XYZW_MANIPULATORS_H_

/**
 * Spatial euler classes.
 * Defines a templates base class for 3D euler angles to specialise.
 *
 * @file   basic_euler.h
 * @author David Gill
 * @date   19/11/2009
 */

#include <cstddef>

namespace loci {
namespace numeric {
namespace detail
{
    template <class seqT>
    class with_reset_xy : public seqT
    {
    public:
        typedef typename seqT::value_type value_type;
        void reset(const value_type & nx, const value_type & ny)
        {
            elems[0] = nx;
            elems[1] = ny;
        }
    };

    template <class seqT>
    class with_reset_xyz : public seqT
    {
    public:
        typedef typename seqT::value_type value_type;
        void reset(const value_type & nx, const value_type & ny, const value_type & nz)
        {
            elems[0] = nx;
            elems[1] = ny;
            elems[2] = nz;
        }
    };

    template <class seqT>
    class with_reset_xyzw : public seqT
    {
    public:
        typedef typename seqT::value_type value_type;
        void reset(const value_type & nx, const value_type & ny, const value_type & nz, const value_type & nw)
        {
            elems[0] = nx;
            elems[1] = ny;
            elems[2] = nz;
            elems[3] = nw;
        }
    };

    template <class seqT>
    class with_reset_wxyz : public seqT
    {
    public:
        typedef typename seqT::value_type value_type;
        void reset(const value_type & nw, const value_type & nx, const value_type & ny, const value_type & nz)
        {
            elems[0] = nw;
            elems[1] = nx;
            elems[2] = ny;
            elems[3] = nz;
        }
    };


    template <class seqT, std::size_t I = 0>
    class with_manip_xy : public seqT
    {
    public:
        typedef typename seqT::value_type value_type;

        const value_type & x() const { return elems[I]; }
        const value_type & x(const value_type & nx) { return elems[I] = nx; }
        
        const value_type & y() const { return elems[I+1]; }
        const value_type & y(const value_type & ny) { return elems[I+1] = ny; }
    };

    template <class seqT, std::size_t I = 0>
    class with_manip_xyz : public with_manip_xy<seqT, I>
    {
    public:
        const value_type & z() const { return elems[I+2]; }
        const value_type & z(const value_type & nz) { return elems[I+2] = nz; }
    };

    template <class seqT>
    class with_manip_xyzw : public with_manip_xyz<seqT, 0>
    {
    public:
        const value_type & w() const { return elems[3]; }
        const value_type & w(const value_type & nw) { return elems[3] = nw; }
    };

    template <class seqT>
    class with_manip_wxyz : public with_manip_xyz<seqT, 1>
    {
    public:
        const value_type & w() const { return elems[0]; }
        const value_type & w(const value_type & nw) { return elems[0] = nw; }
    };


    template <class seqT> class with_xy   : public with_reset_xy  < with_manip_xy  <seqT> > { };
    template <class seqT> class with_xyz  : public with_reset_xyz < with_manip_xyz <seqT> > { };
    template <class seqT> class with_wxyz : public with_reset_wxyz< with_manip_wxyz<seqT> > { };
    template <class seqT> class with_xyzw : public with_reset_xyzw< with_manip_xyzw<seqT> > { };
} // namespace detail
} // namespace numeric
} // namespace loci

#endif // LOCI_NUMERIC_DETAIL_XYZW_MANIPULATORS_H_