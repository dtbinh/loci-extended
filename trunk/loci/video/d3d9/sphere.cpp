/**
 * Implementation of a sphere.
 * Encapsulates a d3dxmesh, providing RAII.
 *
 * @file   sphere.cpp
 * @author David Gill
 * @date   15/02/2009
 */

#include <memory>

#include <boost/shared_ptr.hpp>

#include <d3dx9.h>

#include "loci/video/d3d9/sphere.h"
#include "loci/video/d3d9/device_services.h"
#include "loci/video/d3d9/d3d_error.h"

namespace loci {
namespace video {
namespace d3d9
{
    sphere::sphere(
        const boost::shared_ptr<device_services> & device,
        float radius, unsigned int slices, unsigned int stacks)
        : device(device)
        , radius(radius)
        , slices(slices)
        , stacks(stacks)
    {
        reload();
    }

    sphere::~sphere()
    {
        release();
    }

    void sphere::draw()
    {
        HRESULT r;
        r = mesh->DrawSubset(0);
        check_failure(r, "Failed to draw sphere mesh", "ID3DXMesh::DrawSubset");
    }

    void sphere::release()
    {
        mesh->Release();
        mesh = 0;
    }

    void sphere::reload()
    {
        HRESULT r = D3DXCreateSphere(device->get(), radius, slices, stacks, &mesh, NULL);
        check_failure(r, "Failed to create sphere", "D3DXCreateSphere");
    }
} // namespace d3d9
} // namespace video
} // namespace loci