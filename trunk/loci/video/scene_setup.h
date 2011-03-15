
#include "loci/platform/win32/windows_common.h"
#include "loci/platform/tstring.h"
#include "loci/video/d3d9/display_settings.h"

namespace loci {
namespace video
{
    // forward declaration
    class renderer;

    renderer setup_scene_renderer(HWND output_window,
                                  const d3d9::display_settings & config = d3d9::display_settings(),
                                  const std::string & scene_path = "",
                                  float scale = 1.0f);

    renderer setup_scene_renderer(HWND output_window,
                                  const d3d9::display_settings & config,
                                  const platform::tstring & font_name,
                                  unsigned int font_size,
                                  float bone_radius,
                                  unsigned int joint_slices,
                                  unsigned int joint_stacks,
                                  const std::string & scene_path = "",
                                  float scale = 1.0f);
} // namespace video
} // namespace loci