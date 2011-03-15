#ifndef LOCI_VIDEO_D3D9_PRIMITIVE_DRAWER_H_
#define LOCI_VIDEO_D3D9_PRIMITIVE_DRAWER_H_

/**
 * Provides a specific interface for drawing primitives.
 * Wraps the device interface to provide a semantically-rich interface for
 * drawing.
 *
 * @file   primitive_drawer.h
 * @author David Gill
 * @date   07/11/2009
 */

#include <boost/shared_ptr.hpp>

#include <d3d9.h>

namespace loci {
namespace video {
namespace d3d9
{
    // forward declaration
    class device_services;

    class primitive_drawer
    {
    public:
        primitive_drawer(const boost::shared_ptr<device_services> & device,
                         D3DPRIMITIVETYPE primitive_topology = D3DPT_TRIANGLELIST);

        void topology(D3DPRIMITIVETYPE primitive_topology);

        void draw(unsigned int count, unsigned int start_offset = 0);

    private:
        boost::shared_ptr<device_services> device;
        D3DPRIMITIVETYPE primitive_topology;
    };

} // namespace d3d9
} // namespace video
} // namespace loci

#endif // LOCI_VIDEO_D3D9_PRIMITIVE_DRAWER_H_