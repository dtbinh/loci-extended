#ifndef LOCI_VIDEO_D3D9_FAILURE_H_
#define LOCI_VIDEO_D3D9_FAILURE_H_

/**
 * An exception class for d3d9 errors.
 * An exception class, derived from runtime_error, to be used when signalling
 * d3d9 error. A utility function for throwing on HRESULT failure is also provided.
 *
 * @file   d3d_error.h
 * @author David Gill
 * @date   15/02/2009
 */

#include <stdexcept>
#include <string>

#include <d3d9.h>

namespace loci {
namespace video {
namespace d3d9
{
    class d3d_error : public std::runtime_error
    {
    public:
        d3d_error(const std::string & message, const std::string & who)
            : std::runtime_error( message + "(" + who + ")" )
            , error(E_FAIL)
        {
        }

        d3d_error(HRESULT error, const std::string & message, const std::string & who)
            : std::runtime_error( message + "(" + who + ")" )
            , error(error)
        {
        }

        HRESULT result()
        {
            return error;
        }

    private:
        HRESULT error;
    };

    inline void check_failure(HRESULT result, const char * message, const char * who)
    {
        if (FAILED(result))
        {
            throw d3d_error(result, message, who);
        }
    }
} // namespace d3d9
} // namespace video
} // namespace loci

#endif // LOCI_VIDEO_D3D9_FAILURE_H_