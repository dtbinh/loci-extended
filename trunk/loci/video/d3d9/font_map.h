#ifndef LOCI_VIDEO_D3D9_FONT_MAP_H_
#define LOCI_VIDEO_D3D9_FONT_MAP_H_

/**
 * Encapsulates a D3DXFONT, providing RAII.
 * Sets up a font, abstracts certain details of rendering to simplify drawing.
 *
 * @file   font_map.h
 * @author David Gill
 * @date   08/11/2009
 */

#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include <d3d9.h>
#include <d3dx9.h>

#include "loci/platform/tstring.h"

namespace loci {
namespace video {
namespace d3d9
{
    // forward declaration
    class device_services;

    class font_map : boost::noncopyable
    {
    public:
        font_map(const boost::shared_ptr<device_services> & device,
                    const platform::tstring & font_name,
                    unsigned int height,
                    bool bold = false);
        ~font_map();

        void draw_text(const platform::tstring & text, D3DCOLOR colour,
                       unsigned int x, unsigned int y);

        void release();
        void reload();

    private:
        boost::shared_ptr<device_services> device;
        LPD3DXFONT font;
        platform::tstring font_name;
        unsigned int height;
        bool bold;
    };
} // namespace d3d9
} // namespace video
} // namespace loci

#endif // LOCI_VIDEO_D3D9_FONT_MAP_H_