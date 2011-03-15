#ifndef LOCI_VIDEO_D3D9_ADAPTERS_SERVICE_H_
#define LOCI_VIDEO_D3D9_ADAPTERS_SERVICE_H_

/**
 * Initialises the d3d9 runtime.
 * Provides a service for connecting to a d3d9 device and acts as a RAII wrapper
 * for the IDirect3D9 object.
 *
 * @file   adapters_service.h
 * @author David Gill
 * @date   13/09/2009
 */

#include <boost/utility.hpp>

#include <d3d9.h>

namespace loci {
namespace video {
namespace d3d9
{
    class adapters_service : boost::noncopyable
    {
    public:
        adapters_service();
        ~adapters_service();

        void connect_device(LPDIRECT3DDEVICE9 & device,
                            D3DPRESENT_PARAMETERS & pp);

    private:
        LPDIRECT3D9 d3d9;
    };
} // namespace d3d9
} // namespace video
} // namespace loci

#endif // LOCI_VIDEO_D3D9_ADAPTERS_SERVICE_H_