#ifndef LOCI_MOCAPS_PARSE_RECORDING_H_
#define LOCI_MOCAPS_PARSE_RECORDING_H_

/**
 * Records animations into motion capture entities.
 * Samples anim_graph animations and records their frames as to be used as
 * motion captures.
 *
 * @file   recording.h
 * @author David Gill
 * @date   22/04/2010
 */

#include <string>

#include "loci/anim/nodes/anim_graph.h"
#include "loci/mocaps/bvh_document.h"

namespace loci {
namespace mocaps
{
    bvh_document record_to_bvh_document(anim::anim_graph & ag, double frame_rate, double duration, bool reset_on_finish = true)
    {
        bvh_document bvh(ag.figure(), frame_rate, duration);
        frame_rate *= 1000;
        duration *= 1000;
        while (duration > 0)
        {
            bvh.append_frame( *ag.animate(frame_rate) );
            duration -= frame_rate;
        }

        if (reset_on_finish) { ag.reset(); }

        return bvh;
    }

    void record_to_bvh_file(const std::string & path, anim::anim_graph & ag, double frame_rate, double duration, bool reset_on_finish = true)
    {
        bvh_document bvh(ag.figure(), frame_rate, duration);
        frame_rate *= 1000;
        duration *= 1000;
        while (duration > 0)
        {
            bvh.append_frame( *ag.animate(frame_rate) );
            duration -= frame_rate;
        }
        export_bvh_to_file(bvh, path);

        if (reset_on_finish) { ag.reset(); }
    }
} // namespace mocaps
} // namespace loci

#endif // LOCI_MOCAPS_PARSE_RECORDING_H_