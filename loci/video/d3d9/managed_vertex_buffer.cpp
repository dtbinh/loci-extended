/**
 * Implementation of a managed_vertex_buffer.
 * Encapsulates a d3d9 vertex buffer, providing RAII.
 *
 * @file   managed_vertex_buffer.cpp
 * @author David Gill
 * @date   06/11/2009
 */

#include <algorithm>

#include <boost/shared_ptr.hpp>

#include <d3d9.h>

#include "loci/video/d3d9/managed_vertex_buffer.h"
#include "loci/video/d3d9/device_services.h"
#include "loci/video/d3d9/d3d_error.h"

namespace loci {
namespace video {
namespace d3d9
{
    managed_vertex_buffer::managed_vertex_buffer(const boost::shared_ptr<device_services> & device,
                                                 unsigned int bytes)
        : device(device)
    {
        device->allocate_vertex_buffer(buffer, bytes, false, 0, D3DPOOL_MANAGED);
    }

    managed_vertex_buffer::~managed_vertex_buffer()
    {
        buffer->Release();
    }

    void managed_vertex_buffer::update(const void * vertices, unsigned int bytes)
    {
        void * data;
        map(data);

        const char * source = reinterpret_cast<const char *>(vertices);
        char * destination  = reinterpret_cast<char *>(data);
        std::copy(source, source + bytes, destination);

        unmap();
    }

    void managed_vertex_buffer::bind(unsigned int stride, DWORD fvf)
    {
        device->bind_fvf(fvf);
        device->bind_vertex_buffer(buffer, stride);
    }

    void managed_vertex_buffer::map(void *& destination)
    {
        HRESULT r = buffer->Lock(0, 0, &destination, 0);
        check_failure(r, "Failed to map vertex buffer", "Lock");
    }

    void managed_vertex_buffer::unmap()
    {
        HRESULT r = buffer->Unlock();
        check_failure(r, "Failed to unmap vertex buffer", "UnLock");
    }
} // namespace d3d9
} // namespace video
} // namespace loci