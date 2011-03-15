/**
 * Implementation of a reset_notifier.
 * High level rendering service.
 *
 * @file   render_view.cpp
 * @author David Gill
 * @date   08/11/2009
 */

#include <vector>

#include "loci/platform/tstring.h"
#include "loci/platform/tsstream.h"

#include "loci/platform/win32/windows_common.h"
#include "loci/platform/win32/window.h"

#include "loci/video/render_view.h"
#include "loci/video/d3d9/display_settings.h"
#include "loci/video/renderers/renderer.h"
#include "loci/video/scene_setup.h"

#include "loci/skeleton.h"
#include "loci/pose.h"
#include "loci/crowd.h"

namespace loci {
namespace video
{
    // using declaration
    using platform::win32::window;

    render_view::render_view(const platform::tstring & name,
                             unsigned int width, unsigned int height,
                             const std::string & scene_path, float scale)
        : details(window(name, &wndproc, width, height),
                  d3d9::display_settings(width, height, false),
                  scene_path, scale)
    {
        show(details.w); // show first
        details.w.data_ptr(this); // set self as the window's user data
    }

    render_view::render_view(const platform::tstring & name,
                             unsigned int width, unsigned int height,
                             input_event_delegate input_handler,
                             const std::string & scene_path, float scale)
        : details(window(name, &wndproc, width, height),
                  d3d9::display_settings(width, height, false),
                  scene_path, scale)
        , input_handler(input_handler)
    {
        show(details.w); // show first
        details.w.data_ptr(this); // set self as the window's user data
    }

    void render_view::render(const crowd & people)
    {
        platform::otstringstream otss;
        otss << "Time: " << people.time();
        details.r.render(people, otss.str());
    }

    void render_view::render(crowd & people, crowd::time_type start, crowd::time_type stop, crowd::time_type delta)
    {
        start = start < 0 ? 0 : start;
        stop = stop < start ? start : stop;
        if (delta <= 0) { stop = start; delta = 1; }

        platform::otstringstream otss;
        otss << "Time: " << start << " --> " << stop;

        std::vector< std::vector<pose> > trails(people.size());
        for (crowd::size_type i = 0; i < people.size(); ++i)
        {
            trails[i].reserve((stop - start) / delta + 2);
        }

        people.reset();
        people.update(start);
        crowd::time_type time = start;
        while (time < stop)
        {
            people.update(delta);

            time += delta;
            if (time > stop) { delta = stop - time; }

            for (crowd::size_type i = 0; i < people.size(); ++i)
            {
                trails[i].push_back(*people.person(i).bind_pose());
            }
        }

        details.r.render(people, otss.str(), trails);
    }

    render_view::window_dependant_renderer::window_dependant_renderer(
            const platform::win32::window & w,
            const d3d9::display_settings & config,
            const std::string & scene_path,
            float scale)
        : w(w)
        , r(setup_scene_renderer(w.get(), config, scene_path, scale))
    {
    }

    LRESULT CALLBACK render_view::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        LONG_PTR data = GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (data != 0)
        {
            render_view * rv =  reinterpret_cast<render_view *>(data);
            input_event_delegate & input_handler = rv->input_handler;

            switch (msg)
            {
                case WM_KEYDOWN:
                case WM_KEYUP:
                    if (input_handler)
                    {
                        input_handler(input_event(
                            msg == WM_KEYDOWN,
                            wparam == VK_UP,
                            wparam == VK_DOWN,
                            wparam == VK_LEFT,
                            wparam == VK_RIGHT,
                            wparam == VK_HOME,
                            wparam == VK_END,
                            wparam == VK_SPACE,
                            wparam == VK_F1,
                            wparam == VK_F2,
							wparam == VK_ESCAPE,
                            GetAsyncKeyState(VK_LSHIFT),
                            GetAsyncKeyState(VK_RSHIFT),
                            GetAsyncKeyState(VK_LCONTROL),
                            GetAsyncKeyState(VK_RCONTROL),
                            GetAsyncKeyState('X'),
							GetAsyncKeyState('Z'),
                            GetAsyncKeyState('R')
							));
                    }
                    break;
                case WM_CLOSE:
                    ::PostQuitMessage(0);
                    return 0;

                case WM_SIZE:
                        renderer & r =  rv->details.r;
                        r.reset_device(
                            d3d9::display_settings(
                                static_cast<unsigned int>(LOWORD(lparam)), // width
                                static_cast<unsigned int>(HIWORD(lparam)), // height
                                false));                                   // fullscreen
            }
        }
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
} // namespace video
} // namespace loci