/**
 * Implementation of display_settings.
 * Encapsulates various settings used for comfiguring a d3d device.
 *
 * @file   adapters_service.cpp
 * @author David Gill
 * @date   30/09/2009
 */

#include "loci/video/d3d9/display_settings.h"

namespace loci {
namespace video {
namespace d3d9
{
    display_settings::display_settings()
        : screen_width_(640)
        , screen_height_(480)
        , fullscreen_(false)
    {
    }

    display_settings::display_settings(unsigned int screen_width,
                                       unsigned int screen_height,
                                       bool fullscreen)
        : screen_width_(screen_width)
        , screen_height_(screen_height)
        , fullscreen_(fullscreen)
    {
    }

    unsigned int display_settings::screen_width() const
    {
        return screen_width_;
    }

    unsigned int display_settings::screen_height() const
    {
        return screen_height_;
    }

    bool display_settings::fullscreen() const
    {
        return fullscreen_;
    }
} // namespace d3d9
} // namespace video
} // namespace loci