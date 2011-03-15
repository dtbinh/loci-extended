/**
 * Implementation of windows_common functions.
 * Implements the get_XXX functions.
 *
 * @file   windows_common.cpp
 * @author David Gill
 * @date   21/08/2009
 */

#include "loci/platform/win32/windows_common.h"
#include "loci/platform/tstring.h"
#include <string>

namespace loci {
namespace platform {
namespace win32
{
    module_type get_root_module()
    {
        return ::GetModuleHandle(NULL);
    }

    brush_type get_brush(int brush)
    {
        return static_cast<::HBRUSH>(::GetStockObject(brush));
    }

    cursor_type get_cursor(name_type cursor)
    {
        return ::LoadCursor(NULL, cursor);
    }

    icon_type get_icon(name_type icon)
    {
        // deprecated for LoadImage
        // TODO: replace with LoadImage.
        return ::LoadIcon(NULL, icon);
    }
} // namespace win32
} // namespace platform
} // namespace loci