#ifndef LOCI_PLATFORM_WIN32_GET_ERROR_H_
#define LOCI_PLATFORM_WIN32_GET_ERROR_H_

/**
 * Win32 error retrieval - this is currently unused.
 * Returns the corresponding system error string for a call to GetLastError.
 *
 * @file   get_error.h
 * @author David Gill
 * @date   24/08/2009
 */

#include <boost/shared_ptr.hpp>
#include "loci/platform/win32/windows_common.h"
#include "loci/platform/tstring.h"

namespace loci {
namespace platform {
namespace win32 {
namespace detail
{
    tstring get_last_error_tstr()
    {
        // retrieve error code and from that the description string
        ::DWORD error_code = ::GetLastError();
        ::LPTSTR msg_buffer = NULL;
        ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                            FORMAT_MESSAGE_FROM_SYSTEM |
                            FORMAT_MESSAGE_IGNORE_INSERTS,
                        NULL,
                        error_code,
                        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                        reinterpret_cast<::LPTSTR>(&msg_buffer),
                        0,
                        NULL);

        // create string from TCHAR buffer
        tstring msg(msg_buffer);
        // deallocate the buffer
        ::LocalFree(msg_buffer);

        return msg;
    }
} // namespace detail
} // namespace win32
} // namespace platform
} // namespace loci

#endif // LOCI_PLATFORM_WIN32_GET_ERROR_H_
