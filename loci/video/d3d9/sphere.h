#ifndef LOCI_VIDEO_D3D9_SPHERE_H_
#define LOCI_VIDEO_D3D9_SPHERE_H_

/**
 * Encapsulates a d3dxmesh modelling a sphere.
 * Maintains a d3dxmesh with proper RAII support.
 *
 * @file   sphere.h
 * @author David Gill
 * @date   08/02/2010
 */

#include <memory>

#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include <d3dx9.h>

namespace loci {
namespace video {
namespace d3d9
{
    // forward declaration
    class device_services;

    class sphere : boost::noncopyable
    {
    public:
        sphere(const boost::shared_ptr<device_services> & device,
               float radius, unsigned int slices, unsigned int stacks);

        ~sphere();

        void draw();

        void release();
        void reload();

    private:
        boost::shared_ptr<device_services> device;
        LPD3DXMESH mesh;
        // for recovery
        float radius;
        unsigned int slices;
        unsigned int stacks;
    };
} // namespace d3d9
} // namespace video
} // namespace loci
    
#endif // LOCI_VIDEO_D3D9_SPHERE_H_