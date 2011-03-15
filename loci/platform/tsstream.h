#ifndef LOCI_PLATFORM_TSSTREAM_H_
#define LOCI_PLATFORM_TSSTREAM_H_

/**
 * Defines string-stream types as unicode-portable std::[i|o]stringstream types.
 * Uses tchar_t as a the character type for std::[i|o]stringstream.
 *
 * @file   tsstream.h
 * @author David Gill
 * @date   21/08/2009
 */

#include <sstream>
#include "loci/platform/tchar.h"

namespace loci {
namespace platform
{
    typedef std::basic_istringstream<tchar_t> itstringstream;
    typedef std::basic_ostringstream<tchar_t> otstringstream;
    typedef  std::basic_stringstream<tchar_t> tstringstream;
} // namespace platform
} // namespace loci

#endif // LOCI_PLATFORM_TSSTREAM_H_
