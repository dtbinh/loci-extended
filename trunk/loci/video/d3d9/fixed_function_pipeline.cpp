/**
 * Implementation of fixed_function_pipeline.
 * Provides a specific interface for configuring fixed-function states.
 *
 * @file   fixed_function_pipeline.cpp
 * @author David Gill
 * @date   15/02/2009
 */

#include <boost/shared_ptr.hpp>

#include <d3d9.h>
#include <d3dx9.h>

#include "loci/video/d3d9/fixed_function_pipeline.h"
#include "loci/video/d3d9/device_services.h"

namespace loci {
namespace video {
namespace d3d9
{
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
                                 float attenuation2)
    {
        light.Type         = type;
        light.Diffuse      = diffuse;
        light.Specular     = specular;
        light.Ambient      = ambient;
        light.Position     = position;
        light.Direction    = direction;
        light.Range        = range;
        light.Falloff      = falloff;
        light.Attenuation0 = attenuation0;
        light.Attenuation1 = attenuation1;
        light.Attenuation2 = attenuation2;
        light.Theta        = theta;
        light.Phi          = phi;
        return light;
    }

    D3DLIGHT9 & create_point_light(D3DLIGHT9 & light,
                                   const D3DCOLORVALUE & diffuse,
                                   const D3DCOLORVALUE & specular,
                                   const D3DCOLORVALUE & ambient,
                                   const D3DVECTOR & position,
                                   float range,
                                   float attenuation0,
                                   float attenuation1,
                                   float attenuation2)
    {
        return initialise_light(
            light,
            D3DLIGHT_POINT,
            diffuse,
            specular,
            ambient,
            position,
            D3DVECTOR(),
            range,
            0.0f, 0.0f, 0.0f,
            attenuation0,
            attenuation1,
            attenuation2);
    }

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
                                  float attenuation2)
    {
        return initialise_light(
            light,
            D3DLIGHT_SPOT,
            diffuse,
            specular,
            ambient,
            position,
            direction,
            range,
            falloff,
            theta,
            phi,
            attenuation0,
            attenuation1,
            attenuation2);
    }

