#ifndef LOCI_VIDEO_RENDERERS_JOINT_RENDERER_H_
#define LOCI_VIDEO_RENDERERS_JOINT_RENDERER_H_

/**
 * Configures the d3d9 pipeline to draw a joint.
 * Sets up resources and provides a function to bind those to the pipeline in
 * order to render a single joint.
 *
 * @file   joint_renderer.h
 * @author David Gill
 * @date   05/11/2009
 */

#include <boost/shared_ptr.hpp>

#include <d3dx9.h>

#include "loci/video/d3d9/fixed_function_pipeline.h"
#include "loci/numeric/vector.h"

namespace loci {
namespace video
{
    // forward declarations
    namespace d3d9
    {
        class sphere;
        class device_services;
    }

    class joint_renderer
    {
    public:
        joint_renderer(const boost::shared_ptr<d3d9::sphere> & joint_mesh,
                       const D3DXMATRIX & model_transform,
                       const d3d9::fixed_function_pipeline & pipeline);

#ifndef LOCI_DEBUGGING
        void draw(const D3DXMATRIX & joint_transform, const numeric::vector3f & tip_offset);
#else
        void draw(const D3DXMATRIX & joint_transform, const numeric::vector3f & tip_offset, float r, float g, float b);
#endif

    private:
        boost::shared_ptr<d3d9::sphere> joint_mesh;
        D3DXMATRIX model_transform;
        d3d9::fixed_function_pipeline pipeline;

#ifdef LOCI_DEBUGGING
        D3DMATERIAL9 material;
#endif
    };

    D3DXMATRIX & create_bone_transform(D3DXMATRIX & matrix, float radius, float offset = 0.5f);
} // namespace video
} // namespace loci

#endif // LOCI_VIDEO_RENDERERS_JOINT_RENDERER_H_