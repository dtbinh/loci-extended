#ifndef LOCI_PLATFORM_WIN32_WINDOW_H_
#define LOCI_PLATFORM_WIN32_WINDOW_H_

/**
 * Convenient RAII utility class for windows.
 * Defines RAII inspired class for actual windows.
 *
 * @file   window.h
 * @author David Gill
 * @date   15/08/2009
 */

#include <boost/shared_ptr.hpp>
#include "loci/platform/win32/windows_common.h"
#include "loci/platform/win32/window_class.h"
#include "loci/platform/tstring.h"

namespace loci {
namespace platform {
namespace win32
{   
    class window
    {
    public:
        typedef ::HWND       handle;

        typedef tstring      caption_type;
        typedef ::DWORD      style_type;
        typedef int          position_type;
        typedef unsigned int size_type;
        typedef menu_type    menu_type;
        typedef module_type  module_type;
        typedef ::LPVOID     param_ptr_type;

        typedef window_class::name_type          name_type;
        typedef window_class::event_handler_type event_handler_type;
        typedef window_class::style_type         class_style_type;
        typedef window_class::brush_type         brush_type;
        typedef window_class::cursor_type        cursor_type;
        typedef window_class::icon_type          icon_type;

        window(::HWND to_own, const window_class & wndclass);

        window(const caption_type & title,
               event_handler_type handler,
               param_ptr_type parameter = NULL);

        window(const caption_type & title,
               const window_class & wndclass,
               param_ptr_type parameter = NULL);

        window(const caption_type & title,
               event_handler_type handler,
               size_type width, size_type height,
               bool adjust_to_client_area = true,
               param_ptr_type parameter = NULL);

        window(const caption_type & title,
               event_handler_type handler,
               size_type width, size_type height,
               brush_type background,
               icon_type icon,
               bool adjust_to_client_area = true,
               param_ptr_type parameter = NULL);

        window(const caption_type & title,
               const window_class & wndclass,
               style_type style,
               size_type width, size_type height,
               bool adjust_to_client_area = true,
               param_ptr_type parameter = NULL);

        window(const caption_type & title,
               event_handler_type handler,
               style_type style,
               size_type width, size_type height,
               brush_type background,
               icon_type icon,
               bool adjust_to_client_area = true,
               param_ptr_type parameter = NULL);

        window(const caption_type & title,
               event_handler_type handler,
               class_style_type class_style,
               style_type style,
               position_type x, position_type y,
               size_type width, size_type height,
               brush_type background,
               icon_type icon,
               bool adjust_to_client_area = true,
               param_ptr_type parameter = NULL);

        window(const caption_type & title,
               const window_class & wndclass,
               style_type style,
               position_type x, position_type y,
               size_type width, size_type height,
               bool adjust_to_client_area = true,
               menu_type menu           = NULL,
               handle parent            = NULL,
               param_ptr_type parameter = NULL);
        
        bool visible() const;
        void visible(bool show);

        bool active() const;
        void close();

        void data_ptr(void * data);
        void * data_ptr();

        handle get() const;
        window_class kind() const;

    private:
        class window_impl;
        boost::shared_ptr<window_impl> impl;
    };

    inline void show(window & w) { w.visible(true); }
    inline void hide(window & w) { w.visible(false); }
} // namespace win32
} // namespace platform
} // namespace loci

#endif // LOCI_PLATFORM_WIN32_WINDOW_H_
