#ifndef LOCI_CONTROL_XML_PARSE_ERROR_H_
#define LOCI_CONTROL_XML_PARSE_ERROR_H_

/**
 * An exception to be thrown upon error parsing a script.
 * Upon failure parsing a script, parsers should throw parse_error which derives
 * from std::runtime_error.
 *
 * @file   parse_error.h
 * @author David Gill
 * @date   02/05/2010
 */

#include <stdexcept>

namespace loci {
namespace control {
namespace xml
{
    class parse_error : public std::runtime_error
    {
    public:
        parse_error(const std::string & message)
            : std::runtime_error(message)
        {
        }
    };
} // namespace xml
} // namespace control
} // namespace loci

#endif // LOCI_CONTROL_XML_PARSE_ERROR_H_