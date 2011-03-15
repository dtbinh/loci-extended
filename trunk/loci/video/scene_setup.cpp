
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <d3dx9.h>

#include "loci/platform/win32/windows_common.h"
#include "loci/platform/tstring.h"

#include "loci/video/d3d9/device_services.h"
#include "loci/video/d3d9/display_settings.h"
#include "loci/video/d3d9/scene_presenter.h"
#include "loci/video/d3d9/reset_service.h"
#include "loci/video/d3d9/fixed_function_pipeline.h"
#include "loci/video/d3d9/sphere.h"
#include "loci/video/d3d9/vertex_buffer_allocator.h"
#include "loci/video/d3d9/primitive_drawer.h"
#include "loci/video/d3d9/font_map.h"

#include "loci/video/renderers/renderer.h"
#include "loci/video/renderers/joint_renderer.h"
#include "loci/video/renderers/arena_renderer.h"
#include "loci/video/renderers/simple_mesh_renderer.h"
#include "loci/video/renderers/drawer.h"

#include "loci/video/resources/mesh.h"
#include "loci/video/resources/obj_loader.h"
#include "loci/video/resources/texture_factory.h"

namespace loci {
namespace video
{
    renderer setup_scene_renderer(HWND output_window,
                             const d3d9::display_settings & config,
                             const platform::tstring & font_name,
                             unsigned int font_size,
                             unsigned int slices,
                             unsigned int stacks,
                             const std::string & scene_path,
                             float scale)
    {
        boost::shared_ptr<d3d9::device_services> device =
            d3d9::create_shared_device(output_window, config);

        d3d9::fixed_function_pipeline pipeline(device);

        static const float unit_sphere_radius = 0.5f;

        boost::shared_ptr<d3d9::sphere> sphere(
            boost::make_shared<d3d9::sphere>(device, unit_sphere_radius, slices, stacks));

        D3DXMATRIX mesh_transform;
        create_bone_transform(mesh_transform, unit_sphere_radius, unit_sphere_radius);

        boost::shared_ptr<scene_renderer> scene;
        if (scene_path.empty())
        {
            scene = boost::make_shared<arena_renderer>(
                        d3d9::vertex_buffer_allocator(device),
                        d3d9::primitive_drawer(device),
                        pipeline,
                        100, 100,
                        100, 100);
        }
        else
        {
            vertex_array va;
            materials_list ml;
            texture_factory textures(device);
            parse_obj(va, ml, textures, scene_path, true);

            scene = boost::make_shared<simple_mesh_renderer>(
                        va, ml, scale,
                        textures,
                        d3d9::vertex_buffer_allocator(device),
                        d3d9::primitive_drawer(device),
                        pipeline);
        }

        return renderer(
            config,
            d3d9::scene_presenter(device),
            d3d9::reset_service(device),
            pipeline,
            scene,
            joint_renderer(
                sphere,
                mesh_transform,
                pipeline),
            boost::make_shared<d3d9::font_map>(device, font_name, font_size),
            sphere);
    }

    renderer setup_scene_renderer(HWND output_window,
                                  const d3d9::display_settings & config,
                                  const std::string & scene_path,
                                  float scale)
    {
        return setup_scene_renderer(output_window, config,
                                    LOCI_TSTR("Century Gothic"), 20,
                                    10, 10,
                                    scene_path, scale);
    }
} // namespace video
} // namespace loci