#ifndef LOCI_VIDEO_D3D9_RESET_NOTIFIER_H_
#define LOCI_VIDEO_D3D9_RESET_NOTIFIER_H_

/**
 * Provides services for handling lost devices and resets.
 * Wraps the device interface to provide a semantically-rich interface for
 * restting.
 *
 * @file   reset_service.h
 * @author David Gill
 * @date   07/11/2009
 */

#include <boost/shared_ptr.hpp>

#include "loci/video/d3d9/display_settings.h"

namespace loci {
namespace video {
namespace d3d9
{
    // forward declaration
    class device_services;

    class reset_service
    {
    public:
        reset_service(const boost::shared_ptr<device_services> & device);

        bool ready();
        bool try_reset(const display_settings & config);

    private:
        boost::shared_ptr<device_services> device;
    };

} // namespace d3d9
} // namespace video
} // namespace loci

#endif // LOCI_VIDEO_D3D9_RESET_NOTIFIER_H_