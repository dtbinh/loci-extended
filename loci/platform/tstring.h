#ifndef LOCI_PLATFORM_TSTRING_H_
#define LOCI_PLATFORM_TSTRING_H_

/**
 * Defines tstring as a unicode-portable std::basic_string type.
 * Uses tchar_t as a the character type for std::basic_string.
 *
 * @file   tstring.h
 * @author David Gill
 * @date   21/08/2009
 */

#include <string>
#include "loci/platform/tchar.h"

namespace loci {
namespace platform
{
    typedef std::basic_string<tchar_t> tstring;

    std::string narrow(const std::wstring & s);
    std::wstring widen(const std::string & s);

    // overloads to just copy the value through
    inline std::string narrow(const std::string & s) { return s; }
    //inline std::string narrow(const char * s) { return s; }
    inline std::wstring widen(const std::wstring & s) { return s; }
    //inline std::wstring widen(const wchar_t * s) { return s; }

    tstring to_tstr(const std::string & s);
    tstring to_tstr(const std::wstring & s);
} // namespace platform
} // namespace loci

#endif // LOCI_PLATFORM_TSTRING_H_
