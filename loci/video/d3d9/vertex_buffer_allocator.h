#ifndef LOCI_VIDEO_D3D9_VERTEX_BUFFER_ALLOCATOR_H_
#define LOCI_VIDEO_D3D9_VERTEX_BUFFER_ALLOCATOR_H_

/**
 * Allocates vertex buffers.
 * Wraps a device to provide a lean interface for allocating vertex buffers.
 *
 * @file   vertex_buffer_allocator.h
 * @author David Gill
 * @date   06/11/2009
 */

#include <memory>

#include <boost/shared_ptr.hpp>

namespace loci {
namespace video {
namespace d3d9
{
    // forward declarations
    class device_services;
    class managed_vertex_buffer;

    class vertex_buffer_allocator
    {
    public:
        vertex_buffer_allocator(const boost::shared_ptr<device_services> & device);

        std::auto_ptr<managed_vertex_buffer> create_managed(unsigned int length,
                                                            const void * vertices,
                                                            unsigned int bytes) const;

    private:
        boost::shared_ptr<device_services> device;
    };
} // namespace d3d9
} // namespace video
} // namespace loci

#endif // LOCI_VIDEO_D3D9_VERTEX_BUFFER_ALLOCATOR_H_