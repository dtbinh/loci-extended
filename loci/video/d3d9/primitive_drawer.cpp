/**
 * Implementation of a primitive_drawer.
 * Provides a specific interface for drawing primitives.
 *
 * @file   primitive_drawer.cpp
 * @author David Gill
 * @date   07/11/2009
 */

#include <boost/shared_ptr.hpp>

#include <d3d9.h>
#include <d3dx9.h>

#include "loci/video/d3d9/primitive_drawer.h"
#include "loci/video/d3d9/device_services.h"

namespace loci {
namespace video {
namespace d3d9
{
    primitive_drawer::primitive_drawer(const boost::shared_ptr<device_services> & device,
                                       D3DPRIMITIVETYPE primitive_topology)
        : device(device)
    {
        topology(primitive_topology);
    }

    void primitive_drawer::topology(D3DPRIMITIVETYPE primitive_topology)
    {
        this->primitive_topology = primitive_topology;
    }

    void primitive_drawer::draw(unsigned int count,
                                unsigned int start_offset)
    {
        device->draw(primitive_topology, count, start_offset);
    }
} // namespace d3d9
} // namespace video
} // namespace loci