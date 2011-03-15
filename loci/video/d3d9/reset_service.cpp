/**
 * Implementation of a reset_notifier.
 * Provides services for handling lost devices and resets.
 *
 * @file   reset_notifier.cpp
 * @author David Gill
 * @date   07/11/2009
 */

#include <boost/shared_ptr.hpp>

#include "loci/platform/win32/windows_common.h" // for Sleep
#include "loci/video/d3d9/reset_service.h"
#include "loci/video/d3d9/device_services.h"

namespace loci {
namespace video {
namespace d3d9
{
    // forward declaration
    class display_settings;

    reset_service::reset_service(const boost::shared_ptr<device_services> & device)
        : device(device)
    {
    }

    bool reset_service::ready()
    {
        return device->cooperative(false);
    }

    bool reset_service::try_reset(const display_settings & config)
    {
        if (!device->cooperative(true)) { return false; }
        return device->try_reset(config);
    }
} // namespace d3d9
} // namespace video
} // namespace loci