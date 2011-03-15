#ifndef LOCI_CONTROL_XML_MOTION_SET_H_
#define LOCI_CONTROL_XML_MOTION_SET_H_

#include <string>
#include <map>
#include <utility> // for make_pair

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "loci/mocaps/bvh_document.h"
#include "loci/skeleton.h"
#include "loci/anim/time_info.h"

namespace loci {
namespace control {
namespace xml
{
    class motion_set
    {
    public:

        typedef mocaps::bvh_document::mocap mocap;
        typedef mocap::motion_type motion_type;

        struct motion_details
        {
            motion_details(const boost::shared_ptr<const motion_type> & m,
                           float yoffset,
                           const boost::shared_ptr<loci::anim::sync_warp> & s
#ifdef LOCI_DEBUGGING
                           , float r, float g, float b)
                           : motion(m), syncher(s), yoffset(yoffset), r(r), g(g), b(b)
#else
                           ) : motion(m), syncher(s), yoffset(yoffset)
#endif
            {
            }

            boost::shared_ptr<const motion_type> motion;
            boost::shared_ptr<loci::anim::sync_warp> syncher;
            float yoffset;
#ifdef LOCI_DEBUGGING
            float r, g, b;
#endif

            bool operator<(const motion_details & m) const { return motion < m.motion; }
        };


        void add(const std::string & id,
                 const std::string & path,
                 float yoffset,
                 const boost::shared_ptr<loci::anim::sync_warp> & syncher,
#ifdef LOCI_DEBUGGING
                 float r, float g, float b,
#endif
                 bool discard_first_frame = true)
        {
            path_bindings::iterator it = bindings.find(path);
            if (it == bindings.end()) // not loaded...
            {
                
#ifdef LOCI_DEBUGGING
                load(path, id, yoffset, syncher, discard_first_frame, r, g, b);
#else
                load(path, id, yoffset, syncher, discard_first_frame);
#endif
                    
            }
            else
            {
                cache.insert(std::make_pair(id, cache.find(it->second)->second));
            }
        }

        motion_details acquire_motion(
            const std::string & id,
            float yoffset = 0.0f,
            const boost::shared_ptr<loci::anim::sync_warp> & syncher = boost::shared_ptr<loci::anim::sync_warp>(),
#ifdef LOCI_DEBUGGING
            float r = 1.0, float g = 1.0, float b = 0.0,
#endif
            bool discard_first_frame = true)
        {
            motion_cache::iterator it = cache.find(id);
            if (it != cache.end())
            {
                return it->second;
            }

#ifdef LOCI_DEBUGGING
            return load(id, id, yoffset, syncher, discard_first_frame, r, g, b);
#else
            return load(id, id, yoffset, syncher, discard_first_frame);
#endif
        }

        skeleton figure()
        {
            return *figure_;
        }

    private:
#ifdef LOCI_DEBUGGING
        motion_details load(const std::string & path, const std::string & id, float yoffset, const boost::shared_ptr<loci::anim::sync_warp> & syncher, bool discard_first_frame, float r, float g, float b)
#else
        motion_details load(const std::string & path, const std::string & id, float yoffset, const boost::shared_ptr<loci::anim::sync_warp> & syncher, bool discard_first_frame)
#endif
        {
            mocap m = mocaps::import_bvh_file(path, discard_first_frame);
            if (cache.empty()) // first entry so cache skeleton
            {
                figure_ = boost::make_shared<skeleton>(m.figure());
            }
            bindings.insert(std::make_pair(path, id));
#ifdef LOCI_DEBUGGING
            motion_details deets(m.motion(), yoffset, syncher, r, g, b);
#else
            motion_details deets(m.motion(), yoffset, syncher);
#endif
            cache.insert(std::make_pair(id, deets));
            return deets;
        }

        typedef std::map<std::string, motion_details> motion_cache;
        typedef std::map<std::string, std::string> path_bindings;

        motion_cache cache;
        path_bindings bindings;
        boost::shared_ptr<skeleton> figure_;
    };
} // namespace xml
} // namespace control
} // namespace loci

#endif // LOCI_CONTROL_XML_MOTION_SET_H_
