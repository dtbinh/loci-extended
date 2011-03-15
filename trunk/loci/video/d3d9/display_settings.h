#ifndef LOCI_VIDEO_D3D9_DISPLAY_SETTINGS_H_
#define LOCI_VIDEO_D3D9_DISPLAY_SETTINGS_H_

/**
 * Groups various display presentation parameters together.
 * Encapsulates relevant diaplay settings, essentially acting as a nicer version
 * of d3d9's present parameters structure for configuring a device.
 *
 * @file   display_settings.h
 * @author David Gill
 * @date   30/09/2009
 */

namespace loci {
namespace video {
namespace d3d9
{
    class display_settings
    {
    public:
        display_settings();
        display_settings(unsigned int screen_width,
                         unsigned int screen_height,
                         bool fullscreen);

        unsigned int screen_width() const;
        unsigned int screen_height() const;

        bool fullscreen() const;

    private:
        unsigned int screen_width_;
        unsigned int screen_height_;
        bool fullscreen_;
    };
} // namespace d3d9
} // namespace video
} // namespace loci

#endif // LOCI_VIDEO_D3D9_DISPLAY_SETTINGS_H_