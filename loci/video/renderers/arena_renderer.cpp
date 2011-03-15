/**
 * Implementation of a arena_renderer.
 * Configures the d3d9 pipeline to draw the arena.
 *
 * @file   arena_renderer.cpp
 * @author David Gill
 * @date   09/03/2009
 */

#include <memory>
#include <vector>

#include <d3d9.h>
#include <d3dx9.h>

#include "loci/numeric/vector.h"

#include "loci/video/d3d9/vertex_buffer_allocator.h"
#include "loci/video/d3d9/managed_vertex_buffer.h"
#include "loci/video/d3d9/primitive_drawer.h"
#include "loci/video/d3d9/fixed_function_pipeline.h"

#include "loci/video/renderers/arena_renderer.h"

namespace loci {
namespace video
{
    namespace // anonymous
    {
        struct vertex
        {
            vertex(float x, float y, float z, DWORD colour)
                : x(x), y(y), z(z), colour(colour)
            {
            }

            float x, y, z;
            DWORD colour;
        };

        const DWORD fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;

        std::auto_ptr<d3d9::managed_vertex_buffer> create_floor(
            const d3d9::vertex_buffer_allocator & vb_allocator,
            unsigned int num_x, unsigned int num_y,
            float cell_length, float cell_width)
        {
            std::vector<vertex> vertices;
            vertices.reserve(num_x * num_y);

            float middle_x = static_cast<float>(num_x) * cell_length / 2.0f;
            float middle_y = static_cast<float>(num_y) * cell_width  / 2.0f;

            bool first_colour = true;

            for (unsigned int i = 0; i < num_x; ++i)
            {
                for (unsigned int j = 0; j < num_y; ++j)
                {
                    float u = static_cast<float>(i) * cell_length - middle_x;
                    float v = static_cast<float>(j) * cell_width  - middle_y;

                    float ulen = u + cell_length;
                    float vwid = v - cell_width;

                    DWORD colour = first_colour ? D3DCOLOR_RGBA(120, 120, 120, 200) :
                                                  D3DCOLOR_RGBA(80, 80, 80, 200);

                    vertices.push_back( vertex(u   , 0, v   ,  colour) );
                    vertices.push_back( vertex(ulen, 0, v   ,  colour) );
                    vertices.push_back( vertex(u   , 0, vwid,  colour) );
                    vertices.push_back( vertex(ulen, 0, v   ,  colour) );
                    vertices.push_back( vertex(ulen, 0, vwid,  colour) );
                    vertices.push_back( vertex(u   , 0, vwid,  colour) );

                    first_colour = !first_colour;
                }
                if (num_y % 2 == 0) { first_colour = !first_colour; }
            }

            unsigned int bytes = sizeof(vertex) * vertices.size();
            return vb_allocator.create_managed(bytes, &vertices.front(), bytes);
        }
    }

    arena_renderer::arena_renderer(const d3d9::vertex_buffer_allocator & vb_allocator,
                                   const d3d9::primitive_drawer & drawer,
                                   const d3d9::fixed_function_pipeline & pipeline,
                                   unsigned int mid_points_x, unsigned int mid_points_y,
                                   float cell_length, float cell_width)
        : scene_renderer(true) // reflections
        , drawer(drawer)
        , pipeline(pipeline)
        , arena( create_floor(vb_allocator, mid_points_x+1, mid_points_y+1, cell_length, cell_width) )
        , size(2 * (mid_points_x+1) * (mid_points_y+1))
    {
        this->drawer.topology(D3DPT_TRIANGLELIST);
    }

    void arena_renderer::render()
    {
        pipeline.lighting(false);
        pipeline.alpha_blending(true);

        D3DXMATRIX floor_position;
        D3DXMatrixIdentity(&floor_position);

        pipeline.reset_transform(floor_position);

        arena->bind(sizeof(vertex), fvf);
        drawer.draw(size);
    }
} // namespace video
} // namespace loci