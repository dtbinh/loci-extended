#ifndef LOCI_VIDEO_RENDER_VIEW_H_
#define LOCI_VIDEO_RENDER_VIEW_H_

/**
 * High level rendering service.
 * Provides an interface for rendering skeletons.
 *
 * @file   render_view.h
 * @author David Gill
 * @date   08/11/2009
 */

#include <vector>

#include <boost/function.hpp>

#include "loci/platform/tstring.h"
#include "loci/platform/win32/window.h"
#include "loci/video/renderers/renderer.h"
#include "loci/skeleton.h"
#include "loci/pose.h"
#include "loci/crowd.h"

namespace loci {
namespace video
{
    // forward declaration
    namespace d3d9
    {
        class display_settings;
    }

    class render_view
    {
    public:
        class input_event
        {
        public:
            input_event(bool keydown = false,
                        bool up = false, bool down = false, bool left = false, bool right = false,
                        bool home = false, bool end = false,
                        bool space = false,
                        bool f1 = false, bool f2 = false,
						bool esc = false,	
                        bool lshift = false, bool rshift = false,
                        bool lctrl = false, bool rctrl = false,
                        bool x = false, bool z = false, bool r = false)
                : keydown(keydown)
                , up(up), down(down), left(left), right(right)
                , home(home), end(end)
                , space(space)
                , f1(f1), f2(f2)
				, esc(esc)
                , lshift(lshift), rshift(rshift)
                , lctrl(lctrl), rctrl(rctrl)
                , x(x), z(z), r(r)
            {
            }

            bool keydown;
            bool up, down, left, right;
            bool home, end;
            bool space;
            bool f1, f2;
			bool esc;
            bool lshift, rshift;
            bool lctrl, rctrl;
            bool x, z, r;
        };

        typedef boost::function1<void, const input_event &> input_event_delegate;

        render_view(const platform::tstring & name,
                    unsigned int width, unsigned int height,
                    const std::string & scene_path = "",
                    float scale = 1.0f);

        render_view(const platform::tstring & name,
                    unsigned int width, unsigned int height,
                    input_event_delegate input_handler,
                    const std::string & scene_path = "",
                    float scale = 1.0f);

        void input_event_handler(input_event_delegate input_handler)
        {
            this->input_handler = input_handler;
        }

        void move_camera(float yrot, float zoom, float height) { details.r.move_camera(yrot, zoom, height); }

        void render(const crowd & people);
        void render(crowd & people, crowd::time_type start, crowd::time_type stop, crowd::time_type delta);

    private:
        // helper
        struct window_dependant_renderer
        {
            window_dependant_renderer(const platform::win32::window & w,
                                      const d3d9::display_settings & config,
                                      const std::string & scene_path,
                                      float scale);

            // ordering important for proper destruction
            platform::win32::window w;
            renderer r;
        };

        static LRESULT CALLBACK wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    private:
        window_dependant_renderer details;
        input_event_delegate input_handler;
    };
} // namespace video
} // namespace loci

#endif // LOCI_VIDEO_RENDER_VIEW_H_