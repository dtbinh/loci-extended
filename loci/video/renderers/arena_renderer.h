#ifndef LOCI_VIDEO_RENDERERS_ARENA_RENDERER_H_
#define LOCI_VIDEO_RENDERERS_ARENA_RENDERER_H_

/**
 * Configures the d3d9 pipeline to draw an arena mesh.
 * Draws a tesselated floor plane.
 *
 * @file   arena_renderer.h
 * @author David Gill
 * @date   09/03/2009
 */

#include <boost/shared_ptr.hpp>

#include <d3dx9.h>

#include "loci/video/d3d9/primitive_drawer.h"
#include "loci/video/d3d9/fixed_function_pipeline.h"

#include "loci/video/renderers/drawer.h"

namespace loci {
namespace video
{
    // forward declarations
    namespace d3d9
    {
        class vertex_buffer_allocator;
        class managed_vertex_buffer;
    }

    class arena_renderer : public scene_renderer
    {
    public:
        arena_renderer(const d3d9::vertex_buffer_allocator & vb_allocator,
                       const d3d9::primitive_drawer & drawer,
                       const d3d9::fixed_function_pipeline & pipeline,
                       unsigned int mid_points_x, unsigned int mid_points_y,
                       float cell_length, float cell_width);

    private:
        void render();

        d3d9::primitive_drawer drawer;
        d3d9::fixed_function_pipeline pipeline;
        boost::shared_ptr<d3d9::managed_vertex_buffer> arena;
        unsigned int size;
    };
} // namespace video
} // namespace loci

#endif // LOCI_VIDEO_RENDERERS_ARENA_RENDERER_H_