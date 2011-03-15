/**
 * Implementation of a managed_texture2d.
 * Encapsulates a d3d9 texture, providing RAII.
 *
 * @file   managed_texture.cpp
 * @author David Gill
 * @date   24/04/2010
 */

#include <boost/shared_ptr.hpp>

#include <d3d9.h>
#include <d3dx9.h>

#include "loci/platform/tstring.h"

#include "loci/video/d3d9/managed_texture.h"
#include "loci/video/d3d9/device_services.h"
#include "loci/video/d3d9/d3d_error.h"

namespace loci {
namespace video {
namespace d3d9
{
    managed_texture2d::managed_texture2d(const boost::shared_ptr<device_services> & device,
                        const platform::tstring & src_file)
        : device(device)
    {
        HRESULT r = D3DXCreateTextureFromFile(device->get(), src_file.c_str(), &texture);
        check_failure(r, platform::narrow(LOCI_TSTR("Failed to load texture from file: ") + src_file).c_str(), "D3DXCreateTextureFromFile");
    }

    managed_texture2d::~managed_texture2d()
    {
        texture->Release();
    }

    void managed_texture2d::bind()
    {
        device->bind_texture(texture);
    }
} // namespace d3d9
} // namespace video
} // namespace loci