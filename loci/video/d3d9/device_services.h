#ifndef LOCI_VIDEO_D3D9_DEVICE_H_
#define LOCI_VIDEO_D3D9_DEVICE_H_

/**
 * Encapsulates a d3d9 device.
 * Provides a RAII wrapper around a d3d9 device and provides more convenient
 * member functions to perform common tasks.
 *
 * @file   device_services.h
 * @author David Gill
 * @date   30/09/2009
 */

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>

#include <d3d9.h>
#include <d3dx9.h>

#include "loci/platform/win32/windows_common.h"
#include "loci/video/d3d9/display_settings.h"

namespace loci {
namespace video {
namespace d3d9
{
    // forward declarations
    class adapters_service;
    class display_settings;

    class device_services : boost::noncopyable
    {
    public:
        device_services(const boost::shared_ptr<adapters_service> & adapters,
                        HWND window,
                        const display_settings & config);
        ~device_services();

        // accessors
        unsigned int backbuffer_width();
        unsigned int backbuffer_height();

        IDirect3DDevice9 * get() { return device; }

        // device resetting
        bool cooperative(bool accept_not_reset);
        bool try_reset(const display_settings & config);

        // scene
        void begin_scene();
        void end_scene();

        void clear_backbuffers(DWORD clear_colour);
        bool present();

        // drawing
        void draw(D3DPRIMITIVETYPE topology,
                  unsigned int count,
                  unsigned int start_offset);

        // resource allocation
        void allocate_vertex_buffer(LPDIRECT3DVERTEXBUFFER9 & buffer,
                                    unsigned int length,
                                    bool dynamic,
                                    DWORD fvf,
                                    D3DPOOL pool);

        // pipeline state
        void bind_render_state(D3DRENDERSTATETYPE state, DWORD value);
        void bind_texture_stage_state(DWORD stage, D3DTEXTURESTAGESTATETYPE type, DWORD value);
        void bind_sampler_state(DWORD sampler, D3DSAMPLERSTATETYPE type, DWORD value);

        void bind_vertex_buffer(LPDIRECT3DVERTEXBUFFER9 buffer,
                                unsigned int stride);

        void bind_texture(LPDIRECT3DTEXTURE9 texture, DWORD sampler = 0);

        void bind_fvf(DWORD fvf);
        
        void bind_transform(D3DTRANSFORMSTATETYPE state,
                            const D3DMATRIX & matrix);

        void enable_light(unsigned int light, bool enabled);
        void configure_light(unsigned int light, const D3DLIGHT9 & descriptor);

        void reset_material(const D3DMATERIAL9 & material);
    
    private:
        void set_presentation_parameters(const D3DPRESENT_PARAMETERS & pp);

    private:
        boost::shared_ptr<adapters_service> adapters;
        LPDIRECT3DDEVICE9 device;

        // presentation parameters
        HWND window;
        unsigned int width, height;
    };

    boost::shared_ptr<device_services> create_shared_device(
        HWND output_window,
        const display_settings & config = display_settings());
} // namespace d3d9
} // namespace video
} // namespace loci

#endif // LOCI_VIDEO_D3D9_DEVICE_H_