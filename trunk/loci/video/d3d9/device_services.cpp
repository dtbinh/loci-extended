/**
 * Implementation of device_services.
 * Provides a RAII wrapper around a d3d9 device and provides more convenient
 * member functions than a raw device instance does.
 *
 * @file   device_services.cpp
 * @author David Gill
 * @date   30/09/2009
 */

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <d3d9.h>
#include <d3dx9.h>

#include "loci/platform/win32/windows_common.h"
#include "loci/video/d3d9/device_services.h"
#include "loci/video/d3d9/adapters_service.h"
#include "loci/video/d3d9/display_settings.h"
#include "loci/video/d3d9/d3d_error.h"

namespace loci {
namespace video {
namespace d3d9
{
    // connect with a target device to the default adapter
    boost::shared_ptr<device_services> create_shared_device(HWND output_window,
                                                            const display_settings & config)
    {
        return boost::make_shared<device_services>(
            boost::make_shared<adapters_service>(),
            output_window,
            config);
    }

    namespace // anonymous
    {
        D3DPRESENT_PARAMETERS make_present_parameters(HWND window,
                                                      const display_settings & config)
        {
            D3DFORMAT buffer_format =
                config.fullscreen() ? D3DFMT_A8R8G8B8 : D3DFMT_UNKNOWN;

            UINT refresh_hz =
                config.fullscreen() ? 60 : 0;

            D3DPRESENT_PARAMETERS pp =
            {
                // backbuffer
                config.screen_width(),               // BackBufferWidth
                config.screen_height(),              // BackBufferHeight
                buffer_format,                       // BackBufferFormat
                1,                                   // BackBufferCount

                // multisample and swapping
                D3DMULTISAMPLE_NONE,                 // MultiSampleType
                0,                                   // MultiSampleQuality
                D3DSWAPEFFECT_DISCARD,               // SwapEffect
                
                // window
                window,                              // hDeviceWindow
                !config.fullscreen(),                // Windowed

                // depth stencil
                TRUE,                                // EnableAutoDepthStencil
                D3DFMT_D16,                          // AutoDepthStencilFormat

                // frame-by-frame controls
                D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL, // Flags
                refresh_hz,                          // Fullscreen_refreshRateInHz
                D3DPRESENT_INTERVAL_DEFAULT          // PresentationInterval
            };

            return pp;
        }
    } // anonymous namespace

    device_services::device_services(const boost::shared_ptr<adapters_service> & adapters, 
                                     HWND window,
                                     const display_settings & config)
        : adapters(adapters)
    {
        D3DPRESENT_PARAMETERS pp = make_present_parameters(window, config);
        set_presentation_parameters(pp);

        adapters->connect_device(device, pp);
    }

    device_services::~device_services()
    {
        device->Release();
    }

    unsigned int device_services::backbuffer_width()
    {
        return width;
    }

    unsigned int device_services::backbuffer_height()
    {
        return height;
    }

    bool device_services::cooperative(bool accept_not_reset)
    {
        HRESULT r = device->TestCooperativeLevel();

        if (r == D3DERR_DRIVERINTERNALERROR)
        {
            throw d3d_error(r, "Internal driver error", "IDirect3DDevice9::TestCooperativeLevel");
        }

        return r == D3D_OK ||
               (accept_not_reset && r == D3DERR_DEVICENOTRESET);
    }

    bool device_services::try_reset(const display_settings & config)
    {
        D3DPRESENT_PARAMETERS pp = make_present_parameters(window, config);
        set_presentation_parameters(pp);

        HRESULT r = device->Reset(&pp);

        if (r == D3DERR_DRIVERINTERNALERROR)
        {
            throw d3d_error(r, "Internal driver error", "IDirect3DDevice9::Reset");
        }

        return SUCCEEDED(r);
    }

    void device_services::begin_scene()
    {
        HRESULT r = device->BeginScene();
        check_failure(r, "Failed to begin scene", "IDirect3DDevice9::BeginScene"); 
    }

    void device_services::end_scene()
    {
        HRESULT r = device->EndScene();
        check_failure(r, "Failed to end scene", "IDirect3DDevice9::EndScene");
    }

