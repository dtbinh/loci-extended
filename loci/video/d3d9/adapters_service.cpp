/**
 * Implementation of adapters_service.
 * Implements d3d9 adapter services. This abstracts the IDirect3D9 interface to
 * provide a factory for creating raw and unmanaged IDirect3DDevice intances.
 *
 * @file   adapters_service.cpp
 * @author David Gill
 * @date   29/09/2009
 */

#include <string>

#include <d3d9.h>

#include "loci/video/d3d9/adapters_service.h"
#include "loci/video/d3d9/d3d_error.h"

namespace loci {
namespace video {
namespace d3d9
{
    adapters_service::adapters_service()
    {
        d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
        if (!d3d9)
        {
            throw d3d_error("Direct3D9 failed to initialise.", "Direct3DCreate9");
        }
    }

    adapters_service::~adapters_service()
    {
        d3d9->Release();
    }

    void adapters_service::connect_device(LPDIRECT3DDEVICE9 & device,
                                          D3DPRESENT_PARAMETERS & pp)
    {
        D3DCAPS9 caps;
        d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

        DWORD behaviour_flag = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
        if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        {
            behaviour_flag = D3DCREATE_HARDWARE_VERTEXPROCESSING;
        }

        HRESULT r = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                                       pp.hDeviceWindow,
                                       behaviour_flag,
                                       &pp, &device);
        
        if (FAILED(r))
        {
            std::string /*D3DERR_DEVICELOST*/       code = "device lost";
            if      (r == D3DERR_INVALIDCALL)       code = "invalid call";
            else if (r == D3DERR_NOTAVAILABLE)      code = "not available";
            else if (r == D3DERR_OUTOFVIDEOMEMORY)  code = "out of video memory";
            throw d3d_error(r, "Failed to create a device, " + code + ".", "CreateDevice");
        }
    }
} // namespace d3d9
} // namespace video
} // namespace loci