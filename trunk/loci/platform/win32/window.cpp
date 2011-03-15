/**
 * Implementation of the window class.
 * The window acts as the handle part of the handle-body idiom; most of the
 * implementation still resides here, the body/pimpl part is fairly dumb.
 * The idiom was used as an optimisation since it would normally hold two
 * shared_ptr's, one to the hwnd and one wraped in the window_class; now the
 * cost of copying is reduced to a single shared_ptr copy.
 *
 * @file   window.cpp
 * @author David Gill
 * @date   17/08/2009
 */

#include <stdexcept>

#include <boost/make_shared.hpp>

#include "loci/platform/win32/window.h"
#include "loci/platform/win32/window_class.h"
#include "loci/platform/win32/windows_common.h"
#include "loci/platform/win32/detail/window_impl.h"


namespace loci {
namespace platform {
namespace win32
{
    namespace // anonymous
    {
        template <class PrivateImplT>
        boost::shared_ptr<PrivateImplT> make_impl_helper(
            const window_class & wc,
            const window::caption_type & title,
            window::param_ptr_type parameter,
            window::size_type width          = CW_USEDEFAULT,
            window::size_type height         = 0,
            bool adjust_to_client_area       = false,
            window::style_type style         = WS_OVERLAPPEDWINDOW,
            window::position_type x          = CW_USEDEFAULT,
            window::position_type y          = 0,
            window::menu_type menu           = NULL,
            window::handle parent            = NULL)
        {
            if (adjust_to_client_area)
            {
                ::RECT sides = { 0, 0, width, height };
                const ::BOOL is_menu_occupying_space = menu != NULL;
                ::AdjustWindowRect(&sides, style, is_menu_occupying_space);
                width  = sides.right - sides.left;
                height = sides.bottom - sides.top;
            }

            return boost::make_shared<PrivateImplT>(
                ::CreateWindow(wc.get(),
                               title.c_str(), style, x, y,  width, height,
                               parent, menu, wc.module(), parameter),
                wc);
        }
    } // anonymous namespace

    window::window(::HWND to_own, const window_class & wndclass)
        : impl(boost::make_shared<window_impl>(to_own, wndclass))
    {
    }

    window::window(const caption_type & title,
                   event_handler_type handler,
                   param_ptr_type parameter)
        : impl(
            make_impl_helper<window_impl>(
                window_class(handler),
                title, parameter))
    {
    }

    window::window(const caption_type & title,
                   const window_class & wndclass,
                   param_ptr_type parameter)
        : impl(
            make_impl_helper<window_impl>(
                wndclass,
                title, parameter))
    {
    }

    window::window(const caption_type & title,
                   event_handler_type handler,
                   size_type width, size_type height,
                   bool adjust_to_client_area,
                   param_ptr_type parameter)
        : impl(
            make_impl_helper<window_impl>(
                window_class(handler),
                title, parameter, width, height, adjust_to_client_area))
    {
    }

    window::window(const caption_type & title,
                   event_handler_type handler,
                   size_type width, size_type height,
                   brush_type background,
                   icon_type icon,
                   bool adjust_to_client_area,
                   param_ptr_type parameter)
        : impl(
            make_impl_helper<window_impl>(
                window_class(handler, background, icon),
                title, parameter, width, height, adjust_to_client_area))
    {
    }

    window::window(const caption_type & title,
                   const window_class & wndclass,
                   style_type style,
                   size_type width, size_type height,
                   bool adjust_to_client_area,
                   param_ptr_type parameter)
        : impl(
            make_impl_helper<window_impl>(
                wndclass,
                title, parameter, width, height, adjust_to_client_area, style))
    {
    }

    window::window(const caption_type & title,
                   event_handler_type handler,
                   style_type style,
                   size_type width, size_type height,
                   brush_type background,
                   icon_type icon,
                   bool adjust_to_client_area,
                   param_ptr_type parameter)
        : impl(
            make_impl_helper<window_impl>(
                window_class(handler, background, icon),
                title, parameter, width, height, adjust_to_client_area, style))
    {
    }

    window::window(const caption_type & title,
                   event_handler_type handler,
                   class_style_type class_style,
                   style_type style,
                   position_type x, position_type y,
                   size_type width, size_type height,
                   brush_type background,
                   icon_type icon,
                   bool adjust_to_client_area,
                   param_ptr_type parameter)
        : impl(
            make_impl_helper<window_impl>(
                window_class(handler, class_style, background, icon),
                title, parameter, width, height, adjust_to_client_area,
                style, x, y))
    {
    }

    window::window(const caption_type & title,
                   const window_class & wndclass,
                   style_type style,
                   position_type x, position_type y,
                   size_type width, size_type height,
                   bool adjust_to_client_area,
                   menu_type menu,
                   handle parent,
                   param_ptr_type parameter)
       : impl(
            make_impl_helper<window_impl>(
                wndclass,
                title, parameter, width, height, adjust_to_client_area,
                style, x, y, menu, parent))
    {
    }

    bool window::visible() const
    {
        return ::IsWindowVisible(get()) != 0;
    }

    void window::visible(bool show)
    {
        ::ShowWindow(get(), show ? (SW_SHOW) : (SW_HIDE));
    }

    bool window::active() const
    {
        return get() != NULL;
    }

    void window::close()
    {
        impl->try_close();
    }

    void window::data_ptr(void * data)
    {
        ::SetWindowLongPtr(get(), GWLP_USERDATA, (LONG)(LONG_PTR)(data));
    }

    void * window::data_ptr()
    {
        return (void *)(LONG_PTR)(::GetWindowLongPtr(get(), GWLP_USERDATA));
    }

    window::handle window::get() const
    {
        return impl->hwnd;
    }

    window_class window::kind() const
    {
        return impl->wc;
    }
} // namespace win32
} // namespace platform
} // namespace loci