    void device_services::clear_backbuffers(DWORD clear_colour)
    {
        HRESULT r = device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                                  clear_colour, 1.0f, 0);

        check_failure(r, "Failed to clear backbuffers", "IDirect3DDevice9::Clear");
    }

    bool device_services::present()
    {
        return SUCCEEDED(device->Present(NULL, NULL, NULL, NULL));
    }

    void device_services::draw(D3DPRIMITIVETYPE topology,
                               unsigned int count,
                               unsigned int start_offset)
    {
        HRESULT r = device->DrawPrimitive(topology, start_offset, count);
        check_failure(r, "Failed to draw", "IDirect3DDevice9::DrawPrimitive");
    }

    void device_services::allocate_vertex_buffer(LPDIRECT3DVERTEXBUFFER9 & buffer,
                                                 unsigned int length,
                                                 bool dynamic,
                                                 DWORD fvf,
                                                 D3DPOOL pool)
    {
        HRESULT r = device->CreateVertexBuffer(static_cast<UINT>(length),
                                               dynamic ? D3DUSAGE_DYNAMIC : 0,
                                               fvf, pool, &buffer, NULL);

        check_failure(r, "Failed to create vertex buffer", "IDirect3DDevice9::CreateVertexBuffer");
    }

    void device_services::bind_transform(D3DTRANSFORMSTATETYPE state,
                                         const D3DMATRIX & matrix)
    {
        HRESULT r = device->SetTransform(state, &matrix);
        check_failure(r, "Failed to set transform", "IDirect3DDevice9::SetTransform");
    }

    void device_services::bind_render_state(D3DRENDERSTATETYPE state,
                                            DWORD value)
    {
        HRESULT r = device->SetRenderState(state, value);
        check_failure(r, "Failed to set render state", "IDirect3DDevice9::SetRenderState");
    }

    void device_services::bind_texture_stage_state(DWORD stage, D3DTEXTURESTAGESTATETYPE type, DWORD value)
    {
        HRESULT r = device->SetTextureStageState(stage, type, value);
        check_failure(r, "Failed to set texture stage state", "IDirect3DDevice9::SetTextureStageState");
    }

    void device_services::bind_sampler_state(DWORD sampler, D3DSAMPLERSTATETYPE type, DWORD value)
    {
        HRESULT r = device->SetSamplerState(sampler, type, value);
        check_failure(r, "Failed to set sample state", "IDirect3DDevice9::SetSamplerState");
    }

    void device_services::bind_fvf(DWORD fvf)
    {
        HRESULT r = device->SetFVF(fvf);
        check_failure(r, "Failed to set FVF", "IDirect3DDevice9::SetFVF");
    }

    void device_services::bind_vertex_buffer(LPDIRECT3DVERTEXBUFFER9 buffer,
                                             unsigned int stride)
    {
        HRESULT r = device->SetStreamSource(0, buffer, 0, stride);
        check_failure(r, "Failed to set FVF", "IDirect3DDevice9::SetStreamSource");
    }

    void device_services::bind_texture(LPDIRECT3DTEXTURE9 texture, DWORD sampler)
    {
        HRESULT r = device->SetTexture(sampler, texture);
        check_failure(r, "Failed to set Texture", "IDirect3DDevice9::SetTexture");
    }

    void device_services::enable_light(unsigned int light, bool enabled)
    {
        HRESULT r = device->LightEnable(light, enabled);
        check_failure(r, "Failed to enable light", "IDirect3DDevice9::LightEnable");
    }

    void device_services::configure_light(unsigned int light, const D3DLIGHT9 & descriptor)
    {
        HRESULT r = device->SetLight(light, &descriptor);
        check_failure(r, "Failed to configure light properties", "IDirect3DDevice9::SetLight");
    }

    void device_services::reset_material(const D3DMATERIAL9 & material)
    {
        HRESULT r = device->SetMaterial(&material);
        check_failure(r, "Failed to set a material", "IDirect3DDevice9::SetMaterial");
    }

    void device_services::set_presentation_parameters(const D3DPRESENT_PARAMETERS & pp)
    {
        window = pp.hDeviceWindow;
        width = pp.BackBufferWidth;
        height = pp.BackBufferHeight;
    }
} // namespace d3d9
} // namespace video
} // namespace loci