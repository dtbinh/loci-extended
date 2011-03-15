/**
 * Implementation of unicode narrow and widen functions.
 * Implements the narrow and widen functions for std::string and std::wstring
 * using the std::ctype facet.
 *
 * @file   tstring.cpp
 * @author David Gill
 * @date   22/08/2009
 */

#include <string>
#include <locale>
#include <vector>

#include "loci/platform/tstring.h"
#include "loci/platform/win32/windows_common.h"

namespace loci {
namespace platform
{
    std::string narrow(const std::wstring & s)
    {
        // get c-string-style information to work with
        const wchar_t * wstr = s.c_str();
        const std::wstring::size_type length = s.length();

        // contiguous buffer to hold transformed characters
        std::vector<char> buffer(length); 

        // transform using the ctype facet and default locale
        typedef std::ctype<wchar_t> facet_type;
        std::use_facet<facet_type>(std::locale()).narrow(
            wstr,
            wstr + length,
            '?',
            &buffer.front());

        // return a string from the buffer
        return std::string(buffer.begin(), buffer.end());
    }

    std::wstring widen(const std::string & s)
    {
        // get c-string-style information to work with
        const char * str = s.c_str();
        const std::string::size_type length = s.length();

        // contiguous buffer to hold transformed characters
        std::vector<wchar_t> buffer(length);

        // transform using the ctype facet and default locale
        typedef std::ctype<wchar_t> facet_type;
        std::use_facet<facet_type>(std::locale()).widen(
            str,
            str + length,
            &buffer.front());

        // return a string from the buffer
        return std::wstring(buffer.begin(), buffer.end());
    }

    tstring to_tstr(const std::string & s)
    {
#ifdef LOCI_UNICODE
        return widen(s);
#else
        return s;
#endif
    }

    tstring to_tstr(const std::wstring & s)
    {
#ifdef LOCI_UNICODE
        return s;
#else
        return narrow(s);
#endif
    }
} // namespace platform
} // namespace loci