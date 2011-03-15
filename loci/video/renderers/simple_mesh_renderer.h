#ifndef LOCI_VIDEO_RENDERERS_SIMPLE_MESH_RENDERER_H_
#define LOCI_VIDEO_RENDERERS_SIMPLE_MESH_RENDERER_H_

/**
 * Configures the d3d9 pipeline to draw an simple mesh.
 * Draws a textured mesh.
 *
 * @file   simple_mesh_renderer.h
 * @author David Gill
 * @date   24/04/2009
 */

#include <memory>
#include <vector>

#include <d3d9.h>
#include <d3dx9.h>

#include "loci/video/d3d9/vertex_buffer_allocator.h"
#include "loci/video/d3d9/managed_vertex_buffer.h"
#include "loci/video/d3d9/primitive_drawer.h"
#include "loci/video/d3d9/fixed_function_pipeline.h"
#include "loci/video/d3d9/managed_texture.h"

#include "loci/video/renderers/drawer.h"

#include "loci/video/resources/mesh.h"
#include "loci/video/resources/texture_factory.h"

namespace loci {
namespace video
{
    namespace // anonymous
    {
        std::auto_ptr<d3d9::managed_vertex_buffer> create_mesh(
            const d3d9::vertex_buffer_allocator & vb_allocator,
            const vertex_array & vertices)
        {
            unsigned int bytes = sizeof(vertex) * vertices.size();
            const void * data = vertices.empty() ? 0 : &vertices.front();
            return vb_allocator.create_managed(bytes, data, bytes);
        }

        const DWORD fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
    } // anonymous namespace

    class simple_mesh_renderer : public scene_renderer
    {
    public:
        simple_mesh_renderer(const vertex_array & vertices,
                             const materials_list & materials,
                             float scale,
                             const texture_factory & textures,
                             const d3d9::vertex_buffer_allocator & vb_allocator,
                             const d3d9::primitive_drawer & drawer,
                             const d3d9::fixed_function_pipeline & pipeline)
            : drawer(drawer)
            , pipeline(pipeline)
            , mesh( create_mesh(vb_allocator, vertices) )
            , scale(scale)
        {
            this->drawer.topology(D3DPT_TRIANGLELIST);

            chunks.reserve(materials.size());
            for (materials_list::const_iterator iter = materials.begin(); iter != materials.end(); ++iter)
            {
                chunks.push_back(
                    geometry_chunk(
                        iter->start * 3,
                        iter->end - iter->start,
                        textures.try_get(iter->material)));
            }
        }

    private:
        void render()
        {
            D3DXMATRIX scaling;
            ::D3DXMatrixScaling(&scaling, scale, scale, scale);
            pipeline.reset_transform(scaling);

            mesh->bind(sizeof(vertex), fvf);
            for (geometry_list::iterator gc = chunks.begin(); gc != chunks.end(); ++gc)
            {
                set_gc_texture(*gc);
                drawer.draw(gc->primitive_count, gc->start_vertex);
            }
            pipeline.disable_texture();
        }

        struct geometry_chunk
        {
            geometry_chunk(unsigned int start_vertex,
                           unsigned int primitive_count,
                           const boost::shared_ptr<d3d9::managed_texture2d> & texture)
                : start_vertex(start_vertex)
                , primitive_count(primitive_count)
                , texture(texture)
            {
            }

            unsigned int start_vertex;
            unsigned int primitive_count;
            boost::shared_ptr<d3d9::managed_texture2d> texture;
        };

        void set_gc_texture(const geometry_chunk & gc)
        {
            if (gc.texture)
            {
                gc.texture->bind();
            }
            else
            {
                pipeline.disable_texture();
            }
        }

        typedef std::vector<geometry_chunk> geometry_list;

        d3d9::primitive_drawer drawer;
        d3d9::fixed_function_pipeline pipeline;

        boost::shared_ptr<d3d9::managed_vertex_buffer> mesh;
        float scale;

        geometry_list chunks;
    };
} // namespace video
} // namespace loci

#endif // LOCI_VIDEO_RENDERERS_SIMPLE_MESH_RENDERER_H_