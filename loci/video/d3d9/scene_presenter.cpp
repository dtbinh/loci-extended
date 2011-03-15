/**
 * Implementation of scene_presenter.
 * Provides RAII support for scene presenting.
 *
 * @file   scene_presenter.cpp
 * @author David Gill
 * @date   01/10/2009
 */

#include <boost/shared_ptr.hpp>

#include "loci/video/d3d9/scene_presenter.h"
#include "loci/video/d3d9/device_services.h"

namespace loci {
namespace video {
namespace d3d9
{
    scene_presenter::scene_presenter(const boost::shared_ptr<device_services> & device,
                                     bool begin_frame_on_construction)
        : device(device)
        , begun(begin_frame_on_construction)
    {
    }

    scene_presenter::~scene_presenter()
    {
        end_frame();
    }

    void scene_presenter::clear(DWORD clear_colour)
    {
        device->clear_backbuffers(clear_colour);
    }

    void scene_presenter::begin_frame(DWORD clear_colour)
    {
        if (begun) { return; }
        begun = true;
        device->begin_scene();
        clear(clear_colour);
    }

    bool scene_presenter::submit_frame()
    {
        end_frame();
        return device->present();
    }

    void scene_presenter::end_frame()
    {
        if (!begun) { return; }
        begun = false;
        device->end_scene();
    }
} // namespace d3d9
} // namespace video
} // namespace loci