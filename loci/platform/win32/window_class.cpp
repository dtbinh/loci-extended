/**
 * Implementation of the window_class.
 * Implements the window_class class.
 *
 * @file   window_class.cpp
 * @author David Gill
 * @date   18/08/2009
 */

#include <boost/make_shared.hpp>

#include "loci/platform/win32/window_class.h"
#include "loci/platform/win32/windows_common.h"
#include "loci/platform/win32/detail/window_class_impl.h"
#include "loci/platform/tstring.h"
#include "loci/platform/tsstream.h"

namespace loci {
namespace platform {
namespace win32
{
    namespace // anonymous
    {
        ATOM registrate(const WNDCLASSEX & wcx)
        {
            return ::RegisterClassEx(&wcx);
        }

        ATOM registrate(const WNDCLASS & wc)
        {
            return ::RegisterClass(&wc);
        }

        ATOM registrate(
            window_class::event_handler_type handler,
            window_class::brush_type         background         = get_brush(BLACK_BRUSH),
            window_class::icon_type          icon               = get_icon(IDI_APPLICATION),
            window_class::style_type         style              = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
            window_class::name_type          name               = unique_class_name(get_root_module()).c_str(),
            window_class::module_type        module             = get_root_module(),
            window_class::cursor_type        cursor             = get_cursor(IDC_ARROW),
            window_class::icon_type          small_icon         = NULL,
            window_class::name_type          menu_name          = NULL,
            window_class::size_type          class_extra_bytes  = 0,
            window_class::size_type          window_extra_bytes = 0)
        {
            WNDCLASSEX wcx =
            {
                sizeof(WNDCLASSEX), style, handler, class_extra_bytes,
                window_extra_bytes, module, icon, cursor, background,
                menu_name, name, small_icon
            };

            return registrate(wcx);
        }
    } // anonymous namespace

    window_class::window_class(ATOM to_own, module_type module)
        : impl(
            boost::make_shared<window_class_impl>(
                to_own,
                module))
    {
    }

    window_class::window_class(const WNDCLASSEX & wcx)
        : impl(
            boost::make_shared<window_class_impl>(
                registrate(wcx),
                wcx.hInstance))
    {
    }

    window_class::window_class(const WNDCLASS & wc)
        : impl(
            boost::make_shared<window_class_impl>(
                registrate(wc),
                wc.hInstance))
    {
    }

    window_class::window_class(event_handler_type handler)
        : impl(
            boost::make_shared<window_class_impl>(
                registrate(handler),
                get_root_module()))
    {
    }

    window_class::window_class(event_handler_type handler,
                               brush_type         background,
                               icon_type          icon)
        : impl(
            boost::make_shared<window_class_impl>(
                registrate(handler, background, icon),
                get_root_module()))
    {
    }

    window_class::window_class(event_handler_type handler,
                               style_type         style,
                               brush_type         background,
                               icon_type          icon)
        : impl(
            boost::make_shared<window_class_impl>(
                registrate(handler, background, icon, style),
                get_root_module()))
    {
    }

    window_class::window_class(name_type          name,
                               module_type        module,
                               event_handler_type handler,
                               style_type         style,
                               brush_type         background,
                               cursor_type        cursor,
                               icon_type          icon,
                               icon_type          small_icon,
                               name_type          menu_name,
                               size_type          class_extra_bytes,
                               size_type          window_extra_bytes)
        : impl(
            boost::make_shared<window_class_impl>(
                registrate(handler, background, icon, style, name, module,
                           cursor, small_icon, menu_name, class_extra_bytes,
                           window_extra_bytes),
                module))
    {
    }

    window_class::handle window_class::get() const
    {
        return impl->atom;
    }

    window_class::module_type window_class::module() const
    {
        return impl->hinstance;
    }

    tstring unique_class_name(module_type module)
    {
        static unsigned long uid = 0;
        
        ::DWORD initial_error_state = ::GetLastError();

        tstring name;
        WNDCLASS wc;
        for(;;)
        {
            otstringstream ss;
            ss << uid;
            name = ss.str();
            if(!(::GetClassInfo(module, name.c_str(), &wc)))
            {
                ::SetLastError(initial_error_state);
                return name;
            }
            ++uid;
        }
    }
} // namespace win32
} // namespace platform
} // namespace loci