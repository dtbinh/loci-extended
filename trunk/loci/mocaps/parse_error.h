#ifndef LOCI_MOCAPS_PARSE_ERROR_H_
#define LOCI_MOCAPS_PARSE_ERROR_H_

/**
 * An exception to be thrown upon error parsing a mocap.
 * Upon failure parsing a mocap parsers should throw parse_error which derives
 * from std::runtime_error.
 *
 * @file   parse_error.h
 * @author David Gill
 * @date   21/02/2010
 */

#include <stdexcept>

namespace loci {
namespace mocaps
{
    class parse_error : public std::runtime_error
    {
    public:
        parse_error(const std::string & message)
            : std::runtime_error(message)
        {
        }
    };
} // namespace mocaps
} // namespace loci

#endif // LOCI_MOCAPS_PARSE_ERROR_H_