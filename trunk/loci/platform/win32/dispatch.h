#ifndef LOCI_PLATFORM_WIN32_DISPATCH_H_
#define LOCI_PLATFORM_WIN32_DISPATCH_H_

/**
 * The message pump for dispatching messages to window handlers.
 * Defines the message pump.
 *
 * @file   dispatch.h
 * @author David Gill
 * @date   24/08/2009
 */

#include "loci/platform/win32/windows_common.h"

namespace loci {
namespace platform {
namespace win32
{
    inline bool dispatch_messages()
    {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) { return true; }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return false;
    }
} // namespace win32
} // namespace platform
} // namespace loci

#endif // LOCI_PLATFORM_WIN32_DISPATCH_H_
