#ifndef LOCI_PLATFORM_WIN32_DETAIL_WINDOW_IMPL_H_
#define LOCI_PLATFORM_WIN32_DETAIL_WINDOW_IMPL_H_

/**
 * Pimpl implementation class for windows.
 * Defines data members for windows and any functions that require accessing
 * those variables more than once to ensure only one indirection is ever paid.
 *
 * @file   window_impl.h
 * @author David Gill
 * @date   18/08/2009
 */

#include "loci/platform/win32/windows_common.h"
#include "loci/platform/win32/window.h"

namespace loci {
namespace platform {
namespace win32
{
    class window::window_impl
    {
    public:
        window_impl(::HWND hwnd, const window_class & wc)
            : hwnd(hwnd)
            , wc(wc)
        {
            if (!hwnd)
            {
                throw std::runtime_error(
                    "window_impl error: Creation failure, the received hwnd was"
                    "null.");
            }
        }

       ~window_impl()
        {
            ::DestroyWindow(hwnd);
        }

        void try_close()
        {
            if (::DestroyWindow(hwnd))
            {
                hwnd = NULL;
            }
        }

        window::handle hwnd;
        window_class wc;
    };
} // namespace win32
} // namespace platform
} // namespace loci


#endif // LOCI_PLATFORM_WIN32_DETAIL_WINDOW_IMPL_H_
