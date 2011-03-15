#ifndef LOCI_PLATFORM_TSTREAM_H_
#define LOCI_PLATFORM_TSTREAM_H_

/**
 * Defines stream types as unicode-portable std::[i|o|io]stream types.
 * Uses tchar_t as a the character type for std::[i|o|io]stream.
 *
 * @file   tstream.h
 * @author David Gill
 * @date   21/08/2009
 */

#include <ostream>
#include <istream>
#include "loci/platform/tchar.h"

namespace loci {
namespace platform
{
    typedef  std::basic_istream<tchar_t> itstream;
    typedef  std::basic_ostream<tchar_t> otstream;
    typedef std::basic_iostream<tchar_t> iotstream;
} // namespace platform
} // namespace loci

#endif // LOCI_PLATFORM_TSTREAM_H_
