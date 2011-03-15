#ifndef LOCI_VIDEO_TEXTURE_FACTORY_H_
#define LOCI_VIDEO_TEXTURE_FACTORY_H_

/**
 * A 'manager' class for loading and caching textures.
 * Loads d3d9 textures and caches them for multiple requests.
 *
 * @file   texture_factory.h
 * @author David Gill
 * @date   25/04/2009
 */

#include <map>
#include <string>

#include <boost/shared_ptr.hpp>

#include "loci/platform/tstring.h"

#include "loci/video/d3d9/managed_texture.h"

#include "loci/video/resources/file_resource_cache.h"

namespace loci {
namespace video
{
    // forward declaration
    namespace d3d9
    {
        class device_services;
    }

    class texture_factory : public file_resource_cache
    {
    public:
        texture_factory(const boost::shared_ptr<d3d9::device_services> & device);

        boost::shared_ptr<d3d9::managed_texture2d> get(const std::string & id);
        boost::shared_ptr<d3d9::managed_texture2d> try_get(const std::string & id) const;

    private:
        void add(const platform::tstring & path, const std::string & id);
        boost::shared_ptr<d3d9::managed_texture2d> load_and_cache(const platform::tstring & path, const std::string & id);

        typedef std::map<std::string, boost::shared_ptr<d3d9::managed_texture2d> > texture_cache;

        texture_cache cache;
        boost::shared_ptr<d3d9::device_services> device;
    };
} // namespace video
} // namespace loci

#endif // LOCI_VIDEO_TEXTURE_FACTORY_H_