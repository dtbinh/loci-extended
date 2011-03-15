#ifndef LOCI_PLATFORM_WIN32_WINDOW_CLASS_H_
#define LOCI_PLATFORM_WIN32_WINDOW_CLASS_H_

/**
 * Convenient RAII utility class for window classes (wndclassex's).
 * Defines RAII inspired class for window-classes.
 *
 * @file   window_class.h
 * @author David Gill
 * @date   16/08/2009
 */

#include <boost/shared_ptr.hpp>
#include "loci/platform/win32/windows_common.h"
#include "loci/platform/tstring.h"

namespace loci {
namespace platform {
namespace win32
{
    class window_class
    {
    public:
        typedef name_type           handle;

        typedef name_type           name_type;
        typedef module_type         module_type;
        typedef event_handler_type  event_handler_type;
        typedef ::UINT              style_type;
        typedef brush_type          brush_type;
        typedef cursor_type         cursor_type;
        typedef icon_type           icon_type;
        typedef unsigned int        size_type;

        window_class(::ATOM to_own, module_type module);
        window_class(const ::WNDCLASSEX & wcx);
        window_class(const ::WNDCLASS   &  wc);

        window_class(event_handler_type handler);

        window_class(event_handler_type handler,
                     brush_type         background,
                     icon_type          icon);

        window_class(event_handler_type handler,
                     style_type         style,
                     brush_type         background,
                     icon_type          icon);

        window_class(name_type          name,
                     module_type        module,
                     event_handler_type handler,
                     style_type         style,
                     brush_type         background,
                     cursor_type        cursor,
                     icon_type          icon,
                     icon_type          small_icon = NULL,
                     name_type          menu_name  = NULL,
                     size_type          class_extra_bytes  = 0,
                     size_type          window_extra_bytes = 0);

        handle get() const;
        module_type module() const;

    private:
        class window_class_impl;
        boost::shared_ptr<window_class_impl> impl;
    };

    tstring unique_class_name(module_type module);

} // namespace win32
} // namespace platform
} // namespace loci

#endif // LOCI_PLATFORM_WIN32_WINDOW_H_
