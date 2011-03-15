/**
 * Implementation of a font_drawer.
 * Encapsulates a D3DXFONT, providing RAII.
 *
 * @file   font_map.cpp
 * @author David Gill
 * @date   08/11/2009
 */

#include <boost/shared_ptr.hpp>

#include <d3d9.h>
#include <d3dx9.h>

#include "loci/platform/tstring.h"

#include "loci/video/d3d9/font_map.h"
#include "loci/video/d3d9/device_services.h"
#include "loci/video/d3d9/d3d_error.h"

namespace loci {
namespace video {
namespace d3d9
{
    font_map::font_map(const boost::shared_ptr<device_services> & device,
                             const platform::tstring & font_name,
                             unsigned int height,
                             bool bold)
        : device(device)
        , font_name(font_name)
        , height(height)
        , bold(bold)
    {
        reload();
    }

    font_map::~font_map()
    {
        release();
    }

    void font_map::draw_text(const platform::tstring & text, D3DCOLOR colour,
                             unsigned int x, unsigned int y)
    {
        RECT bounds =
        {
            x, y,
            x + 0, // not needed for left-align text
            0 // height not known yet
        };

        // calculate height
        INT height = font->DrawText(NULL, text.c_str(), -1,
                                    &bounds, DT_CALCRECT, colour);

        bounds.bottom = y + height;

        // render
        font->DrawText(NULL, text.c_str(), -1, &bounds, 0, colour);
    }

    void font_map::release()
    {
        font->Release();
        font = 0;
    }

    void font_map::reload()
    {
        DWORD weight = bold ? FW_BOLD : FW_DONTCARE;
        HRESULT r = D3DXCreateFont(device->get(), height, 0, weight, 0, FALSE,
                                   DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                                   DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                                   font_name.c_str(), &font);

        check_failure(r, "Failed to create font", "D3DXCreateFont");
    }
} // namespace d3d9
} // namespace video
} // namespace loci