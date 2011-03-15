/**
 * Implementation of texture_factory.
 * A 'manager' class for loading and caching textures.
 *
 * @file   texture_factory.cpp
 * @author David Gill
 * @date   25/04/2009
 */

#include <map>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "loci/platform/tstring.h"

#include "loci/video/d3d9/device_services.h"
#include "loci/video/d3d9/managed_texture.h"

#include "loci/video/resources/texture_factory.h"

namespace loci {
namespace video
{
    texture_factory::texture_factory(const boost::shared_ptr<d3d9::device_services> & device)
        : device(device)
    {
    }

    boost::shared_ptr<d3d9::managed_texture2d> texture_factory::get(const std::string & id)
    {
        return load_and_cache(LOCI_TSTR(""), id);
    }

    boost::shared_ptr<d3d9::managed_texture2d> texture_factory::try_get(const std::string & id) const
    {
        texture_cache::const_iterator iter = cache.find(id);
        if (iter == cache.end())
        {
            return boost::shared_ptr<d3d9::managed_texture2d>();
        }
        return iter->second;
    }

    void texture_factory::add(const platform::tstring & path, const std::string & id)
    {
        load_and_cache(path, id);
    }

    boost::shared_ptr<d3d9::managed_texture2d> texture_factory::load_and_cache(const platform::tstring & path, const std::string & id)
    {
        texture_cache::const_iterator iter = cache.find(id);
        if (iter != cache.end())
        {
            return iter->second;
        }

        if (path.empty())
        {
            return cache[id] = boost::make_shared<d3d9::managed_texture2d>(device, platform::to_tstr(id));
        }

        return cache[id] = boost::make_shared<d3d9::managed_texture2d>(device, path);
    }
} // namespace video
} // namespace loci