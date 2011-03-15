#ifndef LOCI_VIDEO_D3D9_MANAGED_TEXTURE_H_
#define LOCI_VIDEO_D3D9_MANAGED_TEXTURE_H_

/**
 * Encapsulates a d3d9 texture, providing RAII.
 * Sets up a texture, allows binding of that texture to the pipeline and
 * provides RAII support to automatically release it.
 *
 * @file   managed_texture.h
 * @author David Gill
 * @date   24/04/2010
 */

#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include <d3d9.h>

#include "loci/platform/tstring.h"

namespace loci {
namespace video {
namespace d3d9
{
    // forward declaration
    class device_services;

    class managed_texture2d : boost::noncopyable
    {
    public:
        managed_texture2d(const boost::shared_ptr<device_services> & device,
                          const platform::tstring & src_file);
        ~managed_texture2d();

        void bind();

    private:
        boost::shared_ptr<device_services> device;
        LPDIRECT3DTEXTURE9 texture;
    };
} // namespace d3d9
} // namespace video
} // namespace loci
    
#endif // LOCI_VIDEO_D3D9_MANAGED_TEXTURE_H_