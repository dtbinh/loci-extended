#ifndef LOCI_VIDEO_MESH_H_
#define LOCI_VIDEO_MESH_H_

/**
 * Data structures for representing simple meshes.
 * Defines simple structures for constructing meshes, vertex arrays
 * and materials.
 *
 * @file   mesh.h
 * @author David Gill
 * @date   24/04/2009
 */

#include <vector>
#include <string>

namespace loci {
namespace video
{
    struct vertex_xyz
    {
        vertex_xyz() { }
        vertex_xyz(float x, float y, float z) : x(x), y(y), z(z) { }
        float x, y, z;
    };

    struct vertex_uv
    {
        vertex_uv() { }
        vertex_uv(float u, float v) : u(u), v(v) { }
        float u, v;
    };

    struct vertex_nxyz
    {
        vertex_nxyz() { }
        vertex_nxyz(float x, float y, float z) : x(x), y(y), z(z) { }
        float x, y, z;
    };

    struct vertex
    {
        vertex() { }
        vertex(const vertex_xyz & position,
               const vertex_uv & texture,
               const vertex_nxyz & normal)
            : position(position)
            , texture(texture)
            , normal(normal)
        {
        }

        vertex_xyz position;
        vertex_nxyz normal;
        vertex_uv texture;
    };

    struct material_range
    {
        material_range(unsigned int start) : start(start), end(start) { }
        unsigned int range() const { return end - start; }
        std::string material;
        unsigned int start, end;
    };

    typedef std::vector<vertex> vertex_array;
    typedef std::vector<material_range> materials_list;
} // namespace video
} // namespace loci

#endif // LOCI_VIDEO_MESH_H_