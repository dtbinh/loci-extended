#ifndef LOCI_LOCI_H_
#define LOCI_LOCI_H_

/**
 * A master include file to pull in all the major API components.
 * Conveniently groups all the major inclusion files for the API.
 *
 * @file   loci.h
 * @author David Gill
 * @date   03/05/2010
 */

#include "loci/pose.h"
#include "loci/skeleton.h"
#include "loci/skeleton_builder.h"
#include "loci/hierarchy.h"
#include "loci/named_indices.h"
#include "loci/crowd.h"
#include "loci/application.h"

#include "loci/anim/animtree.h"
#include "loci/anim/keyed_motion.h"
#include "loci/anim/weight_mask.h"

#include "loci/control/actor_controller.h"
#include "loci/control/xml_controller.h"
#include "loci/control/xml/anim_parse_dispatcher.h"
#include "loci/control/xml/motion_set.h"
#include "loci/control/xml/parser.h"
#include "loci/control/xml/parse_error.h"

#include "loci/mocaps/bvh_document.h"
#include "loci/mocaps/mocap.h"
#include "loci/mocaps/parse_error.h"
#include "loci/mocaps/recording.h"

#include "loci/numeric/maths.h"
#include "loci/numeric/conversion.h"
#include "loci/numeric/transform.h"
#include "loci/numeric/parametric.h"
#include "loci/numeric/euler.h"
#include "loci/numeric/quat.h"
#include "loci/numeric/vector.h"

#include "loci/platform/tchar.h"
#include "loci/platform/tstring.h"
#include "loci/platform/win32/dispatch.h"
#include "loci/platform/win32/window.h"
#include "loci/platform/win32/window_class.h"

#include "loci/video/render_view.h"
#include "loci/video/scene_setup.h"

#ifdef LOCI_FULL_FAT

    #include "loci/platform/tsstream.h"
    #include "loci/platform/tstream.h"

    #include "loci/video/d3d9/adapters_service.h"
    #include "loci/video/d3d9/device_services.h"
    #include "loci/video/d3d9/display_settings.h"
    #include "loci/video/d3d9/fixed_function_pipeline.h"
    #include "loci/video/d3d9/font_map.h"
    #include "loci/video/d3d9/managed_texture.h"
    #include "loci/video/d3d9/managed_vertex_buffer.h"
    #include "loci/video/d3d9/primitive_drawer.h"
    #include "loci/video/d3d9/reset_service.h"
    #include "loci/video/d3d9/scene_presenter.h"
    #include "loci/video/d3d9/sphere.h"
    #include "loci/video/d3d9/vertex_buffer_allocator.h"
    #include "loci/video/d3d9/d3d_error.h"

    #include "loci/video/renderers/arena_renderer.h"
    #include "loci/video/renderers/draw_skeleton.h"
    #include "loci/video/renderers/drawer.h"
    #include "loci/video/renderers/joint_renderer.h"
    #include "loci/video/renderers/renderer.h"
    #include "loci/video/renderers/simple_mesh_renderer.h"

    #include "loci/video/resources/file_resource_cache.h"
    #include "loci/video/resources/mesh.h"
    #include "loci/video/resources/obj_loader.h"
    #include "loci/video/resources/texture_factory.h"

#endif

#endif // LOCI_LOCI_H_
