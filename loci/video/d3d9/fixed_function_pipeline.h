#ifndef LOCI_VIDEO_D3D9_FIXED_FUNCTION_PIPELINE_H_
#define LOCI_VIDEO_D3D9_FIXED_FUNCTION_PIPELINE_H_

/**
 * Provides a specific interface for configuring the fixed function pipeline.
 * Wraps the device interface to provide a semantically-rich interface for
 * fixed function pipeline state configuration.
 *
 * @file   fixed_function_pipeline.h
 * @author David Gill
 * @date   15/02/2009
 */

#include <boost/shared_ptr.hpp>

#include <d3d9.h>

namespace loci {
namespace video {
namespace d3d9
{
    // forward declaration
    class device_services;

    D3DLIGHT9 & initialise_light(D3DLIGHT9 & light,
                                 D3DLIGHTTYPE type,
                                 const D3DCOLORVALUE & diffuse,
                                 const D3DCOLORVALUE & specular,
                                 const D3DCOLORVALUE & ambient,
                                 const D3DVECTOR & position,
                                 const D3DVECTOR & direction,
                                 float range,
                                 float falloff,
                                 float theta,
                                 float phi,
                                 float attenuation0,
                                 float attenuation1,
                                 float attenuation2);

    D3DLIGHT9 & create_point_light(D3DLIGHT9 & light,
                                   const D3DCOLORVALUE & diffuse,
                                   const D3DCOLORVALUE & specular,
                                   const D3DCOLORVALUE & ambient,
                                   const D3DVECTOR & position,
                                   float range,
                                   float attenuation0,
                                   float attenuation1,
                                   float attenuation2);

    D3DLIGHT9 & create_spot_light(D3DLIGHT9 & light,
                                  const D3DCOLORVALUE & diffuse,
                                  const D3DCOLORVALUE & specular,
                                  const D3DCOLORVALUE & ambient,
                                  const D3DVECTOR & position,
                                  const D3DVECTOR & direction,
                                  float range,
                                  float falloff,
                                  float theta,
                                  float phi,
                                  float attenuation0,
                                  float attenuation1,
                                  float attenuation2);

    D3DLIGHT9 & create_directional_light(D3DLIGHT9 & light,
                                         const D3DCOLORVALUE & diffuse,
                                         const D3DCOLORVALUE & specular,
                                         const D3DCOLORVALUE & ambient,
                                         const D3DVECTOR & direction);
       

    D3DMATERIAL9 & initialise_material(D3DMATERIAL9 & material,
                                       const D3DCOLORVALUE & diffuse,
                                       const D3DCOLORVALUE & specular,
                                       const D3DCOLORVALUE & ambient,
                                       const D3DCOLORVALUE & emissive,
                                       float power);

    class fixed_function_pipeline
    {
    public:
        fixed_function_pipeline(const boost::shared_ptr<device_services> & device);

        void reset_projection(float fovy = 45.0f, float znear = 1.0f, float zfar = 5000.0f);

        void reset_view(float px, float py, float pz,
                        float ax, float ay, float az,
                        float ux = 0.0f, float uy = 1.0f, float uz = 0.0f,
                        float rotation = 0.0f);

        void reset_transform(const D3DMATRIX & transformation);

        void culling(DWORD mode = D3DCULL_CW);

        void alpha_blending(bool enabled);
        void configure_alpha_blending(DWORD src, DWORD dest, bool activate = false);

        void lighting(bool enabled, bool normalise = true, bool specular = true, DWORD specular_source = D3DMCS_MATERIAL);
        void configure_light(unsigned int light, const D3DLIGHT9 & descriptor, bool activate = false);
        void enable_light(unsigned int light, bool enabled);

        void reset_material(const D3DMATERIAL9 & material);
        
        void modulate_texture(DWORD texture_stage, bool modulate = true);
        void disable_texture(DWORD texture_stage = 0);
        void configure_sampler(DWORD sampler, bool linear_filtering, bool mipmapping);

    private:
        boost::shared_ptr<device_services> device;
    };
} // namespace d3d9
} // namespace video
} // namespace loci

#endif // LOCI_VIDEO_D3D9_FIXED_FUNCTION_PIPELINE_H_