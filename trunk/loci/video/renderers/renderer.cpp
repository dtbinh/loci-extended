/**
 * Implementation of a d3d9 renderer.
 * Implements the d3d9 renderer API.
 *
 * @file   renderer.cpp
 * @author David Gill
 * @date   30/08/2009
 */

#include <vector>

#include <boost/shared_ptr.hpp>

#include <d3d9.h>
#include <d3dx9.h>

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
#include "loci/video/renderers/draw_skeleton.h"

#include "loci/pose.h"
#include "loci/skeleton.h"
#include "loci/crowd.h"

namespace loci {
namespace video
{


    renderer::renderer(const d3d9::display_settings & config,
                       const d3d9::scene_presenter & scene,
                       const d3d9::reset_service & resetter,
                       const d3d9::fixed_function_pipeline & pipeline,
                       const boost::shared_ptr<scene_renderer> scene_visual,
                       const joint_renderer & visuals,
                       const boost::shared_ptr<d3d9::font_map> & font,
                       const boost::shared_ptr<d3d9::sphere> & sphere_to_recover)
        : config(config)
        , scene(scene)
        , resetter(resetter)
        , pipeline(pipeline)
        , scene_visual(scene_visual)
        , visuals(visuals)
        , font(font)
        , sphere_to_recover(sphere_to_recover)
        , resources_held(true)
        , camera_zoom(1.0f)
        , camera_height(200.0f)
        , camera_rotation(0.0f)
    {
        configure_pipeline();

        d3d9::initialise_material(
            character_material,
            D3DXCOLOR(0.7f, 0.7f, 0.0f, 0.0f),
            D3DXCOLOR(0.9f, 0.9f, 0.9f, 0.0f),
            D3DXCOLOR(0.7f, 0.7f, 0.0f, 0.0f),
            D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),
            20.0f);

        d3d9::initialise_material(
            scene_material,
            D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f),
            D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),
            D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f),
            D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),
            20.0f);
    }

    void renderer::move_camera(float yrot, float zoom, float height)
    {
        camera_rotation += yrot;
        camera_zoom += zoom;
        camera_height += height;

        camera_zoom = camera_zoom < 0.1 ? 0.1 : camera_zoom;
        camera_height = camera_height < 10 ? 10 : camera_height;
    }

    bool renderer::render(const crowd & people, const platform::tstring & text)
    {
        if (!pre_render()) { return false; }

        D3DXMATRIX skel_transform;
        ::D3DXMatrixScaling(&skel_transform, 0.5, 0.5, 0.5);
        
        for (crowd::size_type i = 0; i < people.size(); ++i)
        {
            skeleton s = people.person(i);
            draw_skeleton(*s.bind_pose(), *s.structure(), visuals, skel_transform);
        }

        if (scene_visual->reflective())
        {
            pipeline.culling(D3DCULL_CCW);
            D3DXMATRIX invert_transform;
            ::D3DXMatrixScaling(&invert_transform, 1.0f, -1.0f, 1.0f);
            D3DXMATRIX reflection_transform;
            ::D3DXMatrixMultiply(&reflection_transform, &skel_transform, &invert_transform);
       
            for (crowd::size_type i = 0; i < people.size(); ++i)
            {
                skeleton s = people.person(i);
                draw_skeleton(*s.bind_pose(), *s.structure(), visuals, reflection_transform);
            }

            pipeline.culling();
        }

        return post_render(text);
    }

    bool renderer::render(const crowd & people, const platform::tstring & text, const std::vector< std::vector<pose> > & frames)
    {
        if (!pre_render()) { return false; }

        D3DXMATRIX skel_transform;
        ::D3DXMatrixScaling(&skel_transform, 0.5, 0.5, 0.5);
        
        for (crowd::size_type i = 0; i < people.size(); ++i)
        {
            skeleton s = people.person(i);
            for (unsigned int j = 0; j < frames[i].size(); ++j)
            {
                draw_skeleton(frames[i][j], *s.structure(), visuals, skel_transform);
            }
        }

        if (scene_visual->reflective())
        {
            pipeline.culling(D3DCULL_CCW);
            D3DXMATRIX invert_transform;
            ::D3DXMatrixScaling(&invert_transform, 1.0f, -1.0f, 1.0f);
            D3DXMATRIX reflection_transform;
            ::D3DXMatrixMultiply(&reflection_transform, &skel_transform, &invert_transform);
       
            for (crowd::size_type i = 0; i < people.size(); ++i)
            {
                skeleton s = people.person(i);
                for (unsigned int j = 0; j < frames[i].size(); ++j)
                {
                    draw_skeleton(frames[i][j], *s.structure(), visuals, reflection_transform);
                }
            }

            pipeline.culling();
        }

        return post_render(text);
    }

    bool renderer::reset_device(const d3d9::display_settings & config)
    {
        release_all();
        if (resetter.try_reset(config))
        {
            recover_all();
            this->config = config;
            return true;
        }
        return false;
    }

    void renderer::release_all()
    {
        if (!resources_held) { return; }

        font->release();
        sphere_to_recover->release();

        resources_held = false;
    }

    void renderer::recover_all()
    {
        configure_pipeline();
        font->reload();
        sphere_to_recover->reload();

        resources_held = true;
    }

    void renderer::configure_pipeline()
    {
        pipeline.reset_projection();
        pipeline.culling();
        pipeline.configure_alpha_blending(D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA);
        pipeline.lighting(true);
        pipeline.modulate_texture(0);
        pipeline.configure_sampler(0, true, true);

        D3DLIGHT9 white_light;
        pipeline.configure_light(
            0,
            d3d9::create_directional_light(
                white_light,
                D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.0f),
                D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.0f),
                D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.0f),
                D3DXVECTOR3(1.0f, -1.0f, 0.0f)),
            true);
    }

    bool renderer::pre_render()
    {
        if (!resetter.ready() &&
            !reset_device(config))
        {
            return false;
        }

        pipeline.reset_view(0.0f, camera_height * camera_zoom, 400.0f * camera_zoom,
                            0.0f, 0.0, 0,
                            0, 1.0f, 0,
                            camera_rotation);

        scene.begin_frame();

        pipeline.reset_material(character_material);

        return true;
    }

    bool renderer::post_render(const platform::tstring & text)
    {
        font->draw_text(text, D3DCOLOR_XRGB(255, 255, 0), 20, 30);

        pipeline.reset_material(scene_material);
        scene_visual->draw();
        pipeline.alpha_blending(false);
        pipeline.lighting(true);

        return scene.submit_frame();
    }
} // namespace video
} // namespace loci