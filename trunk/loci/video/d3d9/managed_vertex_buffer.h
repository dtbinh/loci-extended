#ifndef LOCI_VIDEO_D3D9_MANAGED_VERTEX_BUFFER_H_
#define LOCI_VIDEO_D3D9_MANAGED_VERTEX_BUFFER_H_

/**
 * Encapsulates a d3d9 vertex buffer, providing RAII.
 * Sets up a vertex buffer, allows binding of that buffer to the pipeline and
 * provides RAII support to automatically release that buffer.
 *
 * @file   managed_vertex_buffer.h
 * @author David Gill
 * @date   05/11/2009
 */

#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include <d3d9.h>

namespace loci {
namespace video {
namespace d3d9
{
    // forward declaration
    class device_services;

    class managed_vertex_buffer : boost::noncopyable
    {
    public:
        managed_vertex_buffer(const boost::shared_ptr<device_services> & device,
                              unsigned int bytes);
        ~managed_vertex_buffer();

        void update(const void * vertices, unsigned int bytes);

        void bind(unsigned int stride, DWORD fvf);

    private:
        void map(void *& destination);
        void unmap();

    private:
        boost::shared_ptr<device_services> device;
        LPDIRECT3DVERTEXBUFFER9 buffer;
    };
} // namespace d3d9
} // namespace video
} // namespace loci
    
#endif // LOCI_VIDEO_D3D9_MANAGED_VERTEX_BUFFER_H_