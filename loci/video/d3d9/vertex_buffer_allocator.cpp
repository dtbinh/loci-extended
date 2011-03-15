/**
 * Implementation of a vertex_buffer_allocator.
 * Allocates vertex buffers.
 *
 * @file   vertex_buffer_allocator.cpp
 * @author David Gill
 * @date   06/11/2009
 */

#include <memory>

#include <boost/shared_ptr.hpp>

#include "loci/video/d3d9/vertex_buffer_allocator.h"
#include "loci/video/d3d9/managed_vertex_buffer.h"
#include "loci/video/d3d9/device_services.h"

namespace loci {
namespace video {
namespace d3d9
{
    vertex_buffer_allocator::vertex_buffer_allocator(
            const boost::shared_ptr<device_services> & device)
        : device(device)
    {
    }

    std::auto_ptr<managed_vertex_buffer>
        vertex_buffer_allocator::create_managed(unsigned int length,
                                                const void * vertices,
                                                unsigned int bytes) const
    {
        std::auto_ptr<managed_vertex_buffer> buffer(
            new managed_vertex_buffer(device, length));

        buffer->update(vertices, bytes);

        return buffer;
    }
} // namespace d3d9
} // namespace video
} // namespace loci