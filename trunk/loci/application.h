#ifndef LOCI_APPLICATION_H_
#define LOCI_APPLICATION_H_

/**
 * A utility interface for simple loci applications.
 * Provides an interface that can be used to create simple loci applications,
 * this is only a utility however. There is also a default implementation
 * provided that creates an application that executes a script.
 *
 * @file   application.h
 * @author David Gill
 * @date   03/05/2010
 */

#include <string>
#include <iostream>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include "loci/platform/tstring.h"
#include "loci/platform/win32/dispatch.h"

#include "loci/video/render_view.h"

#include "loci/crowd.h"
#include "loci/control/xml_controller.h"

namespace loci
{
    class application
    {
    protected:
        application() : first(0), last(1000), delta(100), temporal(false)
        {
        }

        void start(const platform::tstring & name, unsigned int width, unsigned int height,
                   const std::string & scene_path = "",
                   float scale = 1.0f)
        {
            canvas = boost::make_shared<video::render_view>(name, width, height, scene_path, scale);
            canvas->input_event_handler(boost::bind(&application::key_handler, this, _1));

            while (!loci::platform::win32::dispatch_messages())
            {
                if (temporal)
                {
                    canvas->render(update(), first, last, delta);
                    continue;
                }
                canvas->render(update());
            }
        }

        bool up_key()     const { return inputs.up;     }
        bool down_key()   const { return inputs.down;   }
        bool left_key()   const { return inputs.left;   }
        bool right_key()  const { return inputs.right;  }
        bool home_key()   const { return inputs.home;   }
        bool end_key()    const { return inputs.end;    }
        bool space_key()  const { return inputs.space;  }
        bool f1_key()     const { return inputs.f1;     }
        bool f2_key()     const { return inputs.f2;     }
		bool esc_key()    const { return inputs.esc;    }
        bool lshift_key() const { return inputs.lshift; }
        bool rshift_key() const { return inputs.rshift; }
        bool lctrl_key()  const { return inputs.lctrl;  }
        bool rctrl_key()  const { return inputs.rctrl;  }
        bool x_key()   const    { return inputs.x;      }
        bool z_key()   const    { return inputs.z;      }
        bool r_key()   const    { return inputs.r;      }


        void move_camera(float yrot, float zoom, float height)
        {
            if (canvas) { canvas->move_camera(yrot, zoom, height); }
        }

        void adjust_temporal_window(crowd::time_type first, crowd::time_type last, crowd::time_type delta)
        {
            this->first += first;
            this->first = this->first < 0 ? 0 : this->first;
            this->first = this->first > this->last ? this->last : this->first;

            this->last += last;
            this->last = this->last < this->first ? this->first : this->last;

            this->delta += delta;
            this->delta -= (this->delta <= 0 ? delta : 0);
        }

        void temporal_view(bool flag) { temporal = flag; }
        bool temporal_view() const { return temporal; }

    private:
        virtual crowd & update() = 0;

        void key_handler(const video::render_view::input_event & e)
        {
            if (e.keydown)
            {
                inputs.up     |= e.up;
                inputs.down   |= e.down;
                inputs.left   |= e.left;
                inputs.right  |= e.right;
                inputs.home   |= e.home;
                inputs.end    |= e.end;
                inputs.space  |= e.space;
                inputs.f1     |= e.f1;
                inputs.f2     |= e.f2;
				inputs.esc    |= e.esc;
                inputs.lshift |= e.lshift;
                inputs.rshift |= e.rshift;
                inputs.lctrl  |= e.lctrl;
                inputs.rctrl  |= e.rctrl;
                inputs.x      |= e.x;
                inputs.z      |= e.z;
				inputs.r      |= e.r;
            }
            else
            {
                inputs.up     &= !e.up;
                inputs.down   &= !e.down;
                inputs.left   &= !e.left;
                inputs.right  &= !e.right;
                inputs.home   &= !e.home;
                inputs.end    &= !e.end;
                inputs.space  &= !e.space;
                inputs.f1     &= e.f1;
                inputs.f2     &= e.f2;
				inputs.esc    &= e.esc;
                inputs.lshift &= e.lshift;
                inputs.rshift &= e.rshift;
                inputs.lctrl  &= e.lctrl;
                inputs.rctrl  &= e.rctrl;
                inputs.x      &= e.x;
                inputs.z      &= e.z;
				inputs.r      &= e.r;
            }
        }

    private:
        boost::shared_ptr<video::render_view> canvas;
        video::render_view::input_event inputs;
        crowd::time_type first, last, delta;
        bool temporal;
    };

    class script_view_application : public application
    {
    public:
        script_view_application() : controller_(new control::xml_crowd_controller()) { }

        script_view_application(const platform::tstring & name, unsigned int width, unsigned int height,
                                const std::string & script_path,
                                const std::string & scene_path = "", float scale = 1.0f)
            : controller_(new control::xml_crowd_controller(script_path))
        {
            application::start(name, width, height, scene_path, scale);
        }

        script_view_application(const platform::tstring & name, unsigned int width, unsigned int height,
                                const boost::shared_ptr<control::xml_crowd_controller> & controller,
                                const std::string & scene_path = "", float scale = 1.0f)
            : controller_(controller)
        {
            application::start(name, width, height, scene_path, scale);
        }

        void start(const platform::tstring & name, unsigned int width, unsigned int height,
                   const std::string & script_path,
                   const std::string & scene_path = "", float scale = 1.0f)
        {
            controller_->parse(script_path);
            application::start(name, width, height, scene_path, scale);
        }

        boost::shared_ptr<control::xml_crowd_controller> controller() const { return controller_; }

    private:
        crowd & update()
        {
            // camera controls
            if (left_key())  { move_camera(5, 0, 0);    }
            if (right_key()) { move_camera(-5, 0, 0);   }
            if (up_key())    { move_camera(0, 0, 10);   }
            if (down_key())  { move_camera(0, 0, -10);  }
            if (home_key())  { move_camera(0, -0.05, 0); }
            if (end_key())   { move_camera(0, 0.05, 0);  }

            // resetting
            if (space_key()) { controller_->reset(); }
			if (r_key())	 { controller_->reset(); }

            // temporal view on/off
            if (f2_key())    { temporal_view(true);  }
            if (f1_key())    { temporal_view(false); }
			if (esc_key())   { std::cout << "esc pressed: Quitting" << std::endl; ::PostQuitMessage(0); }

            // temporal window
            if (lshift_key()) { adjust_temporal_window(100, 0, 0);  }
            if (lctrl_key())  { adjust_temporal_window(-100, 0, 0); }
            if (rshift_key()) { adjust_temporal_window(0, 100, 0);  }
            if (rctrl_key())  { adjust_temporal_window(0, -100, 0); }
            if (x_key())      { adjust_temporal_window(0, 0, 10);   }
            if (z_key())      { adjust_temporal_window(0, 0, -10);  }
			

            return controller_->update(10);
        }

        boost::shared_ptr<control::xml_crowd_controller> controller_;
    };
} // namespace loci

#endif // LOCI_APPLICATION_H_