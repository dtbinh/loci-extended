/**
 * Implementation of a joint_renderer.
 * Configures the d3d9 pipeline to draw a joint.
 *
 * @file   joint_renderer.cpp
 * @author David Gill
 * @date   06/11/2009
 */

#include <boost/shared_ptr.hpp>

#include <d3dx9.h>

#include "loci/video/d3d9/sphere.h"
#include "loci/video/d3d9/fixed_function_pipeline.h"
#include "loci/numeric/vector.h"
#include "loci/numeric/maths.h"

#include "loci/video/renderers/joint_renderer.h"

namespace loci {
namespace video
{
    joint_renderer::joint_renderer(const boost::shared_ptr<d3d9::sphere> & joint_mesh,
                                   const D3DXMATRIX & model_transform,
                                   const d3d9::fixed_function_pipeline & pipeline)
        : joint_mesh(joint_mesh)
        , model_transform(model_transform)
        , pipeline(pipeline)
    {
    }

#ifndef LOCI_DEBUGGING
    void joint_renderer::draw(const D3DXMATRIX & joint_transform, const numeric::vector3f & tip_offset)
#else
    void joint_renderer::draw(const D3DXMATRIX & joint_transform, const numeric::vector3f & tip_offset, float r, float g, float b)
#endif
    {
        D3DXMATRIX scale;
        D3DXMATRIX rotation;
        D3DXMATRIX local_transform;
        D3DXMATRIX mesh_transform;
        D3DXMATRIX final_transform;
        D3DXMATRIX invert_z;
        D3DXMATRIX final_inverted;

        numeric::vector3f up(0, 1, 0);
        numeric::vector3f axis(1.0f, 0.0f, 0.0f); // default to x-axis
        float angle = rads_between(up, tip_offset);

        if (!numeric::approx_equal(angle, 0.0f) && !numeric::approx_equal<float>(angle, numeric::pi))
        {                
            axis = numeric::cross(up, tip_offset).normalise();
        }
        
        D3DXVECTOR3 d3dx_axis(axis.x(), axis.y(), axis.z());
        D3DXMatrixRotationAxis(&rotation, &d3dx_axis, angle);


        static const float thickness_to_length_ratio = 0.3f;
        float length = tip_offset.length();
        float thickness = 2.0f * length * thickness_to_length_ratio;
        D3DXMatrixScaling(&scale, thickness, length, thickness);
        D3DXMatrixMultiply(&mesh_transform, &model_transform, &scale);
        D3DXMatrixMultiply(&local_transform, &rotation, &joint_transform);
        D3DXMatrixMultiply(&final_transform, &mesh_transform, &local_transform);

#ifdef LOCI_DEBUGGING
        d3d9::initialise_material(
            material,
            D3DXCOLOR(0.7f * r, 0.7f * g, 0.7f * b, 0.0f),
            D3DXCOLOR(0.9f, 0.9f, 0.9f, 0.0f),
            D3DXCOLOR(0.7f * r, 0.7f * g, 0.7f * b, 0.0f),
            D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),
            20.0f);

        pipeline.reset_material(material);
#endif

        pipeline.reset_transform(final_transform);
        joint_mesh->draw();
    }


    D3DXMATRIX & create_bone_transform(D3DXMATRIX & matrix, float radius, float offset)
    {
        D3DXMATRIX offset_mat;
        D3DXMATRIX scale_mat;
        D3DXMatrixTranslation(&offset_mat, 0.0f, offset, 0.0f);
        D3DXMatrixScaling(&scale_mat, radius, 1.0f, radius);
        D3DXMatrixMultiply(&matrix, &scale_mat, &offset_mat);
        return matrix;
    }
} // namespace video
} // namespace loci