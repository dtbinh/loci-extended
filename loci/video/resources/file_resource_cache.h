#ifndef LOCI_VIDEO_FILE_RESOURCE_CACHE_H_
#define LOCI_VIDEO_FILE_RESOURCE_CACHE_H_

/**
 * An interface for resource caches that load resources from files.
 * Provides an interface to notify that a resource should be loaded
 * whilst also mapping a resource id to a file path to refer to it later.
 *
 * @file   file_resource_cache.h
 * @author David Gill
 * @date   25/04/2009
 */

#include <string>

#include "loci/platform/tstring.h"

namespace loci {
namespace video
{
    class file_resource_cache
    {
    public:
        void cache(const platform::tstring & path, const std::string & id)
        {
            add(path, id);
        }

    private:
        virtual void add(const platform::tstring & path, const std::string & id) = 0;
    };
} // namespace video
} // namespace loci

#endif // LOCI_VIDEO_FILE_RESOURCE_CACHE_H_