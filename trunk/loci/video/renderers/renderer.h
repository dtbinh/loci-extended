#ifndef LOCI_VIDEO_RENDERERS_RENDERER_H_
#define LOCI_VIDEO_RENDERERS_RENDERER_H_

/**
 * Renderering class.
 * Defines an API for rendering with d3d9.
 *
 * @file   renderer.h
 * @author David Gill
 * @date   11/06/2009
 */

#include <vector>

#include <boost/shared_ptr.hpp>

#include <d3d9.h>

#include "loci/platform/win32/windows_common.h"

#include "loci/platform/tstring.h"

#include "loci/pose.h"
#include "loci/skeleton.h"
#include "loci/crowd.h"

#include "loci/video/d3d9/scene_presenter.h"
#include "loci/video/d3d9/reset_service.h"
#include "loci/video/d3d9/fixed_function_pipeline.h"
#include "loci/video/d3d9/display_settings.h"

#include "loci/video/renderers/drawer.h"
#include "loci/video/renderers/joint_renderer.h"

namespace loci {
namespace video
{
    // forward declaration
    namespace d3d9
    {
        class font_map;
        class sphere;
    }

    class renderer
    {
    public:
        renderer(const d3d9::display_settings & config,
                 const d3d9::scene_presenter & scene,
                 const d3d9::reset_service & resetter,
                 const d3d9::fixed_function_pipeline & pipeline,
                 const boost::shared_ptr<scene_renderer> scene_visual,
                 const joint_renderer & visuals,
                 const boost::shared_ptr<d3d9::font_map> & font,
                 const boost::shared_ptr<d3d9::sphere> & sphere_to_recover);

        void move_camera(float yrot, float zoom, float height);

        bool render(const crowd & people, const platform::tstring & text);
        bool render(const crowd & people, const platform::tstring & text, const std::vector< std::vector<pose> > & frames);

        bool reset_device(const d3d9::display_settings & config);

    private:
        void release_all();
        void recover_all();

        void configure_pipeline();

        bool pre_render();
        bool post_render(const platform::tstring & text);

    private:
        d3d9::display_settings config;
        d3d9::scene_presenter scene;
        d3d9::reset_service resetter;
        d3d9::fixed_function_pipeline pipeline;
        boost::shared_ptr<scene_renderer> scene_visual;
        joint_renderer visuals;
        boost::shared_ptr<d3d9::font_map> font;
        boost::shared_ptr<d3d9::sphere> sphere_to_recover;
        bool resources_held;
        D3DMATERIAL9 character_material;
        D3DMATERIAL9 scene_material;

        float camera_zoom;
        float camera_height;
        float camera_rotation;
    };
} // namespace video
} // namespace loci

#endif // LOCI_VIDEO_RENDERERS_RENDERER_H_