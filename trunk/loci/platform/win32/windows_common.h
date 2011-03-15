#ifndef LOCI_PLATFORM_WIN32_WINDOWS_COMMON_H_
#define LOCI_PLATFORM_WIN32_WINDOWS_COMMON_H_

/**
 * Wrapper around windows.h to define macros.
 * Rather than #include windows.h directly use this instead as it removes a lot
 * of unneeded bulk from windows.h, most notably the min/max macros. It also
 * wraps the UNICODE macro to LOCI_UNICODE for portable detection of unicode.
 *
 * @file   windows_common.h
 * @author David Gill
 * @date   16/08/2009
 */

#define WIN32_LEAN_AND_MEAN
#include "windows.h"

#ifdef UNICODE
  #define LOCI_UNICODE
#endif

namespace loci {
namespace platform {
namespace win32
{
    typedef TCHAR tchar_t;

    typedef ::LPCTSTR   name_type;
    typedef ::HINSTANCE module_type;
    typedef ::WNDPROC   event_handler_type;
    typedef ::HBRUSH    brush_type;
    typedef ::HCURSOR   cursor_type;
    typedef ::HICON     icon_type;
    typedef ::HMENU     menu_type;

    module_type get_root_module();
    brush_type  get_brush (int brush);
    cursor_type get_cursor(name_type cursor);
    icon_type   get_icon  (name_type icon  );
} // namespace win32
} // namespace platform
} // namespace loci


#endif // LOCI_PLATFORM_WIN32_WINDOWS_COMMON_H_
