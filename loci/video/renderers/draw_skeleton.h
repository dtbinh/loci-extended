#ifndef LOCI_VIDEO_RENDERERS_DRAW_SKELETON_H_
#define LOCI_VIDEO_RENDERERS_DRAW_SKELETON_H_

/**
 * Iterates skeletal joints for rendering.
 * Provides an interface for rendering skeletons.
 *
 * @file   draw_skeleton.h
 * @author David Gill
 * @date   08/11/2009
 */

#include <iterator> // for std::advance

#include <d3dx9.h>

#include "loci/numeric/quat.h"

#include "loci/video/renderers/joint_renderer.h"
#include "loci/video/d3d9/primitive_drawer.h"

#include "loci/pose.h"
#include "loci/hierarchy.h"

namespace loci {
namespace video
{
    inline D3DXQUATERNION & d3dxquat_from_quatf(D3DXQUATERNION & dest, const numeric::quatf & src)
    {
        dest.w = src.w();
        dest.x = src.x();
        dest.y = src.y();
        dest.z = src.z();
        return dest;
    }

    inline D3DXMATRIX & d3dxmatrix_translation(D3DXMATRIX & matrix, const numeric::vector3f & displacement)
    {
        return *D3DXMatrixTranslation(
            &matrix,
            displacement.x(), displacement.y(), displacement.z());
    }

    inline
    void draw_bones(const pose & p,
                    hierarchy::cursor c,
                    joint_renderer & r,
                    const D3DXMATRIX & transform)
    {
        D3DXQUATERNION orientation;
        D3DXMATRIX reused;
        D3DXMATRIX & local_offset = reused;
        D3DXMATRIX & local_rotation = reused;
        D3DXMATRIX world_offset;
        D3DXMATRIX child_world_transform;

        c.to_children();
        while (c.valid())
        {
            const bone_pose & bone = p[c.position()];

#ifndef LOCI_DEBUGGING
            r.draw(transform, bone.tip_offset);
#else
            r.draw(transform, bone.tip_offset, bone.r, bone.g, bone.b);
#endif

            d3dxmatrix_translation(local_offset, bone.tip_offset);
            D3DXMatrixMultiply(&world_offset, &local_offset, &transform);

            d3dxquat_from_quatf(orientation, bone.rotation);
            D3DXMatrixRotationQuaternion(&local_rotation, &orientation);

            D3DXMatrixMultiply(&child_world_transform, &local_rotation, &world_offset);

            draw_bones(p, c, r, child_world_transform);
            c.next();
        }
    }


    inline
    void draw_skeleton(const pose & p, const hierarchy & h, joint_renderer & r, const D3DMATRIX & transform)
    {
        D3DXMATRIX root_translation;
        d3dxmatrix_translation(root_translation, p[h.root().position()].tip_offset);
        
        D3DXMATRIX root_rotation;
        D3DXQUATERNION orientation;
        d3dxquat_from_quatf(orientation, p[h.root().position()].rotation);
        D3DXMatrixRotationQuaternion(&root_rotation, &orientation);

        D3DXMATRIX root_bonespace;
        D3DXMatrixMultiply(&root_bonespace, &root_rotation, &root_translation);

        D3DXMATRIX root_worldspace;
        D3DXMATRIX skeleton_transform(transform);
        D3DXMatrixMultiply(&root_worldspace, &root_bonespace, &skeleton_transform);

        draw_bones(p, h.root(), r, root_worldspace);
    }
} // namespace video
} // namespace loci

#endif // LOCI_VIDEO_RENDERERS_DRAW_SKELETON_H_