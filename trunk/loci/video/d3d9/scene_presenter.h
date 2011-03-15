#ifndef LOCI_VIDEO_D3D9_SCENE_PRESENTER_H_
#define LOCI_VIDEO_D3D9_SCENE_PRESENTER_H_

/**
 * Provides functionality for rendering a scene on a per-frame basis.
 * Wraps device functionality for beginning, presenting
 * and ending a scene rendering.
 *
 * @file   scene_presenter.h
 * @author David Gill
 * @date   01/10/2009
 */

#include <boost/shared_ptr.hpp>

#include <d3d9.h>

namespace loci {
namespace video {
namespace d3d9
{
    // forward declaration
    class device_services;

    class scene_presenter
    {
    public: // public members
        scene_presenter(const boost::shared_ptr<device_services> & device,
                        bool begin_frame_on_construction = false);
        ~scene_presenter();

        void clear(DWORD clear_colour = D3DCOLOR_XRGB(0, 0, 0));
        void begin_frame(DWORD clear_colour = D3DCOLOR_XRGB(0, 0, 0));
        bool submit_frame();

    private:
        void end_frame();

    private:
        boost::shared_ptr<device_services> device;
        bool begun;
    };
} // namespace d3d9
} // namespace video
} // namespace loci

#endif // LOCI_VIDEO_D3D9_SCENE_PRESENTER_H_