    D3DLIGHT9 & create_directional_light(D3DLIGHT9 & light,
                                         const D3DCOLORVALUE & diffuse,
                                         const D3DCOLORVALUE & specular,
                                         const D3DCOLORVALUE & ambient,
                                         const D3DVECTOR & direction)
    {
        return initialise_light(
            light,
            D3DLIGHT_DIRECTIONAL,
            diffuse,
            specular,
            ambient,
            D3DVECTOR(),
            direction,
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    }


    D3DMATERIAL9 & initialise_material(D3DMATERIAL9 & material,
                                       const D3DCOLORVALUE & diffuse,
                                       const D3DCOLORVALUE & specular,
                                       const D3DCOLORVALUE & ambient,
                                       const D3DCOLORVALUE & emissive,
                                       float power)
    {
        material.Diffuse = diffuse;
        material.Specular = specular;
        material.Ambient = ambient;
        material.Emissive = emissive;
        material.Power = power;
        return material;
    }


    fixed_function_pipeline::fixed_function_pipeline(
        const boost::shared_ptr<device_services> & device)
            : device(device)
    {
    }

    void fixed_function_pipeline::reset_projection(float fovy, float znear, float zfar)
    {
            float aspect = static_cast<float>(device->backbuffer_width()) /
                           static_cast<float>(device->backbuffer_height());
            D3DXMATRIX proj_mat;
            D3DXMatrixPerspectiveFovRH(&proj_mat, D3DXToRadian(fovy), aspect, znear, zfar);
            device->bind_transform(D3DTS_PROJECTION, proj_mat);
    }

    void fixed_function_pipeline::reset_view(float px, float py, float pz,
                                             float ax, float ay, float az,
                                             float ux, float uy, float uz,
                                             float rotation)
    {
        D3DXVECTOR3 eye(px, py, pz); // position
        D3DXVECTOR3 at(ax, ay, az);  // look-at
        D3DXVECTOR3 up(ux, uy, uz);  // up);
        
        D3DXMATRIX look_mat;
        D3DXMATRIX rot_mat;
        D3DXMATRIX view_mat;

        D3DXMatrixLookAtRH(&look_mat, &eye, &at, &up);
        D3DXMatrixRotationY(&rot_mat, D3DXToRadian(rotation));
        D3DXMatrixMultiply(&view_mat, &rot_mat, &look_mat);

        device->bind_transform(D3DTS_VIEW, view_mat);
    }

    void fixed_function_pipeline::reset_transform(const D3DMATRIX & transformation)
    {
        device->bind_transform(D3DTS_WORLD, transformation);
    }

    void fixed_function_pipeline::culling(DWORD mode)
    {
        device->bind_render_state(D3DRS_CULLMODE, mode);
    }

    void fixed_function_pipeline::alpha_blending(bool enabled)
    {
        device->bind_render_state(D3DRS_ALPHABLENDENABLE, enabled ? TRUE : FALSE);
    }

    void fixed_function_pipeline::configure_alpha_blending(DWORD src, DWORD dest, bool activate)
    {
        device->bind_render_state(D3DRS_SRCBLEND, src);
        device->bind_render_state(D3DRS_DESTBLEND, dest);
        if (activate)
        {
            alpha_blending(true);
        }
    }
 
    void fixed_function_pipeline::lighting(bool enabled, bool normalise, bool specular, DWORD specular_source)
    {
        const DWORD light_mode     = enabled   ? TRUE : FALSE;
        const DWORD normalise_mode = normalise ? TRUE : FALSE;
        const DWORD specular_mode  = specular  ? TRUE : FALSE;
        device->bind_render_state(D3DRS_LIGHTING              , light_mode);
        device->bind_render_state(D3DRS_NORMALIZENORMALS      , normalise_mode);
        device->bind_render_state(D3DRS_SPECULARENABLE        , specular_mode);
        device->bind_render_state(D3DRS_SPECULARMATERIALSOURCE, specular_source);
    }

    void fixed_function_pipeline::configure_light(unsigned int light,
                                                  const D3DLIGHT9 & descriptor,
                                                  bool activate)
    {
        device->configure_light(light, descriptor);
        if (activate)
        {
            enable_light(light, true);
        }
    }

    void fixed_function_pipeline::enable_light(unsigned int light, bool enabled)
    {
        device->enable_light(light, enabled);
    }

    void fixed_function_pipeline::reset_material(const D3DMATERIAL9 & material)
    {
        device->reset_material(material);
    }

    void fixed_function_pipeline::modulate_texture(DWORD texture_stage, bool modulate)
    {
        DWORD colour_operation = modulate ? D3DTOP_MODULATE : D3DTOP_SELECTARG1;
        device->bind_texture_stage_state(texture_stage, D3DTSS_COLOROP, colour_operation);
        device->bind_texture_stage_state(texture_stage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        device->bind_texture_stage_state(texture_stage, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    }

    void fixed_function_pipeline::disable_texture(DWORD texture_stage)
    {
        device->bind_texture(0, texture_stage);
    }

    void fixed_function_pipeline::configure_sampler(DWORD sampler, bool linear_filtering, bool mipmapping)
    {
        DWORD filter = linear_filtering ? D3DTEXF_LINEAR : D3DTEXF_POINT;
        device->bind_sampler_state(sampler, D3DSAMP_MINFILTER, filter);
        device->bind_sampler_state(sampler, D3DSAMP_MAGFILTER, filter);
        device->bind_sampler_state(sampler, D3DSAMP_MIPFILTER, mipmapping ? filter : D3DTEXF_NONE);
    }
} // namespace d3d9
} // namespace video
} // namespace loci