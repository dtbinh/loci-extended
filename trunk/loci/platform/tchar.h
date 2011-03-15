#ifndef LOCI_PLATFORM_TCHAR_H_
#define LOCI_PLATFORM_TCHAR_H_

/**
 * Lift tchar_t into the loci::platform namespace.
 * Removes the platform specific namespace from tchar_t and define the LOCI_STR
 * macro.
 *
 * @file   tchar.h
 * @author David Gill
 * @date   21/08/2009
 */

#include "loci/platform/win32/windows_common.h"

#define LOCI_TSTR(str) TEXT(str)

namespace loci {
namespace platform
{
    using loci::platform::win32::tchar_t;
} // namespace platform
} // namespace loci

#endif // LOCI_PLATFORM_TCHAR_H